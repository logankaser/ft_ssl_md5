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
    size_t i;

    i = 0;
	while(i < n)
	{
		((uint8_t*)dest)[i] = ((uint8_t*)src)[i];
        ++i;
	}
	return (dest);
}

/*
static	void big_copy(void *dest, const void *src, size_t count)
{

}

static	void copy(void *dest, const void *src, size_t count)
{

}

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	size_t	alignment_offset;
	size_t	large;
	size_t	remaining;

	alignment_offset= MIN((size_t)dest % sizeof(long), (size_t)src % sizeof(long));
	void *position = dest;

	if (bytes_to_align > 0)
	{
		copy(position, src, bytes_to_align);
		position = (char *)position + bytes_to_align;
		src = (char *)src + bytes_to_align;
		size -= bytes_to_align;
	}

	large = size / sizeof(long);
	remaining = size % sizeof(long);
}
*/