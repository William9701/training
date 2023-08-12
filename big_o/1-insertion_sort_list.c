#include <stdio.h>
#include "sort.h"

/**
 * insertion_sort_list - Sorts a doubly linked list of integers in ascending order using Insertion Sort.
 * @list: Double pointer to the head of the list.
 */
void insertion_sort_list(listint_t **list)
{
	listint_t *sorted;
	listint_t *current;
	listint_t *temp;
    if (list == NULL || *list == NULL || (*list)->next == NULL)
        return;

    sorted = NULL;
    current = *list;

    while (current != NULL)
    {
        temp = current->next;

        while (sorted != NULL && sorted->n > current->n)
        {
            sorted = sorted->prev;
        }


        if (sorted == NULL)
        {
            current->prev = NULL;
            current->next = *list;
            (*list)->prev = current;
            *list = current;
        }
        else
        {
            current->prev = sorted;
            current->next = sorted->next;

            if (sorted->next != NULL)
                sorted->next->prev = current;

            sorted->next = current;
        }

        sorted = current;
        current = temp;
        print_list(*list);
    }
}
