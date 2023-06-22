#include "monty.h"

/**
 * free_tokens - Frees the allocated memory for tokens.
 */

void free_tokens(void)
{
	int i = 0;

	if (command->tokens == NULL)
		return;

	while (command->tokens[i])
	{
		free(command->tokens[i]);
		i++;
	}
	free(command->tokens);
	command->tokens = NULL;
}
