/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:22:50 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 15:00:25 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_set(char c, char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	t_length(const char *s, const char *set, int *ss, int *e)
{
	int	i;
	int	start;
	int	end;

	i = 0;
	start = 0;
	while (s[i] && is_set(s[i], (char *)set))
		i++;
	if (!s[i])
		return (0);
	start = i;
	*ss = start;
	while (s[i])
		i++;
	i--;
	while (i >= 0 && is_set(s[i], (char *)set))
		i--;
	end = i;
	*e = end;
	return (end - start + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*d;
	int		start;
	int		end;
	int		t_len;
	int		i;

	start = 0;
	end = 0;
	if (!set || !s1)
		return (NULL);
	t_len = t_length(s1, set, &start, &end) + 1;
	d = gc_malloc(sizeof(char) * t_len);
	if (!d)
		return (NULL);
	i = 0;
	if (t_len > 1)
	{
		while (start <= end)
			d[i++] = s1[start++];
	}
	d[i] = '\0';
	return (d);
}
