#include "minishell.h"

bool is_valid_unset(char *str)
{
    int i;
    i = 0;
    if(!str)
        return(false);
    if(!str[0] || (str[0] != '_' &&!ft_isalpha(str[0])))
        return(false);
    while(str[i] && str[i] != '=')
    {
        if(!ft_isalnum(str[i]) && str[i] != '_')
            return(false);
        i++;
    }
    if(str[i] != '\0')
        return(false);
    return(true);
}

void unset_argument(char *str, t_env **my_env)
{
    if(!my_env || !*my_env)
        return;
    t_env *temp;
    t_env *prev;
    temp = *my_env;
    prev = NULL;
    while(temp)
    {
        if(!ft_strcmp(str, temp->var_name))
        {
            if(prev)
                prev->next = temp->next;
            else
                *my_env = temp->next;
            free(temp->var_name);
            free(temp->var_value);
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

int ft_unset(char **argument, t_env **my_env)
{
    int ac;

    ac = 0;
    while(argument[ac])
        ac++;
    if(ac == 1)
        return 0;
    int i;
    i =  1;
    while(argument[i])
    {
        if(is_valid_unset(argument[i]))
        {
            if (var_exist(*my_env, argument[i]))
                unset_argument(argument[i], my_env);
        }
        i++;
    }
    return 0;
}
