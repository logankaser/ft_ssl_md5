/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/06 20:23:19 by lkaser            #+#    #+#             */
/*   Updated: 2018/07/06 20:23:20 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	uint8_t			*d;
	const uint64_t	*sr;
	uint64_t		*de;

	d = dest;
	while (n && n % 8)
	{
		--n;
		d[n] = ((uint8_t*)src)[n];
	}
	n /= 8;
	de = dest;
	sr = src;
	while (n)
	{
		--n;
		de[n] = sr[n];
	}
	return (dest);
}
