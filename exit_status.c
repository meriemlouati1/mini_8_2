/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:48:12 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/07 15:23:48 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *exit_status()
{
    static int ext_status;

    return (&ext_status);
}

void set_exit_status(int status)
{
    int *e = exit_status();

    *e = status;
}
