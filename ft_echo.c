#include "minishell.h"

int valid_opt(char *str)
{
    int i ;
    i = 0;

    if (!str)
        return(0);
    if(str[i] == '-' && str[i + 1] == 'n')
    {
        i++;
        while(str[i] && str[i] == 'n')
            i++;
        if(str[i] == '\0')
            return(1);
        return(0);
    }
    return (0);
}

int ft_echo(char **argument)
{
    int i ;
    int new_line;

    new_line = 1;
    i = 1;
    if(!argument[1])
    {
        write(1, "\n", 1);
        return(EXIT_SUCCESS);
    }
    if(valid_opt(argument[i]))
    {
        while(argument[i] && valid_opt(argument[i]))
            i++;
        new_line = 0;
        if (!argument[i])
                return(EXIT_SUCCESS);
    }
    while(argument[i])
    {
        write(1, argument[i], ft_strlen(argument[i]));
        if(argument[++i])
                write(1, " ", 1);
    }
    if(new_line)
        write(1, "\n", 1);
    return(EXIT_SUCCESS);
}
