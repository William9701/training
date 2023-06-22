#include "monty.h"

/**
 * mod - computes the rest of the division of the second top
 * element of the stack by the top element of the stack
 * @stack: Pointer to the stack.
 * @line_number: Line number where the mul function is called.
 */
void mod(stack_t **stack, unsigned int line_number)
{
	stack_t *temp1, *temp2;

	(void) stack;
	if (command->stack_length < 2)
	{
		dprintf(2, "L%d: can't mod, stack too short\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}

	temp1 = command->head;
	temp2 = temp1->next;

	if (temp1->n == 0)
	{
		dprintf(2, "L%d: division by zero\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}

	temp2->n = temp2->n % temp1->n;
	delete_stack_node();

	command->stack_length -= 1;
}


/**
 * pop - Removes the top element from the stack.
 * @stack: Pointer to the stack.
 * @line_number: Line number where the pop function is called.
 */
void pop(stack_t **stack, unsigned int line_number)
{
	(void) stack;
	if (command->head == NULL)
	{
		dprintf(2, "L%d: can't pop an empty stack\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}
	delete_stack_node();
	command->stack_length -= 1;
}

/**
 * pstr - prints the string starting at the top of the stack,
 * followed by a new line
 * @stack: Pointer to the stack.
 * @line_number: Line number where the mul function is called.
 */
void pstr(stack_t **stack, unsigned int line_number)
{
	stack_t *temp;

	(void) stack;
	(void) line_number;

	temp = command->head;
	while (temp != NULL)
	{
		if (temp->n <= 0 || temp->n > 127)
			break;
		printf("%c", temp->n);
		temp = temp->next;
	}
	printf("\n");
}

/**
 * queue - sets the format of the data to a queue (FIFO)
 * @stack: Pointer to the stack.
 * @line_number: Line number where the mul function is called.
 */
void queue(stack_t **stack, unsigned int line_number)
{
	(void) stack;
	(void) line_number;
	command->stack = 0;
}

/**
 * rotl - rotates the stack to the top
 * @stack: Pointer to the stack.
 * @line_number: Line number where the mul function is called.
 */
void rotl(stack_t **stack, unsigned int line_number)
{
	stack_t *temp1, *temp2;

	(void) stack;
	(void) line_number;
	if (command->stack_length < 2)
		return;

	temp1 = command->head;
	temp2 = temp1->next;
	command->head = temp2;

	while (temp2)
	{
		if (temp2->next == NULL)
		{
			temp2->next = temp1;
			temp1->next = NULL;
			temp1->prev = temp2;
			break;
		}
		temp2 = temp2->next;
	}
}
