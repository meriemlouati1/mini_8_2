# include "minishell.h"

int ft_is_number(char *str)
{
    int i;

    i = 0;
    if(str[i] == '+' || str[i] == '-')
        i++;
    if(str[i] == '\0')
        return (0);
    while(str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return(0);
        i++;
    }
    return(1);
}

void ft_exit(char **argument)
{
    if(!argument[1])
    {
        set_exit_status(0);
        exit(0);
    }
    if (argument[1] && !ft_is_number(argument[1]))
    {
        printf("bash : exit: %s: numeric argument required\n",argument[1]);
        set_exit_status(2);
        exit(2);
    }
    int error = 0;
    long long num = ft_atoi(argument[1], &error);
    if (error)
    {
        printf("bash : exit: %s: numeric argument required\n",argument[1]);
        set_exit_status(2);
        exit(2);
    }
    if(argument[2])
    {
        printf("bash: exit: too many arguments\n");
        set_exit_status(1);
        return;
    }
    set_exit_status(num % 256);
    exit(num % 256);
}
