/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 18:26:32 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/05 23:17:55 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int equal_signe(char *str)
{
    int i;

    i = 0;
    while(str[i] && str[i] != '=')
        i++;
    return(i);
}

int equal_flag(char *str)
{
    if (!str)
        return (0);
    while (*str) 
    {
        if (*str == '=')
            return (1);
        str++;
    }
    return (0);
}

int env_size(t_env *env)
{
    t_env   *temp;
    int     size;

    if(!env)
        return(0);
    temp = env;
    size = 0;
    while(temp)
    {
        size++;
        temp = temp->next;
    }
    return(size);
}

t_env *new_env(char *str)
{
    t_env   *new;
    int indice;

    indice = equal_signe(str);
    new = (t_env *)malloc(sizeof(t_env));
    if(!new)
        return(NULL);
    new->var_name = ft_substr(str, 0, indice);
    if(!new->var_name)
        return(free(new), NULL);
    new->var_value = ft_substr(str, indice +1, ft_strlen(str) -indice -1);
    if(!new->var_value)
    {
        free(new->var_name);
        return(free(new), NULL);
    }
    new->has_eq_ind = equal_flag(str);
    new->next = NULL;
    return(new);
}

void add_back_env(t_env **my_env, t_env *new)
{
    if(!new || !my_env)
        return;
    if(!(*my_env))
        *(my_env) = new;
    else
    {
        t_env *temp;
        temp = *my_env;
        while(temp->next)
            temp = temp->next;
        temp->next = new;
    }
}

void init_env(t_env **my_env, char **env)
{
    t_env   *new;
    int     i;

    i = 0;
    while(env[i])
    {
        new = new_env(env[i]);
        if(new)
            add_back_env(my_env, new);
        i++;
    }
}

void print_env(t_env *my_env)
{
    t_env   *temp;
    
    temp = my_env;
    while(temp)
    {
        if(temp->has_eq_ind)
        {
            write(1, temp->var_name, ft_strlen(temp->var_name));
            write(1, "=", 1);
            write(1, temp->var_value, ft_strlen(temp->var_value));
            write(1, "\n", 1);
        }
        temp = temp->next;
    }
}

//find in env (traverse the list and find the key and return its value) to replace getenv();