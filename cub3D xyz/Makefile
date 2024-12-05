NAME = cub3D
SRCS = parse/parse1.c parse/map_check.c parse/before_map.c parse/parser.c
LIBFT_SRCS = parse/libft/libft.a
GNL_SRCS = parse/gnl/get_next_line.c parse/gnl/get_next_line_utils.c

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(SRCS) $(LIBFT_SRCS) $(GNL_SRCS)
	$(CC) $(CFLAGS) -o $(NAME) $(SRCS) $(LIBFT_SRCS) $(GNL_SRCS)

$(LIBFT_SRCS):
	make -C parse/libft

clean:
	rm -f $(NAME)

fclean: clean
	rm -f ../libft/libft.a

re: fclean all

.PHONY: all clean fclean re
