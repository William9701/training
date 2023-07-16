#include "shell.h"

/**
 * handle_line - Partitions a line read from standard input as needed.
 * @line: A pointer to a line read from standard input.
 * @read: The length of line.
 * Description: Spaces are inserted to separate ";", "||", and "&&".
 *              Replaces "#" with '\0'.
 */
void process_line(char **line, ssize_t read)
{
	char *old_str, *new_str;
	char prev, cur, next;
	size_t old_index = 0, new_index = 0;
	ssize_t new_length;

	new_length = get_new_len(*line);
	if (new_length == read - 1)
		return;
	new_str = malloc(new_length + 1);
	if (!new_str)
		return;

	old_str = *line;
	while (old_str[old_index])
	{
		cur = old_str[old_index];
		next = old_str[old_index + 1];
		if (old_index != 0)
		{
			prev = old_str[old_index - 1];
			if (cur == ';')
			{
				if (next == ';' && prev != ' ' && prev != ';')
				{
					new_str[new_index++] = ' ';
					new_str[new_index++] = ';';
					old_index++;
					continue;
				}
				else if (prev == ';' && next != ' ')
				{
					new_str[new_index++] = ';';
					new_str[new_index++] = ' ';
					old_index++;
					continue;
				}
				if (prev != ' ')
					new_str[new_index++] = ' ';
				new_str[new_index++] = ';';
				if (next != ' ')
					new_str[new_index++] = ' ';
				old_index++;
				continue;
			}
			else if (cur == '&')
			{
				if (next == '&' && prev != ' ')
				{
					new_str[new_index++] = ' ';
				}
				else if (prev == '&' && next != ' ')
				{
					new_str[new_index++] = '&';
					new_str[new_index++] = ' ';
					old_index++;
					continue;
				}
			}
			else if (cur == '|')
			{
				if (next == '|' && prev != ' ')
				{
					new_str[new_index++] = ' ';
				}
				else if (prev == '|' && next != ' ')
				{
					new_str[new_index++] = '|';
					new_str[new_index++] = ' ';
					old_index++;
					continue;
				}
			}
		}
		else if (cur == ';')
		{
			if (old_index != 0 && old_str[old_index - 1] != ' ')
				new_str[new_index++] = ' ';
			new_str[new_index++] = ';';
			if (next != ' ' && next != ';')
				new_str[new_index++] = ' ';
			old_index++;
			continue;
		}
		new_str[new_index++] = old_str[old_index];
		old_index++;
	}
	new_str[new_index] = '\0';

	free(*line);
	*line = new_str;
}


/**
 * get_new_len - Gets the new length of a line partitioned
 *               by ";", "||", "&&&", or "#".
 * @line: The line to check.
 *
 * Return: The new length of the line.
 *
 * Description: Cuts short lines containing '#' comments with '\0'.
 */

ssize_t get_new_len(char *line)
{
	size_t old_index = 0;
	ssize_t new_length = 0;
	char cur, next;

	while (line[old_index])
	{
		cur = line[old_index];
		next = line[old_index + 1];
		if (cur == '#')
		{
			if (old_index == 0 || line[old_index - 1] == ' ')
			{
				line[old_index] = '\0';
				break;
			}
		}
		else if (old_index != 0)
		{
			if (cur == ';')
			{
				if (next == ';' && line[old_index - 1] != ' ' && line[old_index - 1] != ';')
				{
					new_length += 2;
					old_index++;
					continue;
				}
				else if (line[old_index - 1] == ';' && next != ' ')
				{
					new_length += 2;
					old_index++;
					continue;
				}
				if (line[old_index - 1] != ' ')
					new_length++;
				if (next != ' ')
					new_length++;
			}
			else
				logical_ops(&line[old_index], &new_length);
		}
		else if (cur == ';')
		{
			if (old_index != 0 && line[old_index - 1] != ' ')
				new_length++;
			if (next != ' ' && next != ';')
				new_length++;
		}
		new_length++;
		old_index++;
	}
	return (new_length);
}

/**
 * logical_ops - Checks a line for logical operators "||" or "&&".
 * @line: A pointer to the character to check in the line.
 * @new_len: Pointer to new_len in get_new_len function.
 */
void logical_ops(char *line, ssize_t *new_len)
{
	char prev, curr, next;

	prev = *(line - 1);
	curr = *line;
	next = *(line + 1);

	if (curr == '&')
	{
		if (next == '&' && prev != ' ')
			(*new_len)++;
		else if (prev == '&' && next != ' ')
			(*new_len)++;
	}
	else if (curr == '|')
	{
		if (next == '|' && prev != ' ')
			(*new_len)++;
		else if (prev == '|' && next != ' ')
			(*new_len)++;
	}
}
