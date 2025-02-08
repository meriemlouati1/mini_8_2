/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlouati <mlouati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:59:07 by aakhrif           #+#    #+#             */
/*   Updated: 2025/02/08 14:55:59 by mlouati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	*ft_calloc(size_t count, size_t size)
{
	char	*ptr;

	if (!count || !size)
		return (NULL);
	if (size != 0 && size > (SIZE_MAX / count))
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	return (ptr);
}

char	*ft_strdup(const char *s)
{
	int		len;
	char	*d;
	int		i;

	if (!s)
		return (NULL);
	len = ft_strlen((char *)s);
	d = ft_calloc(sizeof(char) * (len + 1), 1);
	if (!d)
		return (NULL);
	i = 0;
	while (s[i])
	{
		d[i] = s[i];
		i++;
	}
	d[i] = '\0';
	return (d);
}

int ft_strcmp(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (1);
	int i = 0;
	while(s1[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_substr(char *s, int start, int len)
{
	char			*sub;
	int	i;

	if (!s)
		return (0);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > (ft_strlen(s) - start))
		len = ft_strlen(s) - start;
	sub = (char *)malloc(len + 1);
	i = 0;
	if (!sub)
		return (NULL);
	while (s[i + start] && i < len)
	{	
		sub[i] = s[i + start];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

long long ft_atoi(char *str, int *error)
{
	int	i;
	int	signe;
	long long res;

	i = 0;
	signe = 1;
	res = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			signe = -1;
		i++;
	}
    while(str[i] == '0')
        i++;
    int start = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
        start++;
	}
    if (start >= 20 || res*signe > LONG_MAX || res*signe < LONG_MIN)
        *error = 1;
	return (signe * res);
}

int	ft_isalpha(int c)
{	
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) == 1 || ft_isdigit(c) == 1)
		return (1);
	return (0);
}
