/* linked_list.c: Definitions for linked lists */

/*
 *      Copyright (C) 2020 Jithin Renji
 *
 *      This program is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#ifdef linux
#  include <linux/limits.h>
#else
#  include <sys/syslimits.h>
#endif

#include "linked_list.h"

void node_init (struct node *n)
{
        n->str = malloc(PATH_MAX);
        memset(n->str, '\0', PATH_MAX);
        strcpy(n->str, "");
        n->next = NULL;
}

void list_append (struct node *head, const char *str)
{
        struct node *current = head;
        if (current->next == NULL && strcmp(current->str, "") == 0) {
                current->str = malloc(PATH_MAX);
                strcpy(current->str, str);
                current->next = NULL;
        } else {
                while (current->next != NULL) {
                        current = current->next;
                }

                current->next = malloc(sizeof(struct node));
                current->next->str = malloc(PATH_MAX);
                strcpy(current->next->str, str);
                current->next->next = NULL;
        }
}

void list_destroy (struct node *head)
{
        struct node *current = head;

        while (current != NULL) {
                free(current->str);
                current = current->next;
        }

        current = head;
        struct node *cnext = NULL;
        while (current != NULL) {
                cnext = current->next;
                free(current);
                current = cnext;
        }
}

/* Useful for debugging */
void print_list (struct node *head)
{
        struct node *current = head;
        while (current != NULL) {
                printf("%s\n", current->str);
                current = current->next;
        }
}
