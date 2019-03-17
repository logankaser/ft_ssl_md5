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
** sha256 "apple"
** -> 3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b
** sha1 "apple"
** -> d0be2dc421be4fcd0172e5afceea3970e2f3d940
**.
*/

static uint8_t	*read_file(const char *filepath)
{
	int			fd;
	t_uvector	buf;
	ssize_t		ret;

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
	return (buf.data);
}

static void		format_print(
	char *hash_string, const char *src, t_bool reverse, t_bool quiet)
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
	free(hash_string);
}

static void		process_flags(int *i,
	t_hash_function *hash, char **argv, t_flags *f)
{
	char	*tmp;

	if (!ft_strcmp("-r", argv[*i]))
		*f = (t_flags){f->quiet, TRUE, TRUE};
	else if (!ft_strcmp("-q", argv[*i]))
		*f = (t_flags){TRUE, f->reverse, TRUE};
	else if (!ft_strcmp("-p", argv[*i]))
	{
		tmp = (char*)read_file(NULL);
		ft_putstr(tmp);
		format_print(hash((uint8_t*)tmp), NULL, FALSE, TRUE);
		free(tmp);
		f->read_stdin = FALSE;
	}
	else if (!ft_strcmp("-s", argv[*i]))
	{
		*i += 1;
		if (!argv[*i])
			ft_fprintf(stderr, "ft_ssl: %s: -s: Requires argument.\n%s\n",
				argv[1], USAGE);
		tmp = ft_strf("\"%s\"", argv[*i]);
		format_print(hash((uint8_t*)argv[*i]), tmp, f->reverse, f->quiet);
		free(tmp);
		f->read_stdin = FALSE;
	}
}

static void		process_arguments(t_hash_function *hash, int argc, char **argv)
{
	int		i;
	t_flags f;
	uint8_t	*data;

	i = 1;
	f = (t_flags){FALSE, FALSE, TRUE};
	while (++i < argc && argv[i][0] == '-')
		process_flags(&i, hash, argv, &f);
	while (i < argc)
	{
		if ((data = read_file(argv[i])))
		{
			format_print(hash(data), argv[i], f.reverse, f.quiet);
			f.read_stdin = FALSE;
			free(data);
		}
		else
			ft_fprintf(stderr, "ft_ssl: %s: %s: No such file or directory\n",
				argv[1], argv[i]);
		i += 1;
	}
	if (!f.read_stdin)
		return ;
	format_print(hash((data = read_file(NULL))), NULL, f.reverse, TRUE);
	free(data);
}

int				main(int argc, char **argv)
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
		process_arguments(hash, argc, argv);
	else
	{
		ft_fprintf(stderr, "ft_ssl: '%s' is an invalid command.\n", argv[1]);
		return (1);
	}
}
