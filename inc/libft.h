/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneelix <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/06 17:50:40 by vneelix           #+#    #+#             */
/*   Updated: 2019/09/12 17:11:10 by vneelix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# define BUFF_SIZE 4096

void*			ft_memset(void *s, int c, size_t n);
void			ft_bzero(void *nptr, size_t n);
void*			ft_memcpy(void *dest, const void *src, size_t n);
void*			ft_memalloc(size_t n);
void*			ft_memccpy(void *dest, const void *src, int c, size_t n);
void*			ft_memmove(void *dest, const void *src, size_t n);
void*			ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
size_t			ft_strlen(const char *s);
char*			ft_strdup(const char *s1);
char*			ft_strndup(const char *s1, size_t n);
char*			ft_strcpy(char *dst, const char *src);
char*			ft_strncpy(char *dest, const char *src, size_t n);
char*			ft_strcat(char *dest, const char *src);
char*			ft_strncat(char *dest, const char *src, size_t n);
char*			ft_strstr(const char *haystack, const char *needle);
char*			ft_strnstr
				(const char *haystack, const char *needle, size_t n);
char*			ft_strchr(const char* s, int c);
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_atoi(const char *nptr);
char*			ft_itoa(int n);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_toupper(int c);
int				ft_tolower(int c);
char*			ft_strjoin(char const *s1, char const *s2);
char**			ft_strsplit(const char*	s, char c);
int				ft_putchar(int c);
ssize_t			ft_putstr(char const *s);
ssize_t			ft_putendl(const char *s);
ssize_t			ft_putnbr(int n);
int				ft_putchar_fd(int c, int fd);
ssize_t			ft_putstr_fd(char const *s, int fd);
ssize_t			ft_putendl_fd(const char *s, int fd);
ssize_t			ft_putnbr_fd(int n, int fd);
char*			ft_getfile(int fd);
#endif
