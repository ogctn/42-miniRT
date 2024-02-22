rm -rf ./test.out

gcc 		./src/test.c \
			./src/test_mlx.c \
			./src/vector.c \
	 ./minilibx_opengl_20191021/libmlx.a \
	 -I./minilibx_opengl_2019102 \
	 -framework Appkit -framework OpenGL \
	 -fsanitize=address \
	 -o ./test.out
	
./test.out
