#include "minishell.h"

bool built_in_cmd(t_list *cmd)
{
    if(!ft_strcmp(cmd->command[0], "echo"))
            return (true);
    else if(!ft_strcmp(cmd->command[0], "pwd"))
            return (true);
    else if(!ft_strcmp(cmd->command[0], "cd"))
            return (true);
    else if(!ft_strcmp(cmd->command[0], "export"))
            return (true);
    else if(!ft_strcmp(cmd->command[0], "unset"))
            return (true);
    else if(!ft_strcmp(cmd->command[0], "env"))
            return (true);
    else if(!ft_strcmp(cmd->command[0], "exit"))
            return (true);
    else
        return(false);
}

void exec_builtin(t_list *cmd, t_env **env, t_exec *executor)
{    
    if(!ft_strcmp(cmd->command[0], "echo"))
            set_exit_status(ft_echo(cmd->command));
    else if(!ft_strcmp(cmd->command[0], "pwd"))
            set_exit_status(ft_pwd(executor));
    else if(!ft_strcmp(cmd->command[0], "cd"))
            set_exit_status(ft_cd(cmd->command,env, executor));
    else if(!ft_strcmp(cmd->command[0], "export"))
            set_exit_status(ft_export(cmd->command, env));
    else if(!ft_strcmp(cmd->command[0], "unset"))
            set_exit_status(ft_unset(cmd->command, env));
    else if(!ft_strcmp(cmd->command[0], "env"))
            set_exit_status(ft_env(cmd->command, env));
    else 
            ft_exit(cmd->command);
}

char **get_paths(t_env *my_env)
{
    char *str;
    str = get_env_value(my_env, "PATH");
    if(!str)
        return(NULL);
    return(ft_split(str, ':'));
}
char **env_for_execv(t_env *env)
{
    int     size;
    char    **array;
    t_env   *temp;
    int     i;

    size = env_size(env);
    array = malloc(sizeof(char *) *(size + 1));
    i = 0;
    temp = env;
    while(temp)
    {
        array[i] = ft_strjoin1(temp->var_name, "=");
        array[i] = ft_strjoin1(array[i], temp->var_value);
        temp = temp->next;
        i++;
    }
    array[i] = NULL;
    return(array);
}

void path_ready(t_list *cmd, t_env **my_env)
{
    pid_t pid;

    if(access(cmd->command[0], F_OK | X_OK) == 0)
    {
        char **env_exec = env_for_execv(*my_env);
        execve(cmd->command[0], cmd->command, env_exec);
    }
    else if (access(cmd->command[0], F_OK) == 0)
    {
        write(2, cmd->command[0], ft_strlen(cmd->command[0]));
        write(2, ": Permission denied", 19);
        write(2, "\n", 1);
        exit(126);
    }
    else
    {
        write(2, "Error : command not found: ", 27);
        write(2, cmd->command[0], ft_strlen(cmd->command[0]));
        write(2, "\n", 1);
        exit(127);
    }
}

int handle_redirections(t_list *cmd, t_pipes *pipes)
{
    t_list *redir = cmd->next;
    while (redir && (redir->type >= 2 && redir->type <= 5))
    {
        if (redir->type == 2)  // Input redirection
        {
            redir->next->fd_in = open(redir->next->command[0], O_RDONLY, 0644);
            if (redir->next->fd_in < 0)
            {
                perror("Error opening input file");
                return(-1);
            }
            if (dup2(redir->next->fd_in , STDIN_FILENO) == -1)
            {
                perror("dup2 failed for input redirection");
                close(redir->next->fd_in);
                return(-1);
            }
            close(redir->next->fd_in);
        }
        else if (redir->type == 3)  // Output redirection (>)
        {
            redir->next->fd_out = open(redir->next->command[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (redir->next->fd_out < 0)
            {
                perror("Error opening output file");
                return(-1);
            }
            if (dup2(redir->next->fd_out, STDOUT_FILENO) == -1)
            {
                perror("dup2 failed for output redirection");
                close(redir->next->fd_out);
                return(-1);
            }
            close(redir->next->fd_out);
        }
        else if (redir->type == 4)  // Append (>>)
        {
            redir->next->fd_out = open(redir->next->command[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (redir->next->fd_out < 0)
            {
                perror("Error opening append file");
                return(-1);
            }
            if (dup2(redir->next->fd_out, STDOUT_FILENO) == -1)
            {
                perror("dup2 failed for append redirection");
                close(redir->next->fd_out);
                return(-1);
            }
            close(redir->next->fd_out);
        }
        else if (redir->type == 5)  // Here-document (<<)
        {
            redir->next->fd_in = open("here_doc", O_RDONLY);
            if (dup2(redir->next->fd_in, STDIN_FILENO) == -1)
            {
                perror("dup2 failed for here-document redirection");
                return(-1);
            }
            close(redir->next->fd_in);
        }
        redir = redir->next->next;
    }
    return (0);
}

void exec_extern_cmd(t_list *cmd, t_env **my_env)
{
    char **paths;
    char *command;
    char *cmd_path;
    pid_t pid;
    int  i;
    int found;

    found = 0;
    command = cmd->command[0];
    if(command[0] == '.' || command[0] == '/')
        path_ready(cmd, my_env);
    paths = get_paths(*my_env);
    if(!paths)
    {
        write(2, "Error : PATH not there\n", 23);
        return;
    }
    cmd_path = NULL;
    i = 0;
    while(paths[i])
    {
        cmd_path = ft_strjoin1(paths[i], "/");
        cmd_path = ft_strjoin1(cmd_path, command);
        if(access(cmd_path, F_OK | X_OK) == 0)
        {
            found = 1;
            char **env_exec = env_for_execv(*my_env);
            execve(cmd_path, cmd->command, env_exec);
            free(cmd_path);
        }
        else if (access(cmd_path, F_OK) == 0)
        {
            write(2, command, ft_strlen(command));
            write(2, ": Permission denied", 19);
            write(2, "\n", 1);
            exit(126);
        }
        free(cmd_path);
        cmd_path = NULL;
        i++;

    }
    if(!found)
    {
        write(2, "Error : command not found: ", 27);
        write(2, command, ft_strlen(command));
        write(2, "\n", 1);
        exit(127);
    }
}

t_pids *new_pid(pid_t pid_value)
{
    t_pids *new;
    new = malloc(sizeof(t_pids));
    if (!new)
        return (NULL);
    new->pid = pid_value;
    new ->next = NULL;
    return (new);
}

void add_back_pid(t_pids **pids, t_pids *new)
{
    if (!new || ! pids )
        return ;
    if (!(*pids))
        (*pids) = new;
    else
    {
        t_pids *temp = (*pids);
        while(temp ->next)
        {
            temp = temp -> next;
        }
        temp->next = new;
    }
}
int size_pids(t_pids *pids)
{
    if(!pids)
        return (0);
    int size;
    t_pids * temp = pids;
    while(temp)
    {
        size++;
        temp = temp->next;
    }
    return (size);
}
void exceute_cmds(t_exec *executor)
{
    t_pipes pipes;
    t_list *cmd = executor->commands_list;
    t_list *tmp;
    pid_t pid;
    int has_pipe;
    pipes.prev_fd = -1;

    int saved_std_in = dup(STDIN_FILENO);
    int saved_std_out = dup(STDOUT_FILENO);
    set_exit_status(0);

    while (cmd)
    {
        if ( cmd -> type == 0)
        {
            has_pipe = 0;
            tmp = cmd;

            while (tmp)
            {
                if (tmp->type == 1) 
                {
                    has_pipe = 1;
                    break;
                }
                tmp = tmp->next;
            }
            if (has_pipe)
                pipe(pipes.pipefd);

            if (built_in_cmd(cmd))
            {
                if (has_pipe) 
                {
                    pid = fork();
                    if (pid == 0)
                    {
                        signal(SIGQUIT, handle_sigquit);
                        if (pipes.prev_fd != -1)
                        {
                            dup2(pipes.prev_fd, STDIN_FILENO);
                            close(pipes.prev_fd);
                        }
                        if (has_pipe)
                        {
                            dup2(pipes.pipefd[1], STDOUT_FILENO);
                            close(pipes.pipefd[1]);
                            close(pipes.pipefd[0]);
                        }

                        if (handle_redirections(cmd, &pipes) == -1)
                            exit(1);
                        exec_builtin(cmd, &executor->env, executor);
                        exit(EXIT_SUCCESS);
                    }
                    else if (pid > 0) 
                    {
                        t_pids *new = new_pid(pid);
                        add_back_pid(&executor->pids, new);
                        // waitpid(pid, NULL, 0);
                        if (pipes.prev_fd != -1)
                            close(pipes.prev_fd);
                        if (has_pipe)
                        {
                            close(pipes.pipefd[1]);
                            pipes.prev_fd = pipes.pipefd[0];
                        }
                    }
                    else
                        perror("fork failed");
                }
                else 
                {
                    if (handle_redirections(cmd, &pipes) == -1)
                        exit(1);
                    exec_builtin(cmd, &executor->env, executor);
                }
            }
            else
            {
                pid = fork();
                if (pid == 0)
                {
                    signal(SIGQUIT, handle_sigquit);
                    signal(SIGINT, handle_ctrlc);
                    if (pipes.prev_fd != -1)
                    {
                        dup2(pipes.prev_fd, STDIN_FILENO);
                        close(pipes.prev_fd);
                    }
                    if (has_pipe)
                    {
                        dup2(pipes.pipefd[1], STDOUT_FILENO);
                        close(pipes.pipefd[1]);
                        close(pipes.pipefd[0]);
                    }
                    if (handle_redirections(cmd, &pipes) == -1)
                        exit(1);
                    exec_extern_cmd(cmd, &executor->env);
                    exit(EXIT_FAILURE);
                }
                else if (pid > 0)
                {
                    t_pids *new = new_pid(pid);
                    add_back_pid(&executor->pids, new);
                    if (pipes.prev_fd != -1)
                        close(pipes.prev_fd);
                    if (has_pipe)
                    {
                        close(pipes.pipefd[1]);
                        pipes.prev_fd = pipes.pipefd[0];
                    }
                }
                else
                    perror("fork failed");
            }
        }
        cmd = cmd->next;
    }
    int i = 0;
    t_pids * temp = executor->pids;
    while(temp)
    {
        int status;
        waitpid(temp->pid, &status, 0);
        set_exit_status(WEXITSTATUS(status));
        temp = temp->next;
    }
    dup2(saved_std_in, STDIN_FILENO);
    dup2(saved_std_out, STDOUT_FILENO);
    close(saved_std_in);
    close(saved_std_out);
}
