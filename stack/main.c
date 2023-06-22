#include "monty.h"

arg_t *command = NULL;
/**
 * main - Entry point
 * @argc: The number of command-line arguments.
 * @argv: A pointer to an array of strings containing
 * the command-line arguments.
 * Description: print alphabets in lowercase using the putchar
 *
 * Return: returns 0 (Success)
 */
int main(int argc, char **argv)
{
	size_t n = 0;

	if (argc != 2)
	{
		dprintf(2, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}
	initialize_arguments();
	get_stream(argv[1]);

	while (getline(&command->line, &n, command->stream) != -1)
	{
		command->line_number += 1;
		tokenize_line();
		get_command();
		run_command();
		free_tokens();
	}

	close_stream();
	free_arguments();

	return (0);
}
