/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:42:23 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/07 16:15:42 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void *signal_state()
{
    static int sig_state = 0;

    return &sig_state;
}

void *sig_handler()
{
    static t_sig stats;

    return &stats;
}


void handle_ctrlc_child()
{
    //handle in here_doc
    t_sig *stats = sig_handler();
    int *sig_state = signal_state();
    if (stats->reading_from_here_doc)
    {
        
        rl_on_new_line();
        rl_replace_line("", 0);
        stats->reading_from_here_doc = 0;
        exit(1);
    }
}

void handle_ctrlc()
{
    t_sig *stats = sig_handler();
    int *sig_state = signal_state();
    write(1, "\n", 1);
    if (stats->executing == 0)
    {
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        set_exit_status(130);
    }
}

void handle_sigquit()
{
    t_sig *stats = sig_handler();
    if (stats->executing)
    {
        write(2, "Quit (core dumped)\n", 19);
        set_exit_status(131);
    }
    else
        signal(SIGQUIT, SIG_IGN);
}

void setup_signals()
{
    struct sigaction sa;

    sa.sa_handler = handle_sigquit;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGQUIT, &sa, 0);
}

int main(int ac, char **av, char **envp)
{
    if (ac != 1)
        return 1;
    t_exec executor;
    executor.path = NULL;
    executor.envp = envp;
    t_env *my_env = NULL;
    signal(SIGINT, handle_ctrlc);
    init_env(&my_env, envp);
    executor.env = my_env;
    executor.pids = NULL;
    executor.last_pwd = NULL;
    signal(SIGQUIT, SIG_IGN);
    t_sig *sig_state = sig_handler();
    while(1)
    {
        sig_state->executing = 0;
        char *s = readline("minishell > ");
        if (!s)
        {
            printf("exit\n");
            break ;
        }
        add_history(s);
        if (simple_parsing(s, &executor) == 0)
            exceute_cmds(&executor);
        free_all_in_gc();
        free(s);
    }
    return (0);
}
