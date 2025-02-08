/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 17:14:13 by aakhrif           #+#    #+#             */
/*   Updated: 2025/01/11 18:28:56 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void *get_head()
// {
//     static t_gc *head = NULL;
//     return (&head);
// }

static t_gc *head = NULL;

void add_to_gc(void *addr)
{
    // t_gc *t_head = get_head();
    t_gc *mem_node = malloc(sizeof(t_gc));
    if (!mem_node)
        exit(11);
    mem_node->p = addr;
    mem_node->next = head;
    head = mem_node;
}

void *gc_malloc(size_t bytes)
{
    void *p = malloc(bytes);
    if (!p)
        exit(11);
    add_to_gc(p);
    return p;
}

// void gc_free(void *ptr) {
//     t_gc **current = &head;
//     while (*current) {
//         if ((*current)->p == ptr) {
//             t_gc *temp = *current;
//             *current = (*current)->next;
//             free(ptr);         // Free the allocated memory
//             free(temp);        // Free the node
//             return;
//         }
//         current = &((*current)->next);
//     }
//     fprintf(stderr, "Pointer not found in garbage collector\n");
// }

void free_all_in_gc()
{
    // t_gc *t_head = get_head();

    t_gc *current = head;
    while (current) {
        free(current->p);
        t_gc *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
}