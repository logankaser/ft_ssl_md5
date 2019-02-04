/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_has_only.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/06 20:24:42 by lkaser            #+#    #+#             */
/*   Updated: 2018/07/06 20:24:43 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_bool	ft_str_has_only(const char *str, const char *has_only)
{
	char *check;

	while (*str)
	{
		check = (char *)has_only;
		while (*check && *check != *str)
			++check;
		if (!*check)
			return (0);
		++str;
	}
	return (1);
}
