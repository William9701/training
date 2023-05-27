#include "shell.h"

/**
 * get_location - Locates a command in the PATH.
 * @command: The command to locate.
 * Return: If an error occurs or the command cannot be located - NULL.
 *         Otherwise - the full pathname of the command.
 */
char *get_path(char *command)
{
	char **path_list;
	char *temp_path;
	struct stat file_stat;
	list_t *dirs;
	list_t *current_dir;

    path_list = _getenv("PATH");
    if (!path_list || !(*path_list))
        return NULL;

    dirs = get_path_dir(*path_list + 5);
    current_dir = dirs;

    

    while (current_dir)
    {
        temp_path = malloc(_strlen(current_dir->dir) + _strlen(command) + 2);
        if (!temp_path)
            return NULL;

        _strcpy(temp_path, current_dir->dir);
        _strcat(temp_path, "/");
        _strcat(temp_path, command);

        if (stat(temp_path, &file_stat) == 0)
        {
            free_list(dirs);
            return temp_path;
        }

        current_dir = current_dir->next;
        free(temp_path);
    }

    free_list(dirs);
    return NULL;
}


/**
 * fill_path_dir - Copies path but also replaces leading/sandwiched/trailing
 *		   colons (:) with current working directory.
 * @path: The colon-separated list of directories.
 * Return: A copy of path with any leading/sandwiched/trailing colons replaced
 *	   with the current working directory.
 */
char *fill_path_dir(char *path)
{
    char *pwd = *(_getenv("PWD")) + 4;
    int path_length = _strlen(path);
    int new_length = 0;
    char *path_copy;
    int i;

    for (i = 0; i < path_length; i++)
    {
        if (path[i] == ':')
        {
            if (i == 0 || path[i + 1] == ':' || path[i + 1] == '\0')
                new_length += _strlen(pwd) + 1;
            else
                new_length++;
        }
        else
        {
            new_length++;
        }
    }

    path_copy = malloc(sizeof(char) * (new_length + 1));
    if (!path_copy)
        return NULL;

    path_copy[0] = '\0';

    for (i = 0; i < path_length; i++)
    {
        if (path[i] == ':')
        {
            if (i == 0)
            {
                _strcat(path_copy, pwd);
                _strcat(path_copy, ":");
            }
            else if (path[i + 1] == ':' || path[i + 1] == '\0')
            {
                _strcat(path_copy, ":");
                _strcat(path_copy, pwd);
            }
            else
            {
                _strcat(path_copy, ":");
            }
        }
        else
        {
            _strncat(path_copy, &path[i], 1);
        }
    }

    return (path_copy);
}


/**
 * get_path_dir - Tokenizes a colon-separated list of
 *                directories into a list_s linked list.
 * @path: The colon-separated list of directories.
 * Return: A pointer to the initialized linked list.
 */
list_t *get_path_dir(char *path)
{
    char **path_tokens, *path_copy;
    list_t *head = NULL;
    int token_count = 0;
    int i = 0;

    path_copy = fill_path_dir(path);
    if (!path_copy)
        return NULL;

    path_tokens = _strtok(path_copy, ":");
    free(path_copy);
    if (!path_tokens)
        return NULL;

    while (path_tokens[i])
    {
        if (add_node_end(&head, path_tokens[i]) == NULL)
        {
            free_list(head);
            free(path_tokens);
            return NULL;
        }
        token_count++;
        i++;
    }

    free(path_tokens);

    return head;
}

