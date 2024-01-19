#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "monty.h"

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
*/
int _putchar(char c)
{
	return (write(1, &c, 1));
}

/**
 * _puts - writes a string to stdout
 * @str: The string to print
 *
 * Return: void
*/
void _puts(char *str)
{
	size_t i;

	for (i = 0; str[i] != '\0'; i++)
	{
		_putchar(str[i]);
	}
}

/**
 * push - pushes an element onto the stack
 * @stack: A pointer to the head of the stack
 * @value_str: The string representation of the value to push
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Converts @value_str to an integer and pushes it onto the stack.
 */
void push(stack_t **stack, const char *value_str, unsigned int line_number)
{
	int value;
	char *endptr;
	stack_t *new_node = malloc(sizeof(stack_t));

	value = strtol(value_str, &endptr, 10);

	if (*endptr != '\0' && *endptr != '\n')
	{
		fprintf(stderr, "L%d: usage: push integer\n", line_number);
		exit(EXIT_FAILURE);
	}

	if (!new_node)
	{
		fprintf(stderr, "Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}

	new_node->n = value;
	new_node->prev = NULL;
	new_node->next = *stack;

	if (*stack)
	{
		(*stack)->prev = new_node;
	}

	*stack = new_node;
}

/**
 * pall - prints all values on the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty file
 *
 * Description: Prints all values on the stack
 */
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *current = *stack;

	(void)line_number;

	while (current != NULL)
	{
		printf("%d\n", current->n);
		current = current->next;
	}
}


/**
 * main - entry point of the Monty interpreter
 * @argc: The number of command-line arguments
 * @argv: An array of command-line argument strings
 *
 * Description: Reads Monty bytecode
 *
 * Return: Always true
 */
int main(int argc, char *argv[])
{
	FILE *file = fopen(argv[1], "r");
	stack_t *stack = NULL;
	char line[100];
	unsigned int line_number = 0;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (!file)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while (fgets(line, sizeof(line), file) != NULL)
	{
		char *opcode = strtok(line, " ");

		line_number++;

		line[strcspn(line, "\n")] = '\0';

		if (opcode != NULL)
		{
			if (strcmp(opcode, "push") == 0)
			{
				char *value_str = strtok(NULL, " ");

				if (value_str != NULL)
				{
					push(&stack, value_str, line_number);
				}
				else
				{
					fprintf(stderr, "L%d: usage: push integer\n", line_number);
					exit(EXIT_FAILURE);
				}
			}
			else if (strcmp(opcode, "pall") == 0)
			{
				pall(&stack, line_number);
			}
			else
			{
				fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
				exit(EXIT_FAILURE);
			}
		}
	}

	fclose(file);

	return (0);
}
