/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/06 20:24:51 by lkaser            #+#    #+#             */
/*   Updated: 2018/07/06 20:24:52 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strcat(char *dst, char *src)
{
	char *ch;

	ch = dst;
	while (*ch)
		++ch;
	while (*src)
		*ch++ = *src++;
	*ch = '\0';
	return (dst);
}
