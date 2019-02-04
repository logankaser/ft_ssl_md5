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

void	ft_map_init(t_map *m, unsigned key_size, unsigned size)
{
	m->data = ft_memalloc(sizeof(t_list*) * size);
	m->count = 0;
	m->capacity = size;
	m->key_size = key_size;
}

/*
** Current implementation is chaining based,
** Reuses the 42 list struct by storing the hash in the
** content_size field, which allows the reuse of all the t_lst fucntions.
*/

void	ft_map_set(t_map *m, const char *key, void *ptr)
{
	ft_map_insert(m, ft_map_hash(m, key), ptr);
	if (m->count / (double)m->capacity >= 0.7)
		ft_map_resize_(m, m->capacity * 2 + 1);
}

void	*ft_map_remove(t_map *m, const char *key)
{
	uint32_t	hash;
	t_list		*bucket;
	void		*value;
	t_list		*last;

	hash = ft_map_hash(m, key);
	if ((bucket = m->data[hash % m->capacity]) && bucket->content_size == hash)
	{
		m->data[hash % m->capacity] = bucket->next;
		value = bucket->content;
		free(bucket);
		return (value);
	}
	last = NULL;
	while (bucket && bucket->content_size != hash)
	{
		last = bucket;
		bucket = bucket->next;
	}
	if (!bucket)
		return (NULL);
	last->next = bucket->next;
	value = bucket->content;
	free(bucket);
	return (value);
}

void	*ft_map_get(t_map *m, const char *key)
{
	uint32_t	hash;
	t_list		*bucket;

	hash = m->key_size ? ft_fnv_32((uint8_t*)key, m->key_size)
		: ft_fnv_32((uint8_t*)key, ft_strlen(key));
	bucket = m->data[hash % m->capacity];
	while (bucket)
	{
		if (bucket->content_size == hash)
			return (bucket->content);
		bucket = bucket->next;
	}
	return (NULL);
}

void	ft_map_clear(t_map *m, void (*free_fn)(void *))
{
	unsigned	i;
	t_list		*bucket;
	t_list		*tmp;

	i = 0;
	while (i < m->capacity)
	{
		if (!(bucket = m->data[i]))
		{
			++i;
			continue ;
		}
		while (bucket)
		{
			if (free_fn)
				free_fn(bucket->content);
			tmp = bucket;
			bucket = bucket->next;
			free(tmp);
		}
		m->data[i++] = NULL;
	}
	m->count = 0;
}
