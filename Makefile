NAME	=	miniRT
CC		=	gcc
FLAGS	=	#-Wall -Wextra -Werror
RM		=	rm -rf
LIBFT	=	lib/libft/libft.a
MLX		=	lib/mlx/libmlx.a
GNL		=	lib/gnl

SRC_DIR	=	src
SRCS	=	$(SRC_DIR)/main.c \
			$(SRC_DIR)/parse/utils.c \
			$(SRC_DIR)/parse/parser.c \
			$(SRC_DIR)/parse/assign_features.c \
			$(SRC_DIR)/parse/control.c \
			$(SRC_DIR)/parse/init_elements.c \
			$(SRC_DIR)/parse/init_objects.c \
			$(SRC_DIR)/parse/ft_atod.c\
			$(SRC_DIR)/parse/get_next_line.c \
			$(SRC_DIR)/parse/get_next_line_utils.c \
			$(SRC_DIR)/render/color.c \
			$(SRC_DIR)/render/get_normal.c \
			$(SRC_DIR)/render/intersections_cy.c \
			$(SRC_DIR)/render/intersections.c \
			$(SRC_DIR)/render/mlx_utils.c \
			$(SRC_DIR)/render/render_utils.c \
			$(SRC_DIR)/render/test.c \
			$(SRC_DIR)/render/vector2.c \
			$(SRC_DIR)/render/vector.c \

OBJ_DIR	=	obj
OBJS	=	$(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all:$(MLX) $(NAME)

$(NAME):$(MLX) $(OBJ_DIR) $(OBJS)
	@make -C lib/libft
	$(CC) $(FLAGS) $(OBJS) $(LIBFT) -L./lib/mlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)
	@echo "****   minirt ok!    ****"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/parse
	@mkdir -p $(OBJ_DIR)/render

$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.c
	$(CC) $(FLAGS) -c $< -o $@

$(MLX):
	make -C lib/mlx/
$(LIBFT):
	make -C lib/libft/

clean:
	make clean -C lib/mlx/
	make clean -C lib/libft/
	${RM} $(OBJ_DIR)

fclean: clean
	${RM} ${NAME}
	${RM} $(MLX)
	${RM} $(LIBFT)

re:			fclean all

.PHONY: all clean fclean re
