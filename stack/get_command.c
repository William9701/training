#include "monty.h"

/**
 * invalid_instruction - Handles an unknown instruction error.
 */

void invalid_instruction(void)
{
	dprintf(2, "L%d: unknown instruction %s\n",
			command->line_number, command->tokens[0]);
	free_args();
	exit(EXIT_FAILURE);
}

/**
 * get_command - Sets the instruction based on the first token
 * in the input line.
 */

void get_command(void)
{
	int i = 0;
	instruction_t instructions[] = {
		{"push", &push}, {"pop", &pop},
		{"pint", &pint}, {"swap", &swap},
		{"nop", &nop}, {"add", &add},
		{"pall", &pall}, {"sub", &sub},
		{"div", &_div}, {"mul", &mul},
		{"rotl", &rotl}, {"rotr", &rotr},
		{"stack", &stack}, {"queue", &queue},
		{"pstr", &pstr}, {"pchar", &pchar},
		{"mod", &mod},
		{NULL, NULL}
	};

	if (command->n_tokens == 0)
		return;
	if (command->tokens[0][0] == '#')
	{
		command->instruction->opcode = "nop";
		command->instruction->f = nop;
		return;
	}
	for (; instructions[i].opcode != NULL; i++)
	{
		/* compare opcode of instruction to first token (instruct..) */
		if (strcmp(instructions[i].opcode, command->tokens[0])
				== 0)
		{
			command->instruction->opcode = instructions[i].opcode;
			command->instruction->f = instructions[i].f;
			return;
		}
	}
	invalid_instruction();
}

/**
 * run_command - Runs the instruction specified by the arguments.
 */

void run_command(void)
{
	stack_t *stack = NULL;

	if (command->n_tokens == 0)
		return;

	command->instruction->f(&stack, command->line_number);
}
