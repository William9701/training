#include "monty.h"

/**
 * close_stream - Closes the file stream and sets it to NULL.
 */

void close_stream(void)
{
	if (command->stream == NULL)
		return;

	fclose(command->stream);
	command->stream = NULL;
}

/**
 * free_args - Frees all allocated memory for arguments.
 */
void free_args(void)
{
	close_stream();
	free_tokens();
	free_arguments();
}


/**
 * free_head - Frees the memory allocated for the head and
 * sets it to NULL.
 */
void free_head(void)
{
	if (command->head)
		free_stack(command->head);
	command->head = NULL;
}

/**
* free_arguments - free memory allocated to arguments pointer
*/
void free_arguments(void)
{
	if (command == NULL)
		return;
	if (command->instruction)
	{
		free(command->instruction);
		command->instruction = NULL;
	}

	free_head();

	if (command->line)
	{
		free(command->line);
		command->line = NULL;
	}

	free(command);
}


/**
* free_stack - free the nodes in a stack list
* @head: first node of a dlistint link
* Return: void
*/
void free_stack(stack_t *head)
{
	if (head == NULL)
		return;

	if (head->next != NULL)
	{
		free_stack(head->next);
	}

	free(head);
}
