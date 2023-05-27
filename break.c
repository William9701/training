#include "shell.h"

/**
 * token_len - Locates the delimiter index marking the end
 *             of the first token contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 * Return: The delimiter index marking the end of
 *         the intitial token pointed to be str.
 */
int token_len(char *str, char *delim)
{
    int length = 0;

    while (*str != '\0' && *str != *delim)
    {
        length++;
        str++;
    }

    return (length);
}

/**
 * count_tokens - Counts the number of delimited
 *                words contained within a string.
 * @str: The string to be searched.
 * @delim: The delimiter character.
 *
 * Return: The number of words contained within str.
 */
int count_tokens(char *str, char *delim)
{
    int tokens = 0;

    while (*str != '\0')
    {
        if (*str != *delim)
        {
            tokens++;
            str += token_len(str, delim);
        }
        else
        {
            str++;
        }
    }

    return (tokens);
}

/**
 * _strtok - Tokenizes a string.
 * @line: The string.
 * @delim: The delimiter character to tokenize the string by.
 *
 * Return: A pointer to an array containing the tokenized words.
 */
char **_strtok(char *line, char *delim)
{
    char** ptr;
    int index = 0, tokens, t, letters, l;

    tokens = count_tokens(line, delim);
    if (tokens == 0)
        return NULL;

    ptr = (char**)malloc(sizeof(char*) * (tokens + 1));
    if (!ptr)
        return (NULL);

    for (t = 0; t < tokens; t++)
    {
        while (*line == *delim)
            line++;

        letters = token_len(line, delim);

        ptr[t] = (char*)malloc(sizeof(char) * (letters + 1));
        if (!ptr[t])
        {
            for (index = 0; index < t; index++)
                free(ptr[index]);
            free(ptr);
            return (NULL);
        }

        for (l = 0; l < letters; l++)
        {
            ptr[t][l] = line[l];
        }
        ptr[t][l] = '\0';

        line += letters;
    }
    ptr[t] = NULL;

    return (ptr);
}

