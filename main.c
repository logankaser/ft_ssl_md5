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

// md5: "apple" -> 1f3870be274f6c49b3e31a0c6728957f
// sha-2-256 "apple" -> 3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b

int				main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	printf("md5: %s\n", ft_md5(argv[1]));
	printf("sha2_256: %s\n", ft_sha2_256(argv[1]));
	return (0);
}
