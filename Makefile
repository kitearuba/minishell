# **************************************************************************** #
#                              Project Information                             #
# **************************************************************************** #
NAME        = minishell

# **************************************************************************** #
#                            Compiler and Flags                                #
# **************************************************************************** #
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -MMD
MAKE        = make

# **************************************************************************** #
#                              Directories                                     #
# **************************************************************************** #

SRC_DIR         = src
LIBFT_DIR       = libft
INC_DIR         = include

# **************************************************************************** #
#                      File Paths and Dependencies                             #
# **************************************************************************** #

MAKEFILE        = Makefile
HEADER          = $(INC_DIR)/minishell.h $(INC_DIR)/buildin.c
LIBFT_A         = $(LIBFT_DIR)/libft.a
LIBFT_H         = $(LIBFT_DIR)/$(INC_DIR)/libft.h
LIBFT_MAKEFILE  = $(LIBFT_DIR)/$(MAKEFILE)
DEPS            = $(HEADER) $(MAKEFILE)

# **************************************************************************** #
#                          Source Files and Objects                            #
# **************************************************************************** #
SRC = 	minishell.c \
		$(SRC_DIR)/initiation/tokenize.c \
		$(SRC_DIR)/initiation/tokenizer_utils.c \
		$(SRC_DIR)/initiation/parse_tokens.c \
		$(SRC_DIR)/initiation/parse_utils.c \
		$(SRC_DIR)/initiation/parser_checks.c \
		$(SRC_DIR)/initiation/last_token_is_pipe.c \
		$(SRC_DIR)/expand/expand.c \
		$(SRC_DIR)/expand/expand_utils.c \
		$(SRC_DIR)/env/ft_getenv.c \
		$(SRC_DIR)/utils/free_2d_array.c \
		$(SRC_DIR)/utils/free_tokens.c \
		$(SRC_DIR)/utils/free_commands.c \
		$(SRC_DIR)/utils/free_all_and_exit.c \
		$(SRC_DIR)/cmd/built_in.c \
		$(SRC_DIR)/cmd/execute_commands.c \
		$(SRC_DIR)/executor/get_cmd_path.c \
		$(SRC_DIR)/executor/execve.c \
		$(SRC_DIR)/executor/run_external_cmd.c \
		$(SRC_DIR)/executor/redirection.c \
		$(SRC_DIR)/executor/heredoc.c \
		$(SRC_DIR)/executor/pipes/execute_pipeline.c \
		$(SRC_DIR)/executor/pipes/create_pipe.c \
        $(SRC_DIR)/builtin/ft_echo.c \
        $(SRC_DIR)/builtin/ft_cd.c \
        $(SRC_DIR)/builtin/ft_pwd.c \
        $(SRC_DIR)/builtin/ft_export.c \
        $(SRC_DIR)/builtin/ft_export_utils.c \
        $(SRC_DIR)/builtin/ft_unset.c \
        $(SRC_DIR)/builtin/ft_env.c \
        $(SRC_DIR)/builtin/ft_exit.c \
        $(SRC_DIR)/builtin/builtin_utils.c \
        $(SRC_DIR)/signal/signal.c \
        $(SRC_DIR)/tests/print.c


OBJ         = $(SRC:.c=.o)
DEP         = $(OBJ:.o=.d)

# **************************************************************************** #
#                                Headers                                       #
# **************************************************************************** #
INCLUDES    = -I$(INC_DIR) -I$(LIBFT_DIR)

# **************************************************************************** #
#                                Rules                                         #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_A) $(INCLUDES) -lreadline -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule to rebuild libft.a
$(LIBFT_A): $(LIBFT_MAKEFILE) $(LIBFT_SRC) $(LIBFT_H)
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJ) $(DEP)
	$(MAKE) -C $(LIBFT_DIR)  clean

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT_A)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re bonus
