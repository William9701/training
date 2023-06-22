#include "monty.h"

/**
 * push - Pushes an integer onto the stack.
 * @stack: Pointer to the stack.
 * @line_number: Line number where the push function is called.
 */
void push(stack_t **stack, unsigned int line_number)
{
	if (command->n_tokens <= 1 || !(is_number(command->tokens[1])))
	{
		free_arguments();
		dprintf(2, "L%d: usage: push integer\n", line_number);
		exit(EXIT_FAILURE);
	}

	*stack = malloc(sizeof(stack_t));
	if (*stack == NULL)
		malloc_failed();
	(*stack)->next = (*stack)->prev = NULL;
	(*stack)->n = (int) atoi(command->tokens[1]);
	if (command->head == NULL)
		command->head = *stack;
	else
	{
		if (command->stack)
		{
			(*stack)->next = command->head;
			command->head->prev = *stack;
			command->head = *stack;
		}
		else
		{
			stack_t *tmp = command->head;

			while (tmp->next)
				tmp = tmp->next;
			tmp->next = *stack;
			(*stack)->prev = tmp;
		}
	}
	command->stack_length += 1;
}

/**
 * add - Adds the top two elements of the stack.
 * @stack: Pointer to the stack.
 * @line_number: Line number where the add function is called.
 */
void add(stack_t **stack, unsigned int line_number)
{
	stack_t *tmp1, *tmp2;

	(void) stack;
	if (command->stack_length < 2)
	{
		dprintf(2, "L%d: can't add, stack too short\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}
	tmp1 = command->head;
	tmp2 = tmp1->next;

	tmp2->n = tmp1->n + tmp2->n;
	delete_stack_node();

	command->stack_length -= 1;
}
/**
* is_comment - checks the tokens if it starts with #"
* Return: 0 or 1
*/
int is_comment(void)
{
	if (
			command->tokens &&
			command->tokens[0] &&
			command->tokens[0][0] == '#'
			)
		return (1);

	return (0);
}

/**
* delete_stack_node - delete node at the head/top
* Return: void
*/
void delete_stack_node(void)
{
	stack_t *tmp;

	tmp = command->head;
	command->head = tmp->next;
	free(tmp);
}

/**
 * _div - Divides the second top element of the stack by the top element.
 * @stack: Pointer to the stack.
 * @line_number: Line number where the _div function is called.
 */

void _div(stack_t **stack, unsigned int line_number)
{
	stack_t *tmp1, *tmp2;

	(void) stack;
	if (command->stack_length < 2)
	{
		dprintf(2, "L%d: can't div, stack too short\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}

	tmp1 = command->head;
	tmp2 = tmp1->next;

	if (tmp1->n == 0)
	{
		dprintf(2, "L%d: division by zero\n", line_number);
		free_args();
		exit(EXIT_FAILURE);
	}

	tmp2->n = tmp2->n / tmp1->n;
	delete_stack_node();

	command->stack_length -= 1;
}
