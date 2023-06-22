#include "monty.h"

/**
 * mul - Multiplies the top two elements of the stack.
 * @stack: Pointer to the stack.
 * @line_number: Line number where the mul function is called.
 */

void mul(stack_t **stack, unsigned int line_number)
{
	stack_t *tmp1, *tmp2;

	(void) stack;
	if (command->stack_length < 2)
	{
		dprintf(2, "L%d: can't mul, stack too short\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}

	tmp1 = command->head;
	tmp2 = tmp1->next;

	tmp2->n = tmp2->n * tmp1->n;
	delete_stack_node();

	command->stack_length -= 1;
}

/**
 * nop - does nothing
 * @stack: Pointer to the stack.
 * @line_number: Line number where the nop function is called.
 */
void nop(stack_t **stack, unsigned int line_number)
{
	(void) stack;
	(void) line_number;
}

/**
 * pall - Prints all the elements in the stack.
 * @stack: Pointer to the stack.
 * @line_number: Line number where the pall function is called.
 */
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *temp;

	if (command->head == NULL)
		return;

	(void) line_number;
	(void) stack;

	temp = command->head;
	while (temp != NULL)
	{
		printf("%d\n", temp->n);
		temp = temp->next;
	}
}

/**
 * pchar - prints the char at the top of the stack,
 * followed by a new line
 * @stack: Pointer to the stack.
 * @line_number: Line number where the mul function is called.
 */
void pchar(stack_t **stack, unsigned int line_number)
{
	stack_t *temp;

	(void) stack;
	if (command->head == NULL)
	{
		dprintf(2, "L%d: can't pchar, stack empty\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}
	temp = command->head;
	if (temp->n < 0 || temp->n > 127)
	{
		dprintf(2, "L%d: can't pchar, value out of range\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}
	printf("%c\n", temp->n);
}

/**
 * pint - Prints the value at the top of the stack.
 * @stack: Pointer to the stack.
 * @line_number: Line number where the pint function is called.
 */
void pint(stack_t **stack, unsigned int line_number)
{
	(void) stack;

	if (command->head == NULL)
	{
		dprintf(2, "L%d: can't pint, stack empty\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}
	printf("%d\n", command->head->n);
}
