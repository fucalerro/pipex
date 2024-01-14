/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lferro <lferro@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:14:58 by lferro            #+#    #+#             */
/*   Updated: 2024/01/14 14:13:33 by lferro           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Allocates (with malloc(3)) and returns a substring from the
 * string ’s’.
 *
 * @param s
 * @param start
 * @param len
 * @return char*
 */
char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;
	size_t	j;

	start = (size_t)start;
	i = 0;
	j = 0;
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s))
		len = ft_strlen(s) - start;
	res = malloc(sizeof(char) * (len + 1));
	if (res == 0)
		return (0);
	while (i < start && *s)
		i++;
	while (i >= start && i < len + start)
		res[j++] = s[i++];
	res[j] = 0;
	return (res);
}

// int main(int argc, char const *argv[])
// {

// 	// char s1[] = "lorem ipsum dolor sit amet";
// 	// int start = 400;
// 	// int len = 20;

// 	printf("ft: %s\n", ft_substr("tripouille", 0, 42000));

// 	return (0);
// }
