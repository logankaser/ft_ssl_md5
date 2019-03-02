#include <stdint.h>
#include "libft.h"
#include "ft_ssl.h"

/*
** Converts bytes into a hexidecimal encoded string.
*/

char	*bytes_to_hex(const uint8_t *bytes, size_t size)
{
	static const char digits[16] = "0123456789abcdef";
	char	*out;
	size_t	i;
	size_t	j;

	NULL_GUARD(out = malloc(size * 2 + 1));
	i = 0;
	j = 0;
	while(i < size)
	{
		out[j++] = digits[(bytes[i] / 16) % 16];
		out[j++] = digits[bytes[i] % 16];
		++i;
	}
	out[j] = '\0';
	return (out);
}