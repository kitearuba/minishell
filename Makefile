# **************************************************************************** #
#                              Project Information                             #
# **************************************************************************** #
NAME            = minishell

# **************************************************************************** #
#                            Compiler and Flags                                #
# **************************************************************************** #
CC              = cc
CFLAGS          = -Wall -Wextra -Werror -MMD -MP
MAKE            = make
RM              ?= rm -f

# If you need extra flags for readline on your system, override this when calling make:
#   make READLINE_LIBS="-lreadline -lncurses"
READLINE_LIBS   ?= -lreadline

# **************************************************************************** #
#                              Directories                                     #
# **************************************************************************** #
SRC_DIR         = src
LIBFT_DIR       = libft
INC_DIR         = include
LIBFT_INC_DIR   = $(LIBFT_DIR)/include

# **************************************************************************** #
#                          File Paths and Deps                                 #
# **************************************************************************** #
MAKEFILE        = Makefile
HEADER          = $(INC_DIR)/minishell.h $(INC_DIR)/builtin.h $(INC_DIR)/struct.h
LIBFT_A         = $(LIBFT_DIR)/libft.a
LIBFT_H         = $(LIBFT_INC_DIR)/libft.h
DEPS            = $(HEADER) $(MAKEFILE)

# **************************************************************************** #
#                          Source Files and Objects                            #
# **************************************************************************** #
SRC = 	minishell.c \
		$(SRC_DIR)/core/init_minishell.c \
		$(SRC_DIR)/core/execute_minishell.c \
		$(SRC_DIR)/lexer/tokenize.c \
		$(SRC_DIR)/lexer/tokenizer_utils.c \
		$(SRC_DIR)/lexer/tokenizer_redirects.c \
		$(SRC_DIR)/parser/parse_tokens.c \
		$(SRC_DIR)/parser/handle_parse_redirection.c \
		$(SRC_DIR)/parser/parse_utils.c \
		$(SRC_DIR)/parser/parser_cmd.c \
		$(SRC_DIR)/parser/parser_cmd_utils.c \
		$(SRC_DIR)/parser/parser_checks.c \
		$(SRC_DIR)/parser/last_token_is_pipe.c \
		$(SRC_DIR)/expand/expand.c \
		$(SRC_DIR)/expand/expand_utils.c \
		$(SRC_DIR)/expand/expand_wildcard.c \
		$(SRC_DIR)/expand/wildcard_utils.c \
		$(SRC_DIR)/expand/wildcard_utils_2.c \
		$(SRC_DIR)/env/ft_getenv.c \
		$(SRC_DIR)/env/env_set.c \
		$(SRC_DIR)/env/export_noval.c \
		$(SRC_DIR)/utils/free_2d_array.c \
		$(SRC_DIR)/utils/free_tokens.c \
		$(SRC_DIR)/utils/free_commands.c \
		$(SRC_DIR)/utils/free_all_and_exit.c \
		$(SRC_DIR)/utils/input_utils.c \
		$(SRC_DIR)/cmd/built_in.c \
		$(SRC_DIR)/cmd/execute_commands.c \
		$(SRC_DIR)/execution/get_cmd_path.c \
		$(SRC_DIR)/execution/execve.c \
		$(SRC_DIR)/execution/run_external_cmd.c \
		$(SRC_DIR)/execution/run_external_cmd_utils.c \
		$(SRC_DIR)/execution/redirection.c \
		$(SRC_DIR)/execution/heredoc.c \
		$(SRC_DIR)/execution/heredoc_utils.c \
		$(SRC_DIR)/execution/pipes/execute_pipeline.c \
		$(SRC_DIR)/execution/pipes/create_pipe.c \
        $(SRC_DIR)/builtin/ft_echo.c \
        $(SRC_DIR)/builtin/ft_cd.c \
        $(SRC_DIR)/builtin/ft_pwd.c \
        $(SRC_DIR)/builtin/ft_export.c \
        $(SRC_DIR)/builtin/ft_export_utils.c \
        $(SRC_DIR)/builtin/export_view.c \
        $(SRC_DIR)/builtin/ft_unset.c \
        $(SRC_DIR)/builtin/ft_env.c \
        $(SRC_DIR)/builtin/ft_exit.c \
        $(SRC_DIR)/builtin/builtin_utils.c \
        $(SRC_DIR)/signal/signal.c

OBJ             = $(SRC:.c=.o)
DEP             = $(OBJ:.o=.d)

# **************************************************************************** #
#                                Headers                                       #
# **************************************************************************** #
INCLUDES        = -I$(INC_DIR) -I$(LIBFT_INC_DIR)

# **************************************************************************** #
#                                Rules                                         #
# **************************************************************************** #
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_A) $(INCLUDES) $(READLINE_LIBS) -o $(NAME)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# --- libft: always ask libft to build itself; it will only recompile what changed
libft:
	$(MAKE) -C $(LIBFT_DIR)

$(LIBFT_A): libft
	@true

clean:
	$(RM) $(OBJ) $(DEP)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re libft
