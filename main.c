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

/*
** SHA2 round constants
** First 32 bits of the fractional parts of the cube roots of primes 2..311
** In Big-Endian.
*/

static uint32_t g_sha2_round[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/*
** Binary right and left rotation operations. Unsigned types only.
*/

#define RIGHT_ROTATE(x, c) (((x) >> c) | ((x) << ((sizeof(x) * 8) - c)))
#define LEFT_ROTATE(x, c) (((x) << c) | ((x) >> ((sizeof(x) * 8) - c)))

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

#define A (state[0])
#define B (state[1])
#define C (state[2])
#define D (state[3])

#define A_ (state[4])
#define B_ (state[5])
#define C_ (state[6])
#define D_ (state[7])

/*
** As you can see hash funcitons are not very norm compatible
** TODO: handle more than 448 bits.
*/

/*
** Fills two bytes at a time.
*/

void			hex_byte(uint8_t byte, char *fill)
{
	static const char digits[16] = "0123456789abcdef";

	fill[1] = digits[byte % 16];
	byte /= 16;
	fill[0] = digits[byte % 16];
}

char			*ft_md5(const char *data)
{
	uint64_t	len;
	uint32_t	state[8];
	char		*out;

	len = ft_strlen(data);
	A_ = 0x67452301;
	B_ = 0xefcdab89;
	C_ = 0x98badcfe;
	D_ = 0x10325476;
	if (len < 56)
	{
		ft_md5_transform(state, md5_pad(data, len));
	}
	out = ft_memalloc(33);
	len = 0;
	while (len < 32)
	{
		hex_byte(((uint8_t*)(state + 4))[len / 2], out + len);
		len += 2;
	}
	return (out);
}

void			ft_md5_transform(uint32_t state[8], const uint32_t *chunks)
{
	uint32_t	f;
	uint32_t	g;
	uint32_t	i;

	ft_memcpy(state, state + 4, 16);
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
		B += LEFT_ROTATE(f, g_md5_shift[i]);
		++i;
	}
	A_ += A;
	B_ += B;
	C_ += C;
	D_ += D;
}

uint32_t		*sha2_pad(const char *data)
{
	uint32_t	*m;
	uint64_t	i;
	uint64_t	len;
	uint64_t	pad_len;

	len = ft_strlen(data);
	pad_len = ((len + 72) / 64) * 64;
	m = ft_memalloc(pad_len);
	ft_memcpy(m, data, len);
	i = len;
	((uint8_t*)m)[i++] = 0x80;
	len *= 8;
	((uint8_t*)m)[i++] = len;
	((uint8_t*)m)[i++] = len >> 8;
	((uint8_t*)m)[i++] = len >> 16;
	((uint8_t*)m)[i++] = len >> 24;
	((uint8_t*)m)[i++] = len >> 32;
	((uint8_t*)m)[i++] = len >> 40;
	((uint8_t*)m)[i++] = len >> 48;
	((uint8_t*)m)[i++] = len >> 56;
	return (m);
}

char			*ft_sha2_256(const char *data)
{
	(void)data;
	(void)g_sha2_round;
	return (NULL);
}

int				main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	printf("md5: %s\n", ft_md5(argv[1]));
	printf("sha2_256: %s\n", ft_sha2_256(argv[1]));
	return (0);
}
