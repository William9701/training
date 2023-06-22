#include "monty.h"

/**
 * tokenize_line - Tokenizes the line read from file.
 */

void tokenize_line(void)
{
	int i = 0;
	char *delims = " \n", *token = NULL, *linecpy = NULL;

	linecpy = malloc(sizeof(char) * (strlen(command->line) + 1));
	strcpy(linecpy, command->line);
	command->n_tokens = 0;
	token = strtok(linecpy, delims);
	while (token)
	{
		command->n_tokens += 1;
		token = strtok(NULL, delims);
	}

	command->tokens = malloc(sizeof(char *) *
			(command->n_tokens + 1));
	strcpy(linecpy, command->line);
	token = strtok(linecpy, delims);
	while (token)
	{
		command->tokens[i] = malloc(sizeof(char) *
				(strlen(token) + 1));
		if (command->tokens[i] == NULL)
			malloc_failed();
		strcpy(command->tokens[i], token);
		token = strtok(NULL, delims);
		i++;
	}
	command->tokens[i] = NULL;
	free(linecpy);
}

/**
 * initialize_arguments - Initializes a pointer to
 * arg_s structure.
 */
void initialize_arguments(void)
{
	command = malloc(sizeof(arg_t));
	if (command == NULL)
		malloc_failed();

	command->instruction = malloc(sizeof(instruction_t));
	if (command->instruction == NULL)
		malloc_failed();

	command->stream = NULL;
	command->head = NULL;
	command->line = NULL;
	command->n_tokens = 0;
	command->line_number = 0;
	command->stack_length = 0;
	command->stack = 1;
}
