/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:36:31 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/07 15:32:06 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command(char *cmd, char **paths)
{
	char	*tmp;
	char	*command;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin1(paths[i], "/");
		command = ft_strjoin1(tmp, cmd);
		free(tmp);
		if (access(command, F_OK) == 0)
			return (command);
		free(command);
		i++;
	}
	return (NULL);
}

int count_size(char **tokens)
{
	int i = 0;
	int count = 0;
	while(tokens[i])
	{
		count++;
        i++;
    }
	return (count);
}

int count_tokens(char *s)
{
    int i = 0;
    int tokens = 0;
    
    while (s[i])
    {
        while (s[i] && s[i] == ' ')
            i++;
        if (!s[i])
            break;
            
        tokens++;
        
        while (s[i])
        {
            if (s[i] == '"' || s[i] == '\'')
            {
                char quote = s[i];
                i++;
                while (s[i] && s[i] != quote)
                    i++;
                if (s[i])
                    i++;
            }
            else if (s[i] == ' ')
                break;
            else
                i++;
        }
    }
    return tokens;
}

char ***ft_split_tokens(char **tokens)
{
    char ***commands = gc_malloc(sizeof(char **) * (count_size(tokens) + 1));
    int i = 0;

    while (tokens[i])
    {
        int tokens_inside = count_tokens(tokens[i]);
        commands[i] = gc_malloc(sizeof(char *) * (tokens_inside + 1));
        int j = 0;
        int index = 0;
        
        while (tokens[i][index])
        {
            while (tokens[i][index] && tokens[i][index] == ' ')
                index++;
            if (!tokens[i][index])
                break;
            int start = index;
            int len = 0;
            int in_quotes = 0;
            char quote_type = 0;
            while (tokens[i][index])
            {
                if (!in_quotes && (tokens[i][index] == '"' || tokens[i][index] == '\''))
                {
                    quote_type = tokens[i][index];
                    in_quotes = 1;
                }
                else if (in_quotes && tokens[i][index] == quote_type)
                {
                    in_quotes = 0;
                    quote_type = 0;
                }
                else if (!in_quotes && tokens[i][index] == ' ')
                    break;
                len++;
                index++;
            }
            commands[i][j] = gc_malloc(sizeof(char) * (len + 1));
            int k = 0;
            in_quotes = 0;
            int pos = start;
            while (pos < start + len)
            {
                commands[i][j][k++] = tokens[i][pos];
                pos++;
            }
            commands[i][j][k] = '\0';
            j++;
        }
        commands[i][j] = NULL;
        i++;
    }
    commands[i] = NULL;
    return commands;
}

int check_type(char *s)
{
    if (ft_strcmp(s, "|") == 0)
        return 1;
    if (ft_strcmp(s, ">>") == 0)
        return 4;
    if (ft_strcmp(s, "<<") == 0)
        return 5;
    if (ft_strcmp(s, ">") == 0)
        return 3;
    if (ft_strcmp(s, "<") == 0)
        return 2;
    return (0);
}

t_list *parse_list(char ***commands)
{
    t_list *head = gc_malloc(sizeof(t_list));
    if (!head)
        return NULL;
    t_list *p = head;
    int i = 0;
    p->type = check_type(commands[i][0]);
    int c = p->type;
    p->command = commands[i++];
    p->next = NULL;
    while(commands[i])
    {
        t_list *new = gc_malloc(sizeof(t_list));
        if (!new)
            return NULL;
        new->command = commands[i];
        if (c == 2)
            new->type = 22;
        else if (c == 3)
            new->type = 33;
        else if (c == 4)
            new->type = 44;
        else if (c == 5)
            new->type = 55;
        else
            new->type = check_type(commands[i][0]);
        c = new->type;
        new->next = NULL;
        p->next = new;
        p = p->next;
        i++;
    }
    return head;
}

int count_pipes(char *s)
{
    int i = 0;
    int count = 0;
    while (s[i])
    {
        while (s[i] == ' ')
            i++;
        if (s[i + 1] && s[i] == '>' && s[i + 1] == '>')
        {
            count++;
            i += 2;
        }
        else if (s[i + 1] && s[i] == '<' && s[i + 1] == '<')
        {
            count++;
            i += 2;
        }
        else if (s[i] == '|' || s[i] == '>' || s[i] == '<')
        {
            count++;
            i++;
        }
        else if (s[i])
        {
            count++;
            while (s[i] && s[i] != '|' && s[i] != '<' && s[i] != '>')
                i++;
        }
    }
    return count;
}

char **ft_split_pipes(char *s)
{
    int count = count_pipes(s);
    int i = 0;
    char **arr = gc_malloc(sizeof(char *) * (count + 1));
    if (!arr)
        return NULL;
    i = 0;
    int k = 0;
    while(s[i] && k < count)
    {
        while(s[i] && s[i] == ' ')
            i++;
        if (!s[i])
            break;
        int st = i;
        while(s[i] && s[i] != '|' && s[i] != '<' && s[i] != '>')
        {
            char c = s[i];
            if (c == '\"' || c == '\'')
            {
                i++;
                while(s[i] && s[i] != c)
                    i++;
                if (!s[i])
                    break;
                // continue;
            }
            i++;
        }
        int e = i;
        if (e > st)
        {
            arr[k] = gc_malloc(sizeof(char) * (e - st + 1));
            int j = 0;
            while(s[st] && st < e)
                arr[k][j++] = s[st++];
            arr[k++][j] = '\0';
        }
        int length = 1;
        int t_i = 0;
        if (s[i] && s[i + 1] && s[i] == '>' && s[i + 1] == '>')
        {
            int length = 3;
            arr[k] = gc_malloc(sizeof(char) * length);
            if (!arr[k])
                return NULL;
            arr[k][t_i++] = s[i++];
            arr[k][t_i++] = s[i++];
            arr[k++][t_i] = '\0';
        }
        else if (s[i] && s[i + 1] && s[i] == '<' && s[i + 1] == '<')
        {
            arr[k] = gc_malloc(sizeof(char) * 3);
            arr[k][0] = '<';
            arr[k][1] = '<';
            arr[k++][2] = '\0';
            i += 2;
        }
        else if (s[i] && s[i] == '|' || s[i] == '>' || s[i] == '<')
        {
            arr[k] = gc_malloc(sizeof(char) * 2);
            if (s[i] == '|')
                arr[k][0] = '|';
            else if (s[i] == '<')
                arr[k][0] = '<';
            else if (s[i] == '>')
                arr[k][0] = '>';
            arr[k++][1] = '\0';
            i++;
        }
    }
    arr[k] = NULL;
    return arr;
}

int list_size(t_list *lst)
{
    int i = 0;
    while(lst)
    {
        i++;
        lst = lst->next;
    }
    return i;
}

char *expand_quotes(char *s)
{
    int size = ft_strlen(s);
    int i = 0;
    char c = -2;
    int j = 0;
    int start = -1;
    int end = -1;
    int found = -1;
    int arr[100][2];//allocate
    int should_expand = 0;
    while(i < 100)
    {
        arr[i][0] = -2;
        arr[i][1] = -2;
        i++;
    }
    i = 0;
    int prev_i = -1;
    int inside = 0;//-> 0 signle quotes | 1 double quotes
    while (s[i])
    {
        found = 0;
        if (s[i] == '\'' || s[i] == '"') 
        {
            c = s[i];
            if (c == '\'')
                inside = 0;
            else if (c == '"')
                inside = 1;
            start = i;
            i++;
            while (s[i] && s[i] != c)
            {
                if (s[i] == '$' && inside == 1)
                {
                    //fill array of the $ number where u should expand (first one | second | third ...)
                    should_expand = 1;
                }
                i++;
            }
            if (s[i] == c)
            {
                end = i;
                arr[j][0] = start;
                arr[j][1] = end;
                j++;
            }
        }
        else
        {
            if (i > 0)
            {
                if (s[i - 1] && s[i - 1] == '\'' || s[i - 1] == '"')
                    start = i - 1;
            }
            while(s[i] && (s[i] != '\'' && s[i] != '"'))
                i++;
            end = i;
            arr[j][0] = start;
            arr[j][1] = end;
            j++;
            found = 1;
        }
        if (!found)
            i++;
    }
    i = 0;
    j = 0;
    char *str = ft_strdup("");
    while(arr[j][0] != -2)
    {
        char *s1 = malloc(sizeof(char) * (arr[j][1] - arr[j][0] + 1));
        i = arr[j][0] + 1;
        int k = 0;
        while(i < arr[j][1])
        {
            s1[k++] = s[i++];
        }
        s1[k] = '\0';
        str = ft_strjoin1(str, s1);
        j++;
    }
    if (arr[0][0] == -2)
        return s;
    return str;
}

int ft_var_compare(char *str, char *var_name)
{
    int  i = 0;
    if  (ft_strlen(str) != ft_strlen(var_name))
        return 1;
    while(str[i] && str[i] == var_name[i])
        i++;
    return (str[i] - var_name[i]);
}

char *find_in_env(t_env *env, char *str)
{
    t_env *p = env;
    while(p)
    {
        if (ft_var_compare(str, p->var_name) == 0)
            return (p->var_value);
        p = p->next;
    }
    return NULL;
}

char *get_env_variable(t_exec *executor, char *s, int start, int *end)
{
    int i = start;

    while(s[i])
    {
        if (ft_isalnum(s[i]) || s[i] == '_')
            i++;
        else
            break;
    }
    *end = i;
    int j = 0;
    char *str = malloc(sizeof(char) * (i - start + 1));
    while(start < i)
        str[j++] = s[start++];
    str[j] = '\0';
    return (find_in_env(executor->env, str));
    // return (new_env);
}

int is_in_arr(int counter, int *arr)
{
    int i = 0;
    while(arr[i] != -2)
    {
        if (arr[i] == counter)
            return 1;
        i++;
    }
    return 0;
}

char *new_str(char *s, int to_skip)
{
    int i = 0;
    char *str = malloc(sizeof(char) * (ft_strlen(s) + 1));
    if (!str)
        return NULL;
    int j = 0;
    while(s[i])
    {
        if (i == to_skip)
            i += 2;
        str[j++] = s[i++];
    }
    str[j] = '\0';
    return str;
}

int check_if_limiter(char *s, int i)
{
    i--;
    while(i > 0)
    {
        if (s[i] == '<' && s[i - 1] == '<')
            return 0;
        if (ft_isalnum(s[i]))
            break;
        i--;
    }
    return 1;
}

char *handle_dollars(char *s, int type)
{
    if (type == 55 | type == 5)
        return s;
    int expand = 0;
    char c = -2;
    char *new_s;
    int i = 0;
    int here_doc_found = 0;
    while(s[i])
    {
        if (s[i] && s[i + 1] && s[i] == '<' && s[i + 1] == '<')
        {
            i += 2;
            while(s[i] && s[i] == ' ')
                i++;
            while(s[i] && s[i] != ' ')
                i++;
        }
        if (s[i] == '\'' || s[i] == '"')
        {
            c = s[i];
            i++;
            while(s[i] && s[i] != c)
            {
                if (c == '"')
                {
                    if (s[i] && s[i + 1] && s[i] == '$' && s[i + 1] == '$')
                    {
                        new_s = new_str(s, i);    
                        s = new_s;
                        i = 0;
                    }
                }
                i++;
            }
            if (!s[i])
                break;
        }
        else if (s[i] && s[i + 1] && s[i] == '$' && s[i + 1] == '$')
        {
                new_s = new_str(s, i);    
                s = new_s;
                i = 0;
        }
        else
            i++;
    }
    return s;
}

// "       $a    s    "
static char **split_env_value(char *value) 
{
    char *trimmed;
    char **split;
    
    if (!value)
        return NULL;
    trimmed = ft_strtrim(value, " \t\n");
    split = ft_split(trimmed, ' ');
    return split;
}

static int count_expanded_words(char *env_value)
{
    char **split;
    int count = 0;
    
    split = split_env_value(env_value);
    if (!split)
        return 0;
    while (split[count])
        count++;
    return count;
}

// echo "123" $USER 456
char **expand_env(t_exec *executor, char **commands, int *arr, int type)
{
    // if (type == 5 || type == 55)
    //     return commands;
    int count = 0;
    int dollar_count = 0;
    int k = 0;
    int i = 0;
    int expanded = 0;
    char *s = NULL;
        char quote_char = 0;
    int was_quoted = 0;
    char *new_str = ft_strdup("");
    char **result = malloc(sizeof(char *) * 1024);
    int result_size = 0;
    while(commands[k])
    {
        s = commands[k];
        i = 0;
        while(s[i])
        {
            expanded = 0;
            if (s[i] == '\'' || s[i] == '"')
            {
                if (!quote_char)
                {
                    quote_char = s[i];
                    was_quoted = 1;
                }
                else if (quote_char == s[i])
                    quote_char = 0;
                new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                i++;
            }
            else if (s[i] == '$' && (!quote_char || quote_char == '"'))
            {
                dollar_count++;
                if (s[i + 1] == '?')
                {
                    int *e = exit_status();
                    new_str = ft_strjoin1(new_str, ft_itoa(*e));
                    result[result_size] = ft_strjoin1(result[result_size], ft_strdup(new_str));
                    free(new_str);
                    new_str = ft_strdup("");
                    i += 2;
                }
                else if (ft_isdigit(s[i + 1]))
                {
                    if ((!quote_char || quote_char == '"'))
                        i += 2;
                    else
                    {
                        new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                        i++;
                    }
                }
                else if (ft_isalpha(s[i + 1]) || s[i + 1] == '_')
                {
                    if ((!quote_char || quote_char == '"'))
                    {
                        i++;
                        char *word = get_env_variable(executor, s, i, &i);
                        if (!word && type != 0)
                        {
                            write(2, s, ft_strlen(s));
                            write(2, ": ambiguous redirect\n", 21);
                            set_exit_status(1);
                            return NULL;
                        }
                        if (word)
                        {
                            char *joined;
                            if (new_str && new_str[0])
                            {
                                new_str = ft_strjoin1(result[result_size], new_str);
                                joined = ft_strjoin1(new_str, word);
                            }
                            else
                                joined = ft_strjoin1(result[result_size], word);
                            char **splited = ft_split(joined, ' ');
                            free(new_str);
                            new_str = ft_strdup("");
                            int j = 0;
                            while(splited[j])
                                    result[result_size++] = ft_strdup(splited[j++]);
                            result_size--;
                        }
                        else
                        {
                            new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                            i++;
                        }
                    }
                    else
                    {
                        new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                        i++;
                    }
                }
                else
                {
                    new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                    i++;
                }
            }
            else if(s[i] != '"')
            {
                new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                i++;
            }
        }
            result[result_size++] = ft_strjoin1(result[result_size], new_str);
            free(new_str);
            new_str = ft_strdup("");
        k++;
    }
    if (result)
        result[result_size] = NULL;
    return result;
}

char *expand_env_export(t_exec *executor, char *s, int *arr, int flag, int type)
{
    if (type == 55)
        return s;
    int i = 0;
    int counter = 0;
    char *new_str = ft_strdup("");
    int in_heredoc = 0;
    char quote_char = 0;
    int was_quoted = 0;
    
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
        {
            if (!quote_char)
            {
                quote_char = s[i];
                was_quoted = 1;
            }
            else if (quote_char == s[i])
                quote_char = 0;
            new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
            i++;
        }
        else if (s[i] == '$' && (!quote_char || quote_char == '"') && check_if_limiter(s, i))
        {
            counter++;
            if (s[i + 1] == '?')
            {
                int *e = exit_status();
                new_str = ft_strjoin1(new_str, ft_itoa(*e));
                i += 2;
            }
            else if (ft_isdigit(s[i + 1]))
            {
                if (is_in_arr(counter, arr) || was_quoted)
                    i += 2;
                else
                {
                    new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                    i++;
                }
            }
            else if (ft_isalpha(s[i + 1]) || s[i + 1] == '_')
            {
                if (is_in_arr(counter, arr) || was_quoted)
                {
                    i++;
                    char *word = get_env_variable(executor, s, i, &i);
                    if (word)
                        new_str = ft_strjoin1(new_str, word);
                }
                else
                {
                    new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                    i++;
                }
            }
            else
            {
                new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                i++;
            }
        }
        else
        {
            new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
            i++;
        }
    }
    return new_str;
}

int *which_to_expand(char *s, int type)
{
    if (type == 55 | type == 5)
        return NULL;
    int found = 0;
    int i = 0;
    char c = -2;
    int inside = 0;
    int start;
    int end;
    int should_expand = 0;
    int *arr = malloc(sizeof(int) * 1024);
    while(i < 1024)
        arr[i++] = -2;
    int j = 0;
    int counter = 0;
    i = 0;
    while (s[i])
    {
        found = 0;
        if (s[i] && s[i + 1] && s[i] == '<' && s[i + 1] == '<')
        {
            while(s[i] && s[i] == ' ')
                i++;
            while(s[i] && s[i] != ' ')
                i++;
        }
        else if (s[i] == '\'' || s[i] == '"') 
        {
            c = s[i];
            if (c == '\'')
                inside = 0;
            else if (c == '"')
                inside = 1;
            i++;
            while (s[i] && s[i] != c)
            {
                if (s[i] == '$')
                    counter++;
                if (s[i] == '$' && inside == 1)
                {
                    if (s[i + 1] && ft_isalnum(s[i + 1]))
                        arr[j++] = counter;
                    else if (s[i + 1] && s[i + 1] == '$' || s[i + 1] == '_')
                    {
                        arr[j++] = counter++;
                        i++;
                    }
                }
                i++;
            }
        }
        else
        {
            inside = 1;
            if (i > 0)
            {
                if (s[i - 1] && (s[i - 1] == '\'' || s[i - 1] == '"'))
                    start = i - 1;
            }
            while(s[i] && (s[i] != '\'' && s[i] != '"'))
            {
                if (s[i] == '$')
                    counter++;
                if (s[i] == '$' && inside == 1)
                {
                    if (s[i + 1] && ft_isalnum(s[i + 1]))
                        arr[j++] = counter;
                    else if (s[i + 1] == '$' || s[i + 1] == '_')
                    {
                        arr[j++] = counter++;
                        i++;
                    }
                }
                i++;
            }
            found = 1;
        }
        if (!found)
            i++;
    }
    return arr;
}

int count_words(char *s)
{
    int i = 0;
    int count = -1;
    while(s[i])
    {
        while(s[i] && s[i] == ' ')
            i++;
        if (s[i])
            count++;
        while(s[i] && s[i] != ' ')
        {
            if (s[i] == '"')
            {
                char c = s[i++];
                while(s[i] && s[i] != c)
                    i++;
                i++;
            }
            else
                i++;
        }
    }
    return count;
}

t_list *expand(t_exec *executor, t_list *commands)
{
    t_list *expanded_list = gc_malloc(sizeof(t_list));

    t_list *p = commands;
    char *s = NULL;
    int should_expand = 0;
    char *expanded = NULL;
    while(p)
    {
        int i = 0;
        int n_i = 0;
        char **new = malloc(sizeof(char *) * 1024);
        while(p->command[i])
        {
            int ii = -1;
            while(p->command[++ii])
                p->command[ii] = handle_dollars(p->command[ii], p->type);
            int *arr = which_to_expand(p->command[i], p->type);
            if (ft_strcmp(p->command[0], "export"))
            {
                char **new = expand_env(executor, p->command, arr, p->type);
                if (!new)
                    return NULL;
                int j = -1;
                while(new[++j])
                {
                    if (new[j][0] == '\0')
                    {
                        return NULL;
                    }
                    new[j] = expand_quotes(new[j]);
                }
                p->command = new;
                break;
            }
            else
            {
                p->command[i] = expand_quotes(p->command[i]);
                expanded = expand_env_export(executor, p->command[i], arr, 1, p->type);
            }
            if (ft_strcmp(p->command[0], "export") == 0)
                p->command[i] = expanded;
            i++;
        }
        // new[n_i] = NULL;
        // if (ft_strcmp(p->command[0], "export"))
        //     p->command = new;
        p = p->next;
    }
    return commands;
}

char *expand_here_doc(char *s, t_exec *executor)
{
    int i = 0;
    char *new_str = ft_strdup("");
    
    while (s[i])
    {
        if (s[i] == '$')
        {
            if (s[i + 1] == '?')
            {
                int *e = exit_status();
                new_str = ft_strjoin1(new_str, ft_itoa(*e));
                i += 2;
            }
            else if (ft_isdigit(s[i + 1]))
            {
                    new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                    i++;
            }
            else if (ft_isalpha(s[i + 1]) || s[i + 1] == '_')
            {
                    i++;
                    char *word = get_env_variable(executor, s, i, &i);
                    if (word)
                        new_str = ft_strjoin1(new_str, word);
            }
            else
            {
                new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
                i++;
            }
        }
        else
        {
            new_str = ft_strjoin1(new_str, (char[]){s[i], '\0'});
            i++;
        }
    }
    return new_str;
}

int read_here_doc(t_exec *executor, char *limiter, int counter)
{
    executor->here_doc_fd = open("here_doc", O_CREAT | O_RDWR | O_TRUNC, 0644);
    int *sig_state = signal_state();
    *sig_state = 3;
    t_sig *signal_stat = sig_handler();
    signal_stat->stop_reading = 0;
    signal_stat->reading_from_here_doc = 1;
    char *line;
    while(1)
    {
        line = readline("> ");
        if (!line)
        {
            printf("minishell: warning: here-document at line %d delimited by end-of-file (wanted '%s')\n", counter, limiter);
            close(executor->here_doc_fd);
            return 0;
        }
        if (ft_strcmp(line, limiter) == 0)
            break;
        line = handle_dollars(line, 0);
        line = expand_here_doc(line, executor);
        write(executor->here_doc_fd, line, ft_strlen(line));
        write(executor->here_doc_fd, "\n", 1);
    }
    close(executor->here_doc_fd);
    signal_stat->reading_from_here_doc = 0;
    return 1;
}

int count_here_docs(t_exec *executor)
{
    t_list *p = executor->commands_list;
    int count = 0;
    while(p)
    {
        if (p->type == 55)
            count++;
        p = p->next;
    }
    return count;
}

int handle_here_doc(t_exec *executor)
{
    signal(SIGINT, handle_ctrlc_child);
    static int counter;
    int count = count_here_docs(executor);//limit here_docs to get defined behavior
    if (count > 10)
    {
        write(2, "to many here_docs\n", 18);
        set_exit_status(2);
        exit(2);
    }
    t_list *p = executor->commands_list;
    while(p && p->next)
    {
        if (p->type == 5)
        {
                counter++;
            if (read_here_doc(executor, p->next->command[0], counter))
                counter = counter++;
        }
        p = p->next;
    }
    exit(0);
}

int simple_parsing(char *s, t_exec *executor)
{
    int status;
    t_sig *stats = sig_handler();
    char *str = ft_strtrim(s, " \n");
    if (!str[0])
        return 1;
    if (syntax_errors(str))
        return (set_exit_status(2), 1);
    executor->tokens = ft_split_pipes(str);
    // int j = 0;
    // while(executor->tokens[j])
    // {
    //     printf("|%s|\n", executor->tokens[j]);
    //     j++;
    // }
	executor->commands = ft_split_tokens(executor->tokens);
    executor->commands_list = parse_list(executor->commands);
    executor->commands_list = expand(executor, executor->commands_list);

    if (!executor->commands_list)
        return 1;
    stats->pid = fork();
    if (!stats->pid)
        handle_here_doc(executor);
    else if(stats->pid > 0)
    {
        waitpid(stats->pid, &status, 0);
        if (WEXITSTATUS(status))
            return 1;
    }
    t_list *tmp = executor->commands_list;
    while(tmp)
    {
        int i = 0;
        int f = 0;
        while(tmp->command[i])
        {
            if (ft_strcmp(tmp->command[i], ".") == 0)
            {
                write(2, "minishell: .: filename argument required\n", 41);
                write(2, ".: usage: . filename [arguments]\n", 33);
                set_exit_status(2);
                return 1;
            }
            else if (ft_strcmp(tmp->command[0], "..") == 0)
            {
                write(2, "Error : command not found: ..\n", 30);
                set_exit_status(127);
                return 1;
            }
            else if (tmp->command[i][0] == '/')
            {
                write(2, "minishell: ", 11);
                write(2, tmp->command[i], ft_strlen(tmp->command[i]));
                write(2, ": Is a directory\n", 17);
                set_exit_status(126);
                return 1;
            }
            i++;
        }
        tmp = tmp->next;
    }
    stats->executing = 1;
    // int i;
    // while(executor->commands_list)
    // {
    //     i = 0;
    //     while(executor->commands_list->command[i])
    //     {
    //         printf("|%s|,", executor->commands_list->command[i++]);
    //     }
    //     printf("\n");
    //     executor->commands_list = executor->commands_list->next;
    // }
    // if (!i)
    //     return 1;
    return (0);
}
