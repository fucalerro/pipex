/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 19:14:17 by lferro            #+#    #+#             */
/*   Updated: 2024/01/14 14:12:31 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Allocates (with malloc(3)) and returns a new string, which is the
 * result of the concatenation of ’s1’ and ’s2’.
 *
 * @param s1
 * @param s2
 * @return char*
 */
char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*res;
	size_t	i;
	size_t	j;
	size_t	totlen;

	i = 0;
	j = 0;
	totlen = ft_strlen(s1) + ft_strlen(s2);
	res = malloc(sizeof(char) * (totlen + 1));
	if (res == NULL)
		return (NULL);
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = 0;
	return (res);
}

// int	main(void)
// {
// 	char	**s1 = {"", "123456789"};
// 	char	**res = {"", "123456789", "123456789", "123456789123456789"};
// 	for (size_t i = 0; i < 2; i++)
// 		for (size_t j = 0; j < 2; j++)
// 			if (strcmp(ft_strjoin(s1[i], s1[j]), res[i + j]) != 0)
// 				return (ERR);
// 	return (OK);
// }
