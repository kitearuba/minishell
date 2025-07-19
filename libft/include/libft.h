/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 11:36:52 by chrrodri          #+#    #+#             */
/*   Updated: 2025/01/31 21:48:03 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/* ************************************************************************** */
/*                               Includes                                     */
/* ************************************************************************** */

# include <stddef.h>    /* For size_t */
# include <stdlib.h>    /* For malloc(), free(), exit() */
# include <unistd.h>    /* For write(), read(), close() */
# include <stdarg.h>    /* For variadic functions */

/* ************************************************************************** */
/*                                Macros                                      */
/* ************************************************************************** */

# define FT_STDOUT 1
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif // BUFFER_SIZE
# define MAX_INT 2147483647
# define MIN_INT -2147483648
# define MAX_LONG 9223372036854775807L
# define LLONG_MAX 9223372036854775807LL
# define LLONG_MIN (-9223372036854775807LL - 1)

/* ************************************************************************** */
/*                           Function Prototypes                              */
/* ************************************************************************** */

/* ----------------------------- Libc Functions ----------------------------- */

/* Character checks and conversions */
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_toupper(int c);
int		ft_tolower(int c);

/* String manipulation */
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s1);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);

/* Memory manipulation */
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);

/* Number conversion and memory allocation */
int		ft_atoi(const char *str);
void	*ft_calloc(size_t count, size_t size);
int		ft_atoi_base(const char *str, int base);
long	ft_strtol(const char *str, char **endptr, int base);
long long	ft_atoll(const char *str);

/* --------------------------- Additional Functions ------------------------- */

/* String creation and modification */
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));

/* File descriptor manipulation */
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

/* -------------------------- Linked List Functions ------------------------- */

/**
 * struct s_list - Represents a single linked list node.
 * @content: The data contained in the node.
 * @next: Pointer to the next node in the list.
 */
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **alst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **alst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* ---------------------- ft_printf and Helper Functions -------------------- */

int		ft_printf(const char *format, ...);
int		ft_printf_fd(int fd, const char *format, ...);
void	ft_putunbr(unsigned int n, int fd);
void	ft_puthex(unsigned long n, int uppercase, int fd);
int		handle_format(const char *format, va_list args, int fd);
int		print_format(const char *ptr, va_list args, int fd);
int		handle_char(va_list args, int fd);
int		handle_string(va_list args, int fd);
int		handle_pointer(va_list args, int fd);
int		handle_int(va_list args, int fd);
int		handle_uint(va_list args, int fd);
int		handle_hex(va_list args, int fd, int uppercase);
int		handle_percent(int fd);
int		get_int_length(int n);
int		get_uint_length(unsigned int n);
int		get_hex_length(unsigned long n);

/* ------------------------- Get Next Line Functions ------------------------ */

char	*get_next_line(int fd);
char	*get_next_line_bonus(int fd);
void	ft_strappend(char *dst, const char *src1, const char *src2, \
		size_t size);

#endif // LIBFT_H
