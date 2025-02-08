#include "minishell.h"

bool is_valid(char *str)
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
        {
            if (str[i] == '+' && (str[i + 1] && str[i+1] == '='))
                return (true);
            return (false);
        }
        i++;
    }
    return(true);
}

void ft_export_alone(t_env **my_env)
{
    char    **env_array;
    int     i;

    i = 0;
    env_array = convert_env(*my_env);
    sort_array(env_array);
    while(env_array[i])
    {
        write(1, "declare -x ", 11);
        write(1, env_array[i], ft_strlen(env_array[i]));
        write(1, "\n", 1);
        i++;
    }
}

void sort_array(char **env_array)
{
    char    *temp;
    int     i;
    int     j;

    i = 0;
    while(env_array[i])
    {
        j = i+1;
        while(env_array[j])
        {
            if(ft_strcmp(env_array[i], env_array[j]) > 0)
            {
                temp = env_array[i];
                env_array[i] = env_array[j];
                env_array[j] = temp;
            }
            j++;
        }
        i++;
    }
}
char **convert_env(t_env *env)
{
    int     size;
    int     i;
    char    **env_array;
    t_env   *temp;

    size = env_size(env);
    env_array = malloc(sizeof(char *) *(size + 1));
    if(!env_array)
        return(NULL);
    i = 0;
    temp = env;
    while(temp)
    {
        if(temp->has_eq_ind)
        {
            env_array[i] = ft_strjoin1(temp->var_name, "=");
            env_array[i] = ft_strjoin1(env_array[i], "\"");
            env_array[i] = ft_strjoin1(env_array[i], temp->var_value);
            env_array[i] = ft_strjoin1(env_array[i], "\"");
        }
        else
            env_array[i] = temp->var_name;
        temp = temp->next;
        i++;
    }
    env_array[i] = NULL;
    return(env_array);
}
bool var_exist(t_env *my_env, char *str)
{
    t_env *temp;

    temp = my_env;
    while(temp)
    {
        if(!ft_strcmp(temp->var_name, str))
            return(true);
        temp = temp->next;
    }
    return(false);
}

void change_var_value(t_env **my_env, char *str_name, char *str_value)
{
    t_env *temp;

    temp = *my_env;
    while(temp)
    {
        if(!ft_strcmp(temp->var_name, str_name))
        {
            temp->var_value = ft_strdup(str_value);
            break;
        }
        temp = temp->next;
    }
}
void export_to_env(t_env **my_env, char *var)
{
    int     indice;
    char    *str_name;
    char    *str_value;
    t_env   *new;

    indice = equal_signe(var);
    str_name = ft_substr(var, 0, indice);
    indice++;
    while(var[indice] && var[indice] == ' ')
        indice++;
    str_value = ft_substr(var, indice, ft_strlen(var) - indice);
    if(!str_name)
        return;
    if(var_exist(*my_env, str_name))
        change_var_value(my_env, str_name, str_value);
    else
    {
        new = new_env(var);
        if(new)
            add_back_env(my_env, new);
    }
}
int plus_signe_index(char *str)
{
    int i;
    i = 0;
    while(str[i] && str[i] != '+')
        i++;
    return (i);
}
void change_var_value_special(t_env **my_env, char *str_name, char *str_value)
{
    t_env *temp;

    temp = *my_env;
    while(temp)
    {
        if(!ft_strcmp(temp->var_name, str_name))
        {
            temp->var_value = ft_strjoin1(temp->var_value, str_value);
            temp->has_eq_ind = 1;
            break;
        }
        temp = temp->next;
    }
}
void export_special_case(t_env **my_env, char *str)
{
    int indice ;
    indice = plus_signe_index(str);
    char *str_name = ft_substr(str, 0, indice);
    indice = indice + 2;
    while(str[indice] && str[indice] == ' ')
        indice++;
    char *str_value = ft_substr(str, indice, ft_strlen(str) - indice);
    if (!str_name)
        return;
    if (var_exist(*my_env, str_name))
        change_var_value_special(my_env, str_name, str_value);
    else
    {
        t_env *new = new_env_special(str);
        if(new)
        add_back_env(my_env, new);
    }

}
int is_special_case(char *str)
{
    int i;
    i = 0;
    while(str[i] && str[i] != '+' &&str[i] != '=')
    {
        i++;
    }
    if (str[i] && str[i] == '+')
        return(1);
    return (0);
}

t_env *new_env_special(char *str)
{
    t_env *new;
    int indice;
    indice = plus_signe_index(str);
    new = (t_env *)malloc(sizeof(t_env));
    if (!new)
        return (NULL);
    new->var_name = ft_substr(str, 0, indice);
    if(!new->var_name)
        return(free(new), NULL);
    new->var_value = ft_substr(str, indice + 2, ft_strlen(str) - indice -2);
    if(!new->var_value)
    {
        free(new->var_name);
        return(free(new), NULL);
    }
    new->has_eq_ind = 1;
    new->next = NULL;
    return(new);
}

int ft_export (char **argument, t_env **my_env)
{
    int ac;
    int i;

    ac = 0;
    while(argument[ac])
        ac++;
    if(ac == 1)
        ft_export_alone(my_env);
    if(ac > 1)
    {
        i = 1;
        while(argument[i])
        {
            if(is_valid(argument[i]))
            {
                if (is_special_case(argument[i]))
                    export_special_case(my_env, argument[i]);
                else 
                    export_to_env(my_env, argument[i]);
            }
            else
            {
                printf("bash: export: '%s': not a valid identifier\n", argument[i]);
                return 1;
            }
            i++;
        }
    }
    return 0;
}
