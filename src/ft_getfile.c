#include "rtv1.h"

size_t		ft_strlen(const char *string)
{
	size_t	i;

	i = 0;
	while (string[i])
		i += 1;
	return (i);
}

void		ft_bzero(void *memory, size_t size)
{
	size_t	i;

	i = size % 4;
	while (i)
	{
		((int8_t*)memory)[size - i] = 0;
		i -= 1;
	}
	i = 0;
	while (i < size / 4)
	{
		((int32_t*)memory)[i] = 0;
		i += 1;
	}
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;
	size_t	len2;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if ((__uint128_t)len + (__uint128_t)len2 >= SIZE_MAX)
		return (NULL);
	if ((str = (char*)malloc(sizeof(char) * (len + len2 + 1))) == NULL)
		return (NULL);
	ft_bzero((void*)(str), len + len2 + 1);
	ft_memcpy((void*)(str), (void*)(s1), len);
	ft_memcpy((void*)(str + len), (void*)(s2), len2);
	return (str);
}

char		*ft_getfile(int fd)
{
	long	i;
	char	buff[4096 + 1];
	char	*str;
	void	*ptr;

	if ((str = (char*)malloc(sizeof(char))) == NULL)
		return (NULL);
	ft_bzero(str, 1);
	ft_bzero(buff, 4096 + 1);
	while ((i = read(fd, buff, 4096)) > 0)
	{
		if (i == 0)
			break ;
		ptr = str;
		str = ft_strjoin(str, buff);
		free(ptr);
		if (str == NULL)
			return (NULL);
		ft_bzero(buff, 4096 + 1);
	}
	if (i == (-1) || str[0] == '\0')
	{
		free(str);
		return (NULL);
	}
	return (str);
}