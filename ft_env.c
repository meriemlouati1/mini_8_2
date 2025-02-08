#include "minishell.h"

int ft_env(char  **argument, t_env **env)
{
    int ac;
    ac = 0;
    while(argument[ac])
        ac++;
    if(ac == 1)
        print_env(*env);
    return 0;
}
