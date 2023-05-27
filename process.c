#include "shell.h"

/**
 * cant_open - If the file doesn't exist or lacks proper permissions, print
 * a cant open error.
 * @file_path: Path to the supposed file.
 *
 * Return: 127.
 */

int cant_open(char *file_path)
{
	char *perror, *hist_str;
	int lenght;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (127);

	lenght = _strlen(name) + _strlen(hist_str) + _strlen(file_path) + 16;
	perror = malloc(sizeof(char) * (lenght + 1));
	if (!perror)
	{
		free(hist_str);
		return (127);
	}

	_strcpy(perror, name);
	_strcat(perror, ": ");
	_strcat(perror, hist_str);
	_strcat(perror, ": Can't open ");
	_strcat(perror, file_path);
	_strcat(perror, "\n");

	free(hist_str);
	write(STDERR_FILENO, perror, lenght);
	free(perror);
	return (127);
}

/**
 * proc_file_commands - Takes a file and attempts to run the commands stored
 * within.
 * @file_path: Path to the file.
 * @exe_ret: Return value of the last executed command.
 *
 * Return: If file couldn't be opened - 127.
 *	   If malloc fails - -1.
 *	   Otherwise the return value of the last command ran.
 */
int proccess_commands(char *file_path, int *exit_status)
{
    ssize_t file, bytes_read;
    unsigned int line_capacity = 0;
    unsigned int old_capacity = 120;
    char* line;
    char** arguments;
    char** front;
    char buffer[120];
    int ret;
    unsigned int i;

    hist = 0;
    file = open(file_path, O_RDONLY);
    if (file == -1)
    {
        *exit_status = cant_open(file_path);
        return *exit_status;
    }

    line = malloc(sizeof(char) * old_capacity);
    if (!line)
        return -1;

    do {
        bytes_read = read(file, buffer, 119);
        if (bytes_read == 0 && line_capacity == 0)
            return *exit_status;

        buffer[bytes_read] = '\0';
        line_capacity += bytes_read;
        line = _realloc(line, old_capacity, line_capacity);
        _strcat(line, buffer);
        old_capacity = line_capacity;
    } while (bytes_read);

    i = 0;
    do {
        if (line[i] == '\n')
            line[i] = ';';
        i++;
    } while (i < line_capacity);

    i = 0;
    while (i < line_capacity && line[i] == '\n') {
        line[i] = ' ';
        i++;
    }

    i = 0;
    while (i < line_capacity) {
        if (line[i] == '\n') {
            line[i] = ';';
            i++;
            while (i < line_capacity && line[i] == '\n') {
                line[i] = ' ';
                i++;
            }
        }
        i++;
    }

    replace_variable(&line, exit_status);
    process_line(&line, line_capacity);
    arguments = _strtok(line, " ");
    free(line);

    if (!arguments)
        return 0;

    if (check_args(arguments) != 0) {
        *exit_status = 2;
        free_args(arguments, arguments);
        return *exit_status;
    }

    front = arguments;
    for (i = 0; arguments[i]; i++) {
        if (_strncmp(arguments[i], ";", 1) == 0) {
            free(arguments[i]);
            arguments[i] = NULL;
            ret = prepare_args(arguments, front, exit_status);
            arguments = &arguments[++i];
            i = 0;
        }
    }

    ret = prepare_args(arguments, front, exit_status);
    free(front);

    return ret;
}

