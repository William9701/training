#include "shell.h"


/**
 * free_args - Frees up memory taken by args.
 * @args: A null-terminated double pointer containing commands/arguments.
 * @front: A double pointer to the beginning of args.
 */
void free_args(char **args, char **front)
{
	size_t i = 0;

	while (args[i] || args[i + 1])
	{
		free(args[i]);
		i++;
	}

	free(front);
}


/**
 * get_pid - Gets the current process ID.
 * Description: Opens the stat file, a space-delimited file containing
 *              information about the current process. The PID is the
 *              first word in the file. The function reads the PID into
 *              a buffer and replace the space at the end with a \0 byte.
 *
 * Return: The current process ID or NULL on failure.
 */
char *get_pid(void)
{
	size_t i = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - Gets the value corresponding to an environmental variable.
 * @beginning: The environmental variable to search for.
 * @len: The length of the environmental variable to search for.
 *
 * Return: If the variable is not found - an empty string.
 *         Otherwise - the value of the environmental variable.
 *
 * Description: Variables are stored in the format VARIABLE=VALUE.
 */
char *get_env_value(char *name_begin, int name_len)
{
    char **env_address;
    char *value = NULL;
    char *temp;
    char *name_copy;

    name_copy = malloc(name_len + 1);
    if (!name_copy)
        return NULL;
    name_copy[0] = '\0';
    _strncat(name_copy, name_begin, name_len);

    env_address = _getenv(name_copy);
    free(name_copy);
    if (env_address)
    {
        temp = *env_address;
        while (*temp != '=')
            temp++;
        temp++;
        value = malloc(_strlen(temp) + 1);
        if (value)
            _strcpy(value, temp);
    }

    return value;
}



/**
 * variable_replacement - Handles variable replacement.
 * @line: A double pointer containing the command and arguments.
 * @exe_ret: A pointer to the return value of the last executed command.
 * Description: Replaces $$ with the current PID, $? with the return value
 *              of the last executed program, and envrionmental variables
 *              preceded by $ with their corresponding value.
 */
void replace_variable(char **line, int *exe_ret)
{
    int i = 0, variable_len;
    char *replacement = NULL, *old_str = NULL, *new_str;

    old_str = *line;
    while (old_str[i])
    {
        if (old_str[i] == '$' && old_str[i + 1] && old_str[i + 1] != ' ')
        {
            if (old_str[i + 1] == '$')
            {
                replacement = get_pid();
                i += 2;
            }
            else if (old_str[i + 1] == '?')
            {
                replacement = _itoa(*exe_ret);
                i += 2;
            }
            else if (old_str[i + 1])
            {
                /* extract the variable name to search for */
                int j = i + 1;
                while (old_str[j] && old_str[j] != '$' && old_str[j] != ' ')
                    j++;
                variable_len = j - (i + 1);
                replacement = get_env_value(&old_str[i + 1], variable_len);
                i = j;
            }
            new_str = malloc(i + _strlen(replacement) + _strlen(&old_str[i]) + 1);
            if (!new_str)
                return;
            new_str[0] = '\0';
            _strncat(new_str, old_str, i);
            if (replacement)
            {
                _strcat(new_str, replacement);
                free(replacement);
                replacement = NULL;
            }
            _strcat(new_str, &old_str[i]);
            free(old_str);
            *line = new_str;
            old_str = new_str;
            i = -1;
        }
        i++;
    }
}

