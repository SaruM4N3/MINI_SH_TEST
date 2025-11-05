CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -g3

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin) #macOS
	CFLAGS += -I/opt/homebrew/opt/readline/include
	LDFLAGS = -L/opt/homebrew/opt/readline/lib
endif

SRC = parsing/lexer/operator_handler.c \
      parsing/lexer/tokenizer.c \
      parsing/parser/command_builder.c \
	  parsing/parser/expander.c \
	  parsing/parser/expander_utils.c \
      parsing/parser/pipeline_handler.c \
      parsing/parser/redirect_handler.c \
	  parsing/parser/heredoc_handler.c \
	  parsing/lexer/tokenizer_utils.c \
	  parsing/parser/command_builder_utils.c \
	  signal/* \
	  exec/* \
	  env/* \
	  builtins/* \
	  test_minish.c \
      parsing/parsing.c \

HEADER = include/parsing.h

OBJ_DIR = obj
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)
NAME = minishell

all: $(NAME)

LIB_DIR = libft/libft
LIBFT = $(LIB_DIR)/libft.a

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) -lft $(LDFLAGS) -lreadline -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/parsing/lexer
	mkdir -p $(OBJ_DIR)/parsing/parser

$(LIBFT): force
	$(MAKE) -C $(LIB_DIR) bonus
	
force:

%.o: %.c $(HEADER) Makefile
	$(CC) $(CFLAGS) -O3 -c $< -o $@

$(OBJ_DIR)/%.o: %.c $(HEADER) Makefile | $(OBJ_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -O3 -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

.PHONY: all clean fclean re