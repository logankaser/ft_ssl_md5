#ifndef FT_SSL_H
# define FT_SSL_H
# include <stdint.h>
# include <stddef.h>

/*
** Binary right and left rotation operations. Unsigned types only.
*/

#define ROTATE_RIGHT(x, c) (((x) >> c) | ((x) << ((sizeof(x) * 8) - c)))
#define ROTATE_LEFT(x, c) (((x) << c) | ((x) >> ((sizeof(x) * 8) - c)))

/*
** Simple interface.
*/
char			*ft_md5(const char *data);
char			*ft_sha2_256(const char *data);

/*
** Streaming interface.
*/

/*
** Utility functions.
*/
char	*bytes_to_hex(const uint8_t *bytes, size_t size);

#endif