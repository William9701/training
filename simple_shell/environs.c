#include "shell.h"

/**
 * _copyenv - Creates a copy of the environment.
 *
 * Return: If an error occurs - NULL.
 *         O/w - a double pointer to the new copy.
 */
char **_copyenv(void)
{
	char **new_env;
	size_t env_size = 0;
	int env_index = 0;

	while (environ[env_size])
		env_size++;

	new_env = malloc(sizeof(char *) * (env_size + 1));
	if (!new_env)
		return (NULL);

	while (environ[env_index])
	{
		new_env[env_index] = malloc(_strlen(environ[env_index]) + 1);

		if (!new_env[env_index])
		{
			while (env_index-- >= 0)
				free(new_env[env_index]);
			free(new_env);
			return (NULL);
		}
		_strcpy(new_env[env_index], environ[env_index]);
		env_index++;
	}
	new_env[env_index] = NULL;

	return (new_env);
}


/**
 * free_env - Frees the the environment copy.
 */
void free_env(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * _getenv - Gets an environmental variable from the PATH.
 * @var: The name of the environmental variable to get.
 *
 * Return: If the environmental variable does not exist - NULL.
 *         Otherwise - a pointer to the environmental variable.
 */
char **_getenv(const char *var)
{
	int index, len;

	len = _strlen(var);
	for (index = 0; environ[index]; index++)
	{
		if (_strncmp(var, environ[index], len) == 0)
			return (&environ[index]);
	}

	return (NULL);
}
