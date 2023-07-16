#include "shell.h"

/**
 * shell_alias - Builtin command that either prints all aliases, specific
 * aliases, or sets an alias.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shell_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *current = aliases;
	int index, ret = 0;
	char *value;

	if (!args[0])
	{
		while (current)
		{
			print_alias(current);
			current = current->next;
		}
		return ret;
	}

	index = 0;
	while (args[index])
	{
		current = aliases;
		value = _strchr(args[index], '=');
		if (!value)
		{
			while (current)
			{
				if (_strcmp(args[index], current->name) == 0)
				{
					print_alias(current);
					break;
				}
				current = current->next;
			}
			if (!current)
				ret = create_error(args + index, 1);
		}
		else
			set_alias(args[index], value);

		index++;
	}

	return (ret);
}


/**
 * set_alias - Will either set an existing alias 'name' with a new value,
 * 'value' or creates a new alias with 'name' and 'value'.
 * @var_name: Name of the alias.
 * @value: Value of the alias. First character is a '='.
 */
void set_alias(char *var_name, char *value)
{
	alias_t *current = aliases;
	int value_len = 0, index = 0, new_index = 0;
	char *new_value;

	*value = '\0';
	value++;
	value_len = _strlen(value) - _strspn(value, "'\"");
	new_value = malloc(sizeof(char) * (value_len + 1));
	if (!new_value)
		return;

	while (value[index])
	{
		if (value[index] != '\'' && value[index] != '"')
			new_value[new_index++] = value[index];
		index++;
	}
	new_value[new_index] = '\0';

	while (current)
	{
		if (_strcmp(var_name, current->name) == 0)
		{
			free(current->value);
			current->value = new_value;
			break;
		}
		current = current->next;
	}

	if (!current)
		add_alias_end(&aliases, var_name, new_value);
}


/**
 * print_alias - Prints the alias in the format name='value'.
 * @alias: Pointer to an alias.
 */
void print_alias(alias_t *alias)
{
	char *alias_string;
	int len = _strlen(alias->name) + _strlen(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (len + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "='");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, len);
	free(alias_string);
}
/**
 * replace_aliases - Goes through the arguments and replace any matching alias
 * with their value.
 * @args: 2D pointer to the arguments.
 * Return: 2D pointer to the arguments.
 */
char **replace_aliases(char **args)
{
	alias_t *current;
	int index = 0;
	char *replaced_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);

	while (args[index])
	{
		current = aliases;
		while (current)
		{
			if (_strcmp(args[index], current->name) == 0)
			{
				replaced_value = malloc(sizeof(char) * (_strlen(current->value) + 1));
				if (!replaced_value)
				{
					free_args(args, args);
					return (NULL);
				}
				_strcpy(replaced_value, current->value);
				free(args[index]);
				args[index] = replaced_value;
				index--;
				break;
			}
			current = current->next;
		}
		index++;
	}

	return (args);
}

