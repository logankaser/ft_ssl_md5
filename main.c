/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 18:01:16 by lkaser            #+#    #+#             */
/*   Updated: 2019/01/15 18:14:14 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "libft.h"
#include "ft_ssl.h"

/*
** md5: "apple"
** -> 1f3870be274f6c49b3e31a0c6728957f
** sha-2-256 "apple"
** -> 3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b
** sha-1-256 "apple"
** -> d0be2dc421be4fcd0172e5afceea3970e2f3d940
**.
*/

int	main(int argc, char **argv)
{
	t_map			hashes;
	t_hash_function	*hash;

	ft_map_init(&hashes, 0, 17);
	ft_map_set(&hashes, "md5", &ft_md5);
	ft_map_set(&hashes, "sha256", &ft_sha2_256);
	ft_map_set(&hashes, "sha1", &ft_sha);
	if (argc < 2)
	{
		ft_putstr("usage: ft_ssl command [command opts] [command args]\n");
		return (1);
	}
	hash = ft_map_get(&hashes, argv[1]);
	if (hash)
		ft_printf("%s: %s\n", argv[1], hash((uint8_t*)argv[2]));
	else
	{
		ft_fprintf(stderr, "ft_ssl: '%s' is an invalid command.\n", argv[1]);
		return (1);
	}
}
