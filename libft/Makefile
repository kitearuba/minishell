# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/13 11:12:18 by chrrodri          #+#    #+#              #
#    Updated: 2025/01/31 21:46:47 by chrrodri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = gcc

MAKEFILE = Makefile
HEADER = libft.h

CFLAGS = -Wall -Werror -Wextra
AR = ar rcs

# Paths
SRCS_DIR = src
INCLUDES = include/libft.h

# All source files (from libft, printf, get_next_line, and extra)
SRCS = $(SRCS_DIR)/ft_isalpha.c $(SRCS_DIR)/ft_isdigit.c \
       $(SRCS_DIR)/ft_isalnum.c $(SRCS_DIR)/ft_isascii.c \
       $(SRCS_DIR)/ft_isprint.c $(SRCS_DIR)/ft_strlen.c \
       $(SRCS_DIR)/ft_memset.c $(SRCS_DIR)/ft_bzero.c \
       $(SRCS_DIR)/ft_memcpy.c $(SRCS_DIR)/ft_memmove.c \
       $(SRCS_DIR)/ft_strlcpy.c $(SRCS_DIR)/ft_strlcat.c \
       $(SRCS_DIR)/ft_toupper.c $(SRCS_DIR)/ft_tolower.c \
       $(SRCS_DIR)/ft_strchr.c $(SRCS_DIR)/ft_strrchr.c \
       $(SRCS_DIR)/ft_strncmp.c $(SRCS_DIR)/ft_memchr.c \
       $(SRCS_DIR)/ft_memcmp.c $(SRCS_DIR)/ft_strnstr.c \
       $(SRCS_DIR)/ft_atoi.c $(SRCS_DIR)/ft_calloc.c \
       $(SRCS_DIR)/ft_strdup.c $(SRCS_DIR)/ft_substr.c \
       $(SRCS_DIR)/ft_strjoin.c $(SRCS_DIR)/ft_strtrim.c \
       $(SRCS_DIR)/ft_split.c $(SRCS_DIR)/ft_itoa.c \
       $(SRCS_DIR)/ft_strmapi.c $(SRCS_DIR)/ft_striteri.c \
       $(SRCS_DIR)/ft_putchar_fd.c $(SRCS_DIR)/ft_putstr_fd.c \
       $(SRCS_DIR)/ft_putendl_fd.c $(SRCS_DIR)/ft_putnbr_fd.c \
       $(SRCS_DIR)/ft_lstnew_bonus.c $(SRCS_DIR)/ft_lstadd_front_bonus.c \
       $(SRCS_DIR)/ft_lstsize_bonus.c $(SRCS_DIR)/ft_lstlast_bonus.c \
       $(SRCS_DIR)/ft_lstadd_back_bonus.c $(SRCS_DIR)/ft_lstdelone_bonus.c \
       $(SRCS_DIR)/ft_lstclear_bonus.c $(SRCS_DIR)/ft_lstiter_bonus.c \
       $(SRCS_DIR)/ft_lstmap_bonus.c $(SRCS_DIR)/ft_printf.c \
       $(SRCS_DIR)/print_format.c $(SRCS_DIR)/ft_putunbr.c \
       $(SRCS_DIR)/ft_puthex.c $(SRCS_DIR)/handle_char.c \
       $(SRCS_DIR)/handle_string.c $(SRCS_DIR)/handle_pointer.c \
       $(SRCS_DIR)/handle_int.c $(SRCS_DIR)/handle_uint.c \
       $(SRCS_DIR)/handle_hex.c $(SRCS_DIR)/handle_percent.c \
       $(SRCS_DIR)/utils.c $(SRCS_DIR)/get_next_line.c \
       $(SRCS_DIR)/get_next_line_bonus.c $(SRCS_DIR)/ft_strappend.c \
       $(SRCS_DIR)/ft_printf_fd.c $(SRCS_DIR)/ft_strcmp.c \
       $(SRCS_DIR)/ft_atoi_base.c $(SRCS_DIR)/ft_strtol.c \
       $(SRCS_DIR)/ft_atoll.c \

# Object files
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(NAME)

# Compile the library
$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

# Pattern rule to compile object files from source files. Dependencies include header and Makefile.
$(SRCS_DIR)/%.o: $(SRCS_DIR)/%.c $(INCLUDES) $(MAKEFILE)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# Ensure Make does not confuse these targets with files of the same name.
.PHONY: all clean fclean re
