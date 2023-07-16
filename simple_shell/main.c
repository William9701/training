#include "shell.h"

/**
 * sig_handler - Prints a new prompt upon a signal.
 * @sig: The signal.
 */
void sig_handler(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, sig_handler);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * execute - Executes a command in a child process.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * Return: If an error occurs - a corresponding error code.
 *         O/w - The exit value of the last executed command.
 */
int execute(char **args, char **front)
{
    pid_t child_pid;
    int status, is_external = 0, exit_status = 0;
    char *command = args[0];

    if (command[0] != '/' && command[0] != '.')
    {
        is_external = 1;
        command = get_path(command);
    }

    if (!command || (access(command, F_OK) == -1))
    {
        if (errno == EACCES)
            exit_status = create_error(args, 126);
        else
            exit_status = create_error(args, 127);
    }
    else
    {
        child_pid = fork();
        if (child_pid == -1)
        {
            if (is_external)
                free(command);
            perror("Error creating child process:");
            return 1;
        }
        if (child_pid == 0)
        {
            execve(command, args, environ);
            if (errno == EACCES)
                exit_status = create_error(args, 126);
            free_env();
            free_args(args, front);
            free_alias_list(aliases);
            _exit(exit_status);
        }
        else
        {
            wait(&status);
            exit_status = WEXITSTATUS(status);
        }
    }

    if (is_external)
        free(command);

    return exit_status;
}



/**
 * main - Runs a simple UNIX command interpreter.
 * @argc: The number of arguments supplied to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: The return value of the last executed command.
 */
int main(int argc, char *argv[])
{
    int ret_val = 0;
    int exe_ret_val;
    int *exe_ret = &exe_ret_val;

    name = argv[0];
    hist = 1;
    aliases = NULL;
    signal(SIGINT, sig_handler);

    *exe_ret = 0;
    environ = _copyenv();
    if (!environ)
        exit(-100);

    if (argc != 1)
    {
        ret_val = proccess_commands(argv[1], exe_ret);
        free_env();
        free_alias_list(aliases);
        return (*exe_ret);
    }

    if (!isatty(STDIN_FILENO))
    {
        while (1)
        {
            ret_val = process_args(exe_ret);
            if (ret_val == END_OF_FILE || ret_val == EXIT)
	    {
                write(STDOUT_FILENO, "\n", 1);
                break;
	    }
        }

        free_env();
        free_alias_list(aliases);
        return (*exe_ret);
    }

    while (1)
    {
        write(STDOUT_FILENO, "$ ", 2);
        ret_val = process_args(exe_ret);
        if (ret_val == END_OF_FILE || ret_val == EXIT)
        {
                  if (ret_val == END_OF_FILE)
                                write(STDOUT_FILENO, "\n", 1);
            free_env();
            free_alias_list(aliases);
            exit(*exe_ret);
        }
    }

    free_env();
    free_alias_list(aliases);
    return (*exe_ret);
}

