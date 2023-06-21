#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monty.h"
void push(stack_t** stack, unsigned int line_number);
void pall(stack_t** stack, unsigned int line_number);

stack_t* stack = NULL;

void push(stack_t** stack, unsigned int line_number) 
{
    stack_t* new_node = malloc(sizeof(stack_t));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    new_node->n = line_number;
    new_node->prev = NULL;
    new_node->next = *stack;
    
    if (*stack != NULL) {
        (*stack)->prev = new_node;
    }
    
    *stack = new_node;
}

int main() 
{
	char opcode[50];
	unsigned int line_number;
    FILE* bytecode_file = fopen("bytecodes", "r");
    if (bytecode_file == NULL) {
        fprintf(stderr, "Failed to open bytecode file.\n");
        return 1;
    }


    
    while (fscanf(bytecode_file, "%s %u", opcode, &line_number) == 2) 
    {
        if (strcmp(opcode, "push") == 0) {
            push(&stack, line_number);
        }
	else
		break;
    }
    while (fscanf(bytecode_file, "%s %u", opcode, &line_number) == 2)
    {
	    if (strcmp(opcode, "pall") == 0)
	    {
		    push(&stack, line_number);
	    }
	    else
		    break;
    }

    fclose(bytecode_file);
    return 0;
}

void pall(stack_t** stack, unsigned int line_number)
{

    stack_t* current = *stack;

    (void)line_number;
    while (current != NULL) {
        printf("%d ", current->n);
        current = current->next;
    }
    printf("\n");

}
