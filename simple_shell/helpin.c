#include "shell.h"

/**
 * get_args - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *find_args(char *line, int *exit_status)
{
	size_t buffer = 0;
	ssize_t get;

	if (line)
		free(line);

	get = _getline(&line, &buffer, STDIN_FILENO);
	if (get == -1)
		return (NULL);
	if (get == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
		return (find_args(line, exit_status));
	}

	line[get - 1] = '\0';
	replace_variable(&line, exit_status);
	process_line(&line, get);

	return (line);
}

/**
 * call_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @cmd_front: A double pointer to the beginning of args.
 * @exit_status: The return value of the parent process' last executed command.
 * Return: The return value of the last executed command.
 */
int prepare_args(char **args, char **cmd_front, int *exit_status)
{
	int result, idx = 0;

	if (!args[0])
		return (*exit_status);

	while (args[idx])
	{
		if (_strncmp(args[idx], "||", 2) == 0)
		{
			free(args[idx]);
			args[idx] = NULL;
			args = replace_aliases(args);
			result = execute_args(args, cmd_front, exit_status);
			if (*exit_status != 0)
			{
				args = &args[++idx];
				idx = 0;
			}
			else
			{
				while (args[idx])
					free(args[idx++]);
				return (result);
			}
		}
		else if (_strncmp(args[idx], "&&", 2) == 0)
		{
			free(args[idx]);
			args[idx] = NULL;
			args = replace_aliases(args);
			result = execute_args(args, cmd_front, exit_status);
			if (*exit_status == 0)
			{
				args = &args[++idx];
				idx = 0;
			}
			else
			{
				while (args[idx])
					free(args[idx++]);
				return (result);
			}
		}
		idx++;
	}

	args = replace_aliases(args);
	result = execute_args(args, cmd_front, exit_status);
	return (result);
}


/**
 * run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int execute_args(char **args, char **front, int *exit_status)
{
	int result, i = 0;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		result = builtin(args + 1, front);
		if (result != EXIT)
			*exit_status = result;
	}
	else
	{
		*exit_status = execute(args, front);
		result = *exit_status;
	}

	hist++;

	while (args[i])
	{
		free(args[i]);
		i++;
	}

	return (result);
}

/**
 * handle_args - Gets, calls, and runs the execution of a command.
 * @exit_status: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int process_args(int *exit_status)
{
	int result = 0, idx = 0;
	char **arguments, *line = NULL, **cmd_front;

	line = find_args(line, exit_status);
	if (!line)
		return END_OF_FILE;

	arguments = _strtok(line, " ");
	free(line);
	if (!arguments)
		return result;

	if (check_args(arguments) != 0)
	{
		*exit_status = 2;
		free_args(arguments, arguments);
		return *exit_status;
	}
	cmd_front = arguments;

	while (arguments[idx])
	{
		if (_strncmp(arguments[idx], ";", 1) == 0)
		{
			free(arguments[idx]);
			arguments[idx] = NULL;
			result = prepare_args(arguments, cmd_front, exit_status);
			arguments = &arguments[++idx];
			idx = 0;
		}
		else
		{
			idx++;
		}
	}

	if (arguments)
		result = prepare_args(arguments, cmd_front, exit_status);

	free(cmd_front);
	return (result);
}



/**
 * check_args - Checks if there are any leading ';', ';;', '&&', or '||'.
 * @arguments: 2D pointer to tokenized commands and arguments.
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 *	   Otherwise - 0.
 */
int check_args(char **arguments)
{
	size_t idx = 0;
	char *current, *next;

	while (arguments[idx])
	{
		current = arguments[idx];
		if (current[0] == ';' || current[0] == '&' || current[0] == '|')
		{
			if (idx == 0 || current[1] == ';')
				return (create_error(&arguments[idx], 2));
			next = arguments[idx + 1];
			if (next && (next[0] == ';' || next[0] == '&' || next[0] == '|'))
				return (create_error(&arguments[idx + 1], 2));
		}
		idx++;
	}

	return (0);
}

