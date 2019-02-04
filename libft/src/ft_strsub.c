/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/06 20:27:06 by lkaser            #+#    #+#             */
/*   Updated: 2018/07/06 20:27:07 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *str, unsigned start, size_t len)
{
	char		*sub;
	unsigned	i;

	NULL_GUARD(str);
	sub = malloc(len + 1);
	NULL_GUARD(sub);
	sub[len] = '\0';
	i = 0;
	str += start;
	while (i < len)
	{
		sub[i] = str[i];
		++i;
	}
	return (sub);
}
