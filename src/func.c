#include "rtv1.h"

size_t	ft_splits(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0' && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'))
			i += 1;
	return (i);
}

size_t	ft_number(char *s)
{
	size_t	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i += 1;
	while (ft_isdigit(s[i]))
	{
		i += 1;
		if (s[i] == '.')
			i += 1;
	}
	return (i);
}
