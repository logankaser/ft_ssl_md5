/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfind.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/06 20:22:21 by lkaser            #+#    #+#             */
/*   Updated: 2018/07/06 20:22:22 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstfind(t_list *lst,
	t_bool (*pred)(const void *, const void *), const void *data)
{
	while (lst)
	{
		if (pred(lst->content, data))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
