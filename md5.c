/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 13:40:45 by lkaser            #+#    #+#             */
/*   Updated: 2019/03/04 13:40:46 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "libft.h"
#include "ft_ssl.h"

/*
** MD5 left rotate shift constants.
*/

static uint32_t	g_md5_shift[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

/*
** MD5 round constants.
*/

static uint32_t	g_md5_round[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

#define A (state[0])
#define B (state[1])
#define C (state[2])
#define D (state[3])

#define A_ (state[4])
#define B_ (state[5])
#define C_ (state[6])
#define D_ (state[7])

uint32_t		*md5_pad(const char *data, uint64_t data_len)
{
	uint32_t	*m;
	uint64_t	len;
	uint64_t	pad_len;

	len = ft_strlen(data);
	pad_len = ((len + 72) / 64) * 64;
	m = ft_memalloc(pad_len);
	ft_memcpy(m, data, len);
	((uint8_t*)m)[len] = 0x80;
	data_len *= 8;
	m[pad_len / 4 - 2] = data_len;
	m[pad_len / 4 - 1] = (data_len & 0xffffffff00000000) >> 32;
	return ((uint32_t*)m);
}

void			ft_md5_transform(uint32_t state[8], const uint32_t *chunks)
{
	uint32_t	f;
	uint32_t	g;
	uint32_t	i;

	ft_memcpy(state, state + 4, sizeof(uint32_t) * 4);
	i = 0;
	while (i < 64)
	{
		if (i < 16)
		{
			f = (B & C) | ((~B) & D);
			g = i;
		}
		else if (i < 32)
		{
			f = (D & B) | ((~D) & C);
			g = (5 * i + 1) % 16;
		}
		else if (i < 48)
		{
			f = B ^ C ^ D;
			g = (3 * i + 5) % 16;
		}
		else
		{
			f = C ^ (B | (~D));
			g = (7 * i) % 16;
		}
		f = f + A + g_md5_round[i] + chunks[g];
		A = D;
		D = C;
		C = B;
		B += ROTATE_LEFT(f, g_md5_shift[i]);
		++i;
	}
	A_ += A;
	B_ += B;
	C_ += C;
	D_ += D;
}

char			*ft_md5(const char *data)
{
	uint64_t	len;
	uint32_t	state[8];
	uint32_t	*padded;

	len = ft_strlen(data);
	A_ = 0x67452301;
	B_ = 0xefcdab89;
	C_ = 0x98badcfe;
	D_ = 0x10325476;
	if (len < 56)
	{
		padded = md5_pad(data, len);
		ft_md5_transform(state, padded);
		free(padded);
	}
	return (bytes_to_hex((uint8_t*)(state + 4), 16));
}

#undef A
#undef B
#undef C
#undef D
#undef A_
#undef B_
#undef C_
#undef D_
