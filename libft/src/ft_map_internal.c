/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 18:14:20 by lkaser            #+#    #+#             */
/*   Updated: 2018/07/23 09:05:56 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

uint32_t	ft_map_hash(t_map *m, const char *key)
{
	uint32_t hash;

	if (m->key_size)
		hash = ft_fnv_32((uint8_t*)key, m->key_size);
	else
		hash = ft_fnv_32((uint8_t*)key, ft_strlen(key));
	return (hash);
}

static void	map_place_link(void **new, unsigned bucket_index, t_list *link)
{
	t_list		*i;

	link->next = NULL;
	if (!new[bucket_index])
	{
		new[bucket_index] = link;
		return ;
	}
	i = new[bucket_index];
	while (i->next)
		i = i->next;
	i->next = link;
}

void		ft_map_resize_(t_map *m, unsigned size)
{
	void		**new;
	unsigned	i;
	t_list		*bucket;
	t_list		*tmp;

	new = ft_memalloc(sizeof(t_list*) * size);
	i = 0;
	while (i < m->capacity)
	{
		if (!(bucket = m->data[i++]))
			continue ;
		while (bucket)
		{
			tmp = bucket->next;
			map_place_link(new, bucket->content_size % size, bucket);
			bucket = tmp;
		}
	}
	free(m->data);
	m->data = new;
	m->capacity = size;
}

/*
** Reuses the 42 list struct by storing the hash in the
** content_size field, which allows the reuse of all the t_lst fucntions.
*/

void		ft_map_insert(t_map *m, uint32_t hash, void *ptr)
{
	t_list		*bucket;
	unsigned	bucket_index;

	bucket_index = hash % m->capacity;
	if (!m->data[bucket_index])
	{
		ft_lstpush((t_list **)m->data + bucket_index, ptr, hash);
		++m->count;
	}
	else
	{
		bucket = m->data[bucket_index];
		while (bucket->next && bucket->next->content_size != hash)
			bucket = bucket->next;
		if (!bucket->next)
		{
			bucket->next = ft_lstnew(NULL, 0);
			++m->count;
		}
		bucket->next->content_size = hash;
		bucket->next->content = ptr;
	}
}
