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
 * pint - prints the value at the top of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty file
 *
 * Description: Prints the value at the top of the stack
 */
void pint(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL)
	{
		fprintf(stderr, "L%d: can't pint, stack empty\n", line_number);
		exit(EXIT_FAILURE);
	}

	printf("%d\n", (*stack)->n);
}

/**
 * swap - swaps the top two elements of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Swaps the top two elements of the stack. If the stack contains
 * less than two elements, prints an error message and exits with EXIT_FAILURE.
 */
void swap(stack_t **stack, unsigned int line_number)
{
	stack_t *first, *second;

	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%d: can't swap, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	first = *stack;
	second = first->next;

	first->next = second->next;
	second->prev = NULL;
	first->prev = second;
	second->next = first;

	*stack = second;
}

/**
 * pop - removes the top element of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Removes the top element of the stack. If the stack is empty,
 * prints an error message and exits with EXIT_FAILURE.
 */
void pop(stack_t **stack, unsigned int line_number)
{
	stack_t *temp = *stack;

	if (*stack == NULL)
	{
		fprintf(stderr, "L%d: can't pop an empty stack\n", line_number);
		exit(EXIT_FAILURE);
	}

	*stack = temp->next;

	if (*stack != NULL)
		(*stack)->prev = NULL;

	free(temp);
}

/**
 * add - adds the top two elements of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Adds the top two elements of the stack. If the stack contains
 * less than two elements, prints an error message and exits with EXIT_FAILURE.
 * The result is stored in the second top element of the stack, and the top
 * element is removed.
 */
void add(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%d: can't add, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	(*stack)->next->n += (*stack)->n;

	pop(stack, line_number);
}

/**
 * nop - does nothing
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Does nothing. This opcode is used to represent a no-operation.
 */
void nop(stack_t **stack, unsigned int line_number)
{
	(void)stack;
	(void)line_number;
}

/**
 * sub - subtracts the top element from the second top element of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Subtracts the top element from the second top element of the stack.
 * If the stack contains less than two elements, prints an error message and exits
 * with EXIT_FAILURE. The result is stored in the second top element of the stack,
 * and the top element is removed.
 */
void sub(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%d: can't sub, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	(*stack)->next->n -= (*stack)->n;

	pop(stack, line_number);
}

/**
 * div_op - divides the second top element
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Divides the second top
 */
void div_op(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%d: can't div, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	if ((*stack)->n == 0)
	{
		fprintf(stderr, "L%d: division by zero\n", line_number);
		exit(EXIT_FAILURE);
	}

	(*stack)->next->n /= (*stack)->n;

	pop(stack, line_number);
}

/**
 * mod_op - computes the rest of the division of the second top element
 *          by the top element of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty bytecode file
 *
 * Description: Computes the rest of the division of the second top element
 */
void mod_op(stack_t **stack, unsigned int line_number)
{
	if (*stack == NULL || (*stack)->next == NULL)
	{
		fprintf(stderr, "L%d: can't mod, stack too short\n", line_number);
		exit(EXIT_FAILURE);
	}

	if ((*stack)->n == 0)
	{
		fprintf(stderr, "L%d: division by zero\n", line_number);
		exit(EXIT_FAILURE);
	}

	(*stack)->next->n %= (*stack)->n;

	pop(stack, line_number);
}

/**
 * is_comment - checks if a line is a comment
 * @line: The line to check
 * Return: 1 if the line is a comment, 0 otherwise
 */
int is_comment(const char *line)
{
	while (*line)
	{
		if (*line == '#')
			return 1;
		else if (*line != ' ' && *line != '\t')
			return 0;
		line++;
	}
	return (0);
}

/**
 * pchar - prints the char at the top of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty file
 *
 * Description: Prints the ASCII character at the top of the stack.
 * If the stack is empty or the value is out of range, prints an error message.
 */
void pchar(stack_t **stack, unsigned int line_number)
{
int value;
stack_t *temp = *stack;

if (*stack == NULL)
{
fprintf(stderr, "L%d: can't pchar, stack empty\n", line_number);
exit(EXIT_FAILURE);
}


value = temp->n;

if (value < 0 || value > 127)
{
fprintf(stderr, "L%d: can't pchar, value out of range\n", line_number);
exit(EXIT_FAILURE);
}

putchar(value);
putchar('\n');

*stack = temp->next;
if (*stack)
(*stack)->prev = NULL;
free(temp);
}

#include <stdio.h>

/**
 * pstr - prints the string starting at the top of the stack
 * @stack: A pointer to the head of the stack
 * @line_number: The line number in the Monty file
 *
 * Description: Prints the string starting at the top of the stack.
 */
void pstr(stack_t **stack, unsigned int line_number)
{
stack_t *current = *stack;

(void)line_number;

while (current != NULL && current->n != 0 && (current->n >= 0 && current->n <= 127))
{
_putchar(current->n);
current = current->next;
}

_putchar('\n');
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
		char *rest_of_line = line + strlen(opcode);

		if (line[0] == '\n' || (line[0] == ' ' && line[1] == '\n'))
			continue;

		line_number++;

		if (is_comment(line))
			continue;

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
			else if (strcmp(opcode, "pint") == 0)
			{
				pint(&stack, line_number);
			}
			else if (strcmp(opcode, "pop") == 0)
			{
				pop(&stack, line_number);
			}
			else if (strcmp(opcode, "swap") == 0)
			{
				swap(&stack, line_number);
			}
			else if (strcmp(opcode, "add") == 0)
			{
				add(&stack, line_number);
			}
			else if (strcmp(opcode, "nop") == 0)
			{
				nop(&stack, line_number);
			}
			else if (strcmp(opcode, "sub") == 0)
			{
				sub(&stack, line_number);
			}
			else if (strcmp(opcode, "div") == 0)
			{
				div_op(&stack, line_number);
			}
			else if (strcmp(opcode, "mod") == 0)
			{
				mod_op(&stack, line_number);
			}
			else if (strcmp(opcode, "pchar") == 0)
			{
				pchar(&stack, line_number);
			}
			else if (strcmp(opcode, "pstr") == 0)
			{
				pstr(&stack, line_number);
			}
			else
			{
				fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
				exit(EXIT_FAILURE);
			}
			if (*rest_of_line != '\0' && *rest_of_line != '\n')
				fprintf(stderr, "L%d: ignoring text: %s\n", line_number, rest_of_line);

		}
	}

	fclose(file);

	return (0);
}
