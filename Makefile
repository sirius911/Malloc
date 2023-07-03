# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clorin <clorin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/05 19:07:11 by clorin            #+#    #+#              #
#    Updated: 2023/07/03 11:22:33 by clorin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# If the first argument is "run"...
ifeq (correction,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "test"
  TEST_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(TEST_ARGS):;@:)
endif

TEST_SRCS := test/correction/test$(TEST_ARGS).c

NAME = 		libft_malloc_$(HOSTTYPE).so
LIB_NAME = 	libft_malloc.so
FLAGS = 	-Wall -Wextra -Werror -fPIC
INCLUDES = 	-I ./includes

SRCS = 	srcs/global.c \
		srcs/malloc.c \
		srcs/ft_bzero.c \
		srcs/show_alloc_mem.c \
		srcs/ft_putnbr_fd.c \
		srcs/ft_putstr.c \
		srcs/ft_putstr_fd.c \
		srcs/ft_strlen.c \
		srcs/ft_memset.c \
		srcs/ft_memcpy.c \
		srcs/ft_memmove.c \
		srcs/blocks.c \
		srcs/block_merge.c \
		srcs/heap.c \
		srcs/pointer.c \
		srcs/free.c \
		srcs/log.c \
		srcs/calloc.c \
		srcs/realloc.c \
		srcs/show_alloc_mem_hex.c \


OBJ = 		$(SRCS:.c=.o)

CC = 		gcc $(FLAGS) $(INCLUDES)

%.o:		%.c
			$(CC) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) -shared -o $(NAME) $(OBJ) 
		ln -sf $(NAME) $(LIB_NAME)
		@echo "Compilation done"

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME) $(LIB_NAME) main_test

correction:
		@echo -n "Compile $(TEST_SRCS)"
		@gcc -o main_test $(TEST_SRCS) $(SRCS) $(INCLUDES)
		@echo " ... Done"
		./main_test

test:
		@echo -n "Compile $(TEST_SRCS)"
		@gcc -o main_test test/main.c $(SRCS) $(INCLUDES)
		@echo " ... Done"
		./main_test

re:		fclean all

.PHONY: all clean fclean re test correction