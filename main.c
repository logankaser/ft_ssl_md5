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
#include <fcntl.h>
#include "libft.h"
#include "ft_ssl.h"

#define USAGE "usage: ft_ssl command [command opts] [command args]"

/*
** md5: "apple"
** -> 1f3870be274f6c49b3e31a0c6728957f
** sha-2-256 "apple"
** -> 3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b
** sha-1-256 "apple"
** -> d0be2dc421be4fcd0172e5afceea3970e2f3d940
**.
*/

static uint8_t *read_file(const char *filepath)
{
	int		fd;
	t_uvector buf;
	ssize_t	ret;

	fd = STDIN_FILENO;
	if (filepath)
		fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	ft_uvector_init(&buf, 1);
	while (
		(ret = read(fd, buf.data + buf.length, buf.capacity - buf.length)) > 0)
	{
		buf.length += ret;
		if (buf.capacity < buf.length + 1)
			ft_uvector_resize(&buf, buf.capacity * 2);
	}
	ft_uvector_push(&buf, "\0");
	close(fd);
	return buf.data;
}

static void format_print(
	const char *hash_string, const char *src, t_bool reverse, t_bool quiet)
{
	if (quiet)
		ft_putendl(hash_string);
	else
	{
		if (reverse)
			ft_printf("%s %s\n", hash_string, src);
		else if (ft_strlen(hash_string) == 32)
			ft_printf("MD5 (%s) = %s\n", src, hash_string);
		else if (ft_strlen(hash_string) == 40)
			ft_printf("SHA1 (%s) = %s\n", src, hash_string);
		else if (ft_strlen(hash_string) == 64)
			ft_printf("SHA256 (%s) = %s\n", src, hash_string);
	}
}

static int process_arguments(t_hash_function *hash, int argc, char **argv)
{
	int		i;
	t_bool	reverse;
	t_bool	quiet;
	t_bool	hashed_file;
	uint8_t	*data;

	if (argc == 2)
		format_print(hash(read_file(NULL)), NULL, FALSE, TRUE);
	i = 2;
	reverse = FALSE;
	quiet = FALSE;
	hashed_file = FALSE;
	while (i < argc && argv[i][0] == '-')
	{
		if (!ft_strcmp("-r", argv[i]))
			reverse = TRUE;
		else if (!ft_strcmp("-q", argv[i]))
			quiet = TRUE;
		else if (!ft_strcmp("-p", argv[i]))
		{
			data = read_file(NULL);
			ft_putstr((char*)data);
			format_print(hash(data), NULL, FALSE, TRUE);
			free(data);
		}
		else if (!ft_strcmp("-s", argv[i]))
		{
			++i;
			if (i == argc)
			{
				ft_fprintf(stderr, "ft_ssl: %s: -s: Requires argument.\n%s\n", argv[1], USAGE);
				continue;
			}
			format_print(hash((uint8_t*)argv[i]), argv[i], reverse, quiet);
			hashed_file = TRUE;
		}
		i += 1;
	}
	return 0;
}

int			main(int argc, char **argv)
{
	t_map			hashes;
	t_hash_function	*hash;

	ft_map_init(&hashes, 0, 17);
	ft_map_set(&hashes, "md5", &ft_md5);
	ft_map_set(&hashes, "sha256", &ft_sha2_256);
	ft_map_set(&hashes, "sha1", &ft_sha);
	if (argc < 2)
	{
		ft_puterror(USAGE);
		return (1);
	}
	hash = ft_map_get(&hashes, argv[1]);
	ft_map_clear(&hashes, NULL);
	if (hash)
		return process_arguments(hash, argc, argv);
	else
	{
		ft_fprintf(stderr, "ft_ssl: '%s' is an invalid command.\n", argv[1]);
		return (1);
	}
}
