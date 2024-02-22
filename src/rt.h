#ifndef RT_H
# define RT_H

#include "keys.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vector.h"
#include "../minilibx_opengl_20191021/mlx.h"

#include <stdio.h>

#define TOL 1E-4
#define INF INFINITY
#define SHIFT_VAL 2

#define V_W 800
#define V_H 600
#define V_D 1
#define C_W 200
#define C_H 200


typedef struct	s_roots {
	double t1;
	double t2;
} t_roots;


typedef struct	s_ray {
	t_vec3	origin;
	t_vec3	dir;
} t_ray;

typedef struct	s_cam {
	t_vec3	origin;
	t_vec3	dir;
	double	fov;
	t_ray	*ray;
} t_cam;


typedef enum {
	PLANE,
	SPHERE,
	CYLINDER,
	TRIANGLE,
} e_type;


typedef struct s_color {
	int r;
	int g;
	int b;
}	t_color;


typedef struct s_sphere {
	t_vec3			center;
	double			r;
	t_color			color;
	struct s_sphere	*next;
} t_sphere;



typedef struct s_img {
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_mlx {
	void	*mlx_p;
	void	*win_p;
	t_img	img;

}	t_mlx;

typedef struct s_viewport {
	double	vw;
	double	vh;
	double	d;

}	t_viewport;

typedef struct s_objs {
	t_sphere	*sp;
} t_objs;

typedef struct s_general {
	t_mlx	*mlx;
	t_cam	*cam;

	t_objs	*objs;
}	t_general;



#endif
