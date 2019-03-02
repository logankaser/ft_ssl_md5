#include <stdint.h>
#include "libft.h"
#include "ft_ssl.h"

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

uint8_t		*sha2_pad(const char *data, uint64_t data_len)
{
	uint8_t		*m;
	uint64_t	i;
	uint64_t	len;
	uint64_t	pad_len;

	len = ft_strlen(data);
	pad_len = ((len + 72) / 64) * 64;
	m = ft_memalloc(pad_len);
	ft_memcpy(m, data, len);
	i = len;
	m[i++] = 0x80;
	data_len *= 8;

	m[63] = data_len;
	m[62] = data_len >> 8;
	m[61] = data_len >> 16;
	m[60] = data_len >> 24;
	m[59] = data_len >> 32;
	m[58] = data_len >> 40;
	m[57] = data_len >> 48;
	m[56] = data_len >> 56;

	// for (int i = 0; i < 64; ++i)
	// {
	// 	printf("%08X", m[i]);
	// }
	// printf("\n");

	return (m);
}

#define A (state[0])
#define B (state[1])
#define C (state[2])
#define D (state[3])
#define E (state[4])
#define F (state[5])
#define G (state[6])
#define H (state[7])

#define A_ (state[8])
#define B_ (state[9])
#define C_ (state[10])
#define D_ (state[11])
#define E_ (state[12])
#define F_ (state[13])
#define G_ (state[14])
#define H_ (state[15])

#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTATE_RIGHT(x, 2) ^ ROTATE_RIGHT(x, 13) ^ ROTATE_RIGHT(x, 22))
#define EP1(x) (ROTATE_RIGHT(x, 6) ^ ROTATE_RIGHT(x, 11) ^ ROTATE_RIGHT(x, 25))
#define SIG0(x) (ROTATE_RIGHT(x, 7) ^ ROTATE_RIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTATE_RIGHT(x, 17) ^ ROTATE_RIGHT(x, 19) ^ ((x) >> 10))

void			ft_sha2_256_transform(uint32_t state[16], const uint8_t *data)
{
	uint32_t temp1;
	uint32_t temp2;
	uint32_t i;
	uint32_t j;
	uint32_t m[64];

	/*
	** Copy with endianess swap.
	*/
	i = 0;
	j = 0;
	while (i < 16)
	{
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
		++i;
		j += 4;
	}
	
	/*
	** Extend 0-16 to 0-64.
	*/
	while (i < 64)
	{
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
		++i;
	}

	ft_memcpy(state, state + 8, sizeof(uint32_t) * 8);

	/*
	** Compression loop
	*/
	i = 0;
	while (i < 64) 
	{
		temp1 = H + EP1(E) + CH(E, F, G) + g_sha2_round[i] + m[i];
		temp2 = EP0(A) + MAJ(A, B, C);
		H = G;
		G = F;
		F = E;
		E = D + temp1;
		D = C;
		C = B;
		B = A;
		A = temp1 + temp2;
		++i;
	}
	A_ += A;
	B_ += B;
	C_ += C;
	D_ += D;
	E_ += E;
	F_ += F;
	G_ += G;
	H_ += H;
}

char			*ft_sha2_256(const char *data)
{
	uint64_t	len;
	uint32_t	i;
	uint8_t		*padded;
	uint32_t	state[16];
	uint8_t		hash[32];


	len = ft_strlen(data);
	A_ = 0x6a09e667;
	B_ = 0xbb67ae85;
	C_ = 0x3c6ef372;
	D_ = 0xa54ff53a;
	E_ = 0x510e527f;
	F_ = 0x9b05688c;
	G_ = 0x1f83d9ab;
	H_ = 0x5be0cd19;
	if (len < 56)
	{
		padded = sha2_pad(data, len);
		ft_sha2_256_transform(state, padded);
		free(padded);
	}

	/*
	** Endian swap to to get ready to output.
	*/
	i = 0;
	while (i < 4) 
	{
		hash[i] = (A_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (B_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (C_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (D_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (E_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (F_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (G_ >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (H_ >> (24 - i * 8)) & 0x000000ff;
		++i;
	}

	return (bytes_to_hex(hash, 32));
}

#undef A
#undef B
#undef C
#undef D
#undef E
#undef F
#undef G
#undef H
#undef A_
#undef B_
#undef C_
#undef D_
#undef E_
#undef F_
#undef G_
#undef H_