#include <stdio.h>
#include <stdarg.h>
#include "ft_printf.h"

int	ft_fprintf(FILE *stream, const char *format, ...)
{
	t_uvector	s;
	va_list 	args;

	if (format)
	{
		ft_uvector_init(&s, 1);
		va_start(args, format);
		format_iter(&s, (char*)format, args);
		va_end(args);
		fwrite (s.data, sizeof(char), s.length, stream);
		free(s.data);
		return (s.length);
	}
	return (0);
}
