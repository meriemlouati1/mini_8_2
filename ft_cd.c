# include "minishell.h"

int ft_cd(char **argument, t_env **my_env, t_exec *executor)
{
    int ac;
    char old_buff[4096];
    char new_buff[4096];
    char *old_pwd;
    char *new_pwd;

    ac = 0;
    old_pwd = getcwd(old_buff, 4096);
    while(argument[ac])
        ac++;
    if( ac > 2)
    {
        write(1, "bash : cd : too many arguments\n", 31);
        return 1;
    }
    else
    {
        if(ac == 1 || (ac == 2 && !ft_strcmp(argument[1], "~")))
        {
            if (chdir(get_env_value(*my_env,"HOME")) == -1)
            {
                perror(" failed to change to home directory");
                return 1;
            }
        }
        else
        {
            if (chdir(argument[1]) == -1)
            {
                perror(" failed to change to the directory");
                return 1;
            }
        }
    }
    new_pwd = getcwd(new_buff, 4096);
    if (new_pwd)
        executor->last_pwd = ft_strdup(new_pwd);
    if (!new_pwd)
        write(1, "failed to change to the directory\n", 35);
    if(old_pwd)
        change_var_value(my_env, "OLDPWD", old_pwd);
    if(new_pwd)
        change_var_value(my_env, "PWD", new_pwd);
    return 0;
}
