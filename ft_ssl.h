/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 13:45:34 by lkaser            #+#    #+#             */
/*   Updated: 2019/03/04 13:45:36 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include <stdint.h>
# include <stddef.h>

/*
** Binary right and left rotation operations. Unsigned types only.
*/

# define ROTATE_RIGHT(x, c) (((x) >> c) | ((x) << ((sizeof(x) * 8) - c)))
# define ROTATE_LEFT(x, c) (((x) << c) | ((x) >> ((sizeof(x) * 8) - c)))

/*
** Interface.
*/
char			*ft_md5(const uint8_t *data);
char			*ft_sha2_256(const uint8_t *data);
char			*ft_sha(const uint8_t *data);

typedef char	*t_hash_function(const uint8_t *data);

/*
** Utility functions.
*/
char			*bytes_to_hex(const uint8_t *bytes, size_t size);

/*
** Command line flags.
*/

typedef struct	s_flags
{
	uint8_t		quiet : 1;
	uint8_t		reverse : 1;
	uint8_t		read_stdin : 1;
}				t_flags;

#endif
