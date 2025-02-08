/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakhrif <aakhrif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:57:28 by aakhrif           #+#    #+#             */
/*   Updated: 2025/01/11 17:27:30 by aakhrif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen1(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (0);
}

char	*ft_strjoin1(char *s1, char *s2)
{
	int		i;
	char	*d;
	int		j;

	i = 0;
	j = 0;
	if (!s1)
		s1 = ft_strdup("");
	d = ft_calloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1), 1);
	if (!d)
		return (free(s1), exit(1), NULL);
	if (s1)
		while (s1[i])
			d[j++] = s1[i++];
	j = 0;
	while (s2[j])
	{
		d[i + j] = s2[j];
		j++;
	}
	d[i + j] = '\0';
	return (d);
}
