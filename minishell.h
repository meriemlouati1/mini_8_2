/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 14:41:40 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 14:51:38 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdint.h>
# include <stdbool.h>
#include <signal.h>
#include <limits.h>

//gc utils
typedef struct gc
{
    void *p;
    struct gc *next;
}   t_gc;


typedef struct s_env
{
    char    *var_name;
    char    *var_value;
    int     has_eq_ind;
    struct s_env    *next;
}   t_env;

typedef struct sig_hand
{
    int reading_from_here_doc;
    volatile sig_atomic_t stop_reading;
    int executing;
    pid_t pid;
}   t_sig;

// ls -la | wc -l > output_file

// ls -la -> wc -l -> output_file


// prev = 3 type = 33 next = -1
// prev = 1 type = 0 next = 3
// prev = -1 type = 0 next = 1

typedef struct list
{
    int type;//0 if word | 1 if pipe | 2 if red_inp | 3 if red_out_trunc | 4 if red_out_append | 5 if here_doc
    //55 type is the here_doc limiter
    //44 tyoe is the outfile name in append_mode
    //33 tyoe is the outfile name in trunc_mode
    //22 tyoe is the input_file name
    int fd_in;
    int fd_out;
    char **command;
    char *path;
    struct list *next;
}   t_list;


typedef struct here_doc
{
    char *limiter;
    int fd;
    struct here_doc *next;
}   t_here_doc;

typedef struct execution
{
    pid_t pid;
    int pipefd[2];
    int next_type;
    int cur_type;
    int prev_type;
    int out_fd;
    int in_fd;
}   t_execution;

typedef struct pipes
{
    int pipefd[2];
    int prev_fd;
    int prev_fd2;
}   t_pipes;

typedef struct s_pids
{
    pid_t pid;
    struct s_pids *next;
}   t_pids;

typedef struct mini_exec
{
    int exit_status;
    int here_doc_fd;
    char **path;
    char **tokens;
    char ***commands;
    char **special_chars;
    int cmd_size;
    t_list *commands_list;
    t_env *env;
    t_pids *pids;
    char *command_path;
    char **full_command;
    char **envp;
    char *last_pwd;
    char *pwd;
    char *old_pwd;
    t_here_doc *here_docs;
    t_execution *exec;
}   t_exec;

//parsing
void free_all_in_gc();
void *get_head();
void add_to_gc(void *addr);
void *gc_malloc(size_t bytes);

//exit_status
void *exit_status();
void set_exit_status(int status);
void *sig_handler();

void handle_ctrlc();
void handle_ctrlc_child();
void handle_sigquit();

char	*find_path(char **envp);
int simple_parsing(char *s, t_exec *executor);
void execute_command(char *s, t_exec *executor, t_env *envp);
char	*get_command(char *cmd, char **paths);
//syntax
int syntax_errors(char *s);

//signals
void *signal_state();

// envir functions

int equal_flag(char *str);
void print_env(t_env *my_env);
void init_env(t_env **my_env, char **env);
void add_back_env(t_env **my_env, t_env *new);
t_env *new_env(char *str);
int env_size(t_env *env);
int equal_signe(char *str);
char *get_env_value(t_env *my_env, char *var);

// builtins functions

int    ft_export (char **argument, t_env **my_env);
int    ft_env(char  **argument, t_env **env);
int     ft_echo(char **argument);
void    ft_exit(char **argument);
int     ft_pwd(t_exec *executor);
int ft_cd(char **argument, t_env **env, t_exec *executor);
int ft_unset(char **argument, t_env **my_env);

// export special_case

int is_special_case(char *str);
void export_special_case(t_env **my_env, char *str);
void change_var_value_special(t_env **my_env, char *str_name, char *str_value);
int plus_signe_index(char *str);
t_env *new_env_special(char *str);



//execution
// void exceute_cmds(t_exec *executor, t_env **env);
void exceute_cmds(t_exec *executor);
bool var_exist(t_env *my_env, char *str);
char **convert_env(t_env *env);
void sort_array(char **env_array);
void change_var_value(t_env **my_env, char *str_name, char *str_value);
char **env_for_execv(t_env *env);
int handle_redirections(t_list *cmd, t_pipes *pipes);

//utils

int     ft_isalpha(int c);
int	    ft_isdigit(int c);
int     ft_isalnum(int c);
char	**ft_split(char const *s, char c);
char	*ft_strjoin1(char *s1, char *s2);
char	*ft_strdup(const char *s);
size_t	ft_strlen(char *s);
void	*ft_calloc(size_t count, size_t size);
int ft_strcmp(char *s1, char *s2);
char	*ft_itoa(int n);
// int is_special_char(char *s);
char	*ft_strtrim(char const *s1, char const *set);
int is_special(char c);
char	*ft_substr(char *s, int start, int len);
long long ft_atoi(char *str, int *error);


// pids 

t_pids *new_pid(pid_t pid_value);
void add_back_pid(t_pids **pids, t_pids *new);
int size_pids(t_pids *pids);


#endif