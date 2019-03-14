/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha1_256.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkaser <lkaser@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 13:40:45 by lkaser            #+#    #+#             */
/*   Updated: 2019/03/04 17:49:38 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "libft.h"
#include "ft_ssl.h"

/*
** SHA round constants.
*/

static uint32_t	g_sha_round[80] = {
	0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
	0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
	0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
	0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
	0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
	0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
	0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
	0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
	0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
	0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
	0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
	0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
	0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
	0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
	0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
	0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6
};

#define A (state[0])
#define B (state[1])
#define C (state[2])
#define D (state[3])
#define E (state[4])

#define A_ (state[5])
#define B_ (state[6])
#define C_ (state[7])
#define D_ (state[8])
#define E_ (state[9])

static uint8_t	*sha_pad(const uint8_t *data, uint64_t *data_len)
{
	uint8_t		*m;
	uint64_t	len;
	uint64_t	pad_len;

	len = ft_strlen((char*)data);
	pad_len = ((len + 72) / 64) * 64;
	m = ft_memalloc(pad_len);
	ft_memcpy(m, data, len);
	m[len] = 0x80;
	*data_len = pad_len;
	len *= 8;
	m[--pad_len] = len;
	m[--pad_len] = len >> 8;
	m[--pad_len] = len >> 16;
	m[--pad_len] = len >> 24;
	m[--pad_len] = len >> 32;
	m[--pad_len] = len >> 40;
	m[--pad_len] = len >> 48;
	m[--pad_len] = len >> 56;
	return (m);
}

static void		sha_mix(uint32_t state[10], uint32_t m[80])
{
	uint32_t	f;
	uint32_t	i;

	i = 0;
	while (i < 80)
	{
		if (i < 20)
			f = (B & C) | ((~B) & D);
		else if (i < 40)
			f = B ^ C ^ D;
		else if (i < 60)
			f = (B & C) | (B & D) | (C & D);
		else
			f = B ^ C ^ D;
		f = ROTATE_LEFT(A, 5) + f + E + g_sha_round[i] + m[i];
		E = D;
		D = C;
		C = ROTATE_LEFT(B, 30);
		B = A;
		A = f;
		++i;
	}
}

static void		sha_transform(uint32_t state[10], const uint8_t *d)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	m[80];

	j = 0;
	i = 0;
	while (i < 16)
	{
		m[i] = (d[j] << 24) | (d[j + 1] << 16) | (d[j + 2] << 8) | (d[j + 3]);
		++i;
		j += 4;
	}
	while (i < 80)
	{
		m[i] = ROTATE_LEFT(m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16], 1);
		++i;
	}
	ft_memcpy(state, state + 5, sizeof(uint32_t) * 5);
	sha_mix(state, m);
	A_ += A;
	B_ += B;
	C_ += C;
	D_ += D;
	E_ += E;
}

static char		*hash_little_endian(const uint32_t state[10], uint8_t hash[20])
{
	uint32_t i;

	i = 0;
	while (i < 4)
	{
		hash[i] = (A_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (B_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (C_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (D_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (E_ >> (24 - i * 8)) & 0x000000ff;
		++i;
	}
	return (bytes_to_hex(hash, 20));
}

char			*ft_sha(const uint8_t *raw_data)
{
	uint8_t		hash[20];
	uint32_t	state[10];
	uint8_t		*data;
	uint64_t	len;
	uint64_t	i;

	A_ = 0x67452301;
	B_ = 0xefcdab89;
	C_ = 0x98badcfe;
	D_ = 0x10325476;
	E_ = 0xc3d2e1f0;
	data = sha_pad(raw_data, &len);
	i = 0;
	while (i < len)
	{
		sha_transform(state, data + i);
		i += 64;
	}
	free(data);
	return (hash_little_endian(state, hash));
}

#undef A
#undef B
#undef C
#undef D
#undef A_
#undef B_
#undef C_
#undef D_
