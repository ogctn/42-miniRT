/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 01:00:44 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/23 02:16:13 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include "../lib/mlx/mlx.h"
# include "../lib/libft/libft.h"
# include "./get_next_line.h"
# include "./vector.h"
# include "./keys.h"

# define TOL 1E-4
# define INF INFINITY
# define SHIFT_VAL 2

# define WIDTH 480
# define HEIGHT 480
# define ASPECT_RATIO_X 4
# define ASPECT_RATIO_Y 4

# define COLOR_MIX_RATIO 0.55

typedef struct s_cy_val
{
	double	x1;
	double	x2;
	double	x3;
	int		x1_flag;
	int		x2_flag;
	int		x3_flag;
}	t_cy_val;

typedef struct s_cy_mtrc
{
	double	a;
	double	b;
	double	c;
	double	d;
}	t_cy_mtrc;

typedef struct s_cy_vec
{
	t_vec3		p1;
	t_vec3		p2;
	t_vec3		va;
	t_vec3		oc;
	t_cy_val	values;
	t_cy_mtrc	metrics;

}	t_cy_vec;

typedef struct s_mlx
{
	void	*mlx_p;
	void	*win_p;
	void	*img_p;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_mlx;

typedef struct s_roots
{
	double	t1;
	double	t2;
}	t_roots;

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	dir;
}	t_ray;

typedef enum e_type
{
	PLANE,
	SPHERE,
	CYLINDER,
}	t_type;

typedef struct s_color
{
	double	r;
	double	g;
	double	b;
}	t_color;

typedef struct s_sphere
{
	t_color	color;
	t_vec3	center;
	double	r;
}	t_sphere;

typedef struct s_plane
{
	t_color			color;
	t_vec3			normal;
	t_vec3			point;
}	t_plane;

typedef struct s_cylinder
{
	t_color				color;
	t_vec3				origin;
	t_vec3				dir;
	double				r;
	double				height;
}	t_cylinder;

typedef struct s_obj	t_obj;

typedef struct s_obj
{
	void			*obj;
	t_type			type;
	double			(*f_intersects)(const t_ray *, const t_obj *);
	t_color			(*f_get_color)(const t_obj *);
	t_vec3			(*f_get_normal)(const t_obj *, t_vec3 *);
}	t_obj;

typedef struct s_light
{
	t_vec3	origin;
	double	brightness;
	t_color	color;
}	t_light;

typedef struct s_ambient
{
	double			brightness;
	t_color			color;
}	t_ambient;

typedef struct s_cam
{
	t_vec3	origin;
	t_vec3	dir;
	double	fov;
}	t_cam;

typedef struct t_screen
{
	t_vec3	up;
	t_vec3	right;
	t_vec3	forward;
	t_vec3	origin;
	double	focal_length;
	double	aspect_ratio;
	double	x_pix_min;
	double	x_pix_max;

}	t_screen;

typedef struct s_shade_info
{
	t_color 	c_diffuse;
	t_color 	c_specular;
	t_color 	color_final;
	t_obj		*obj;
	t_color		base_color;
	t_vec3		light_origin;
	t_vec3		hit_point;
	t_vec3		point_to_light;
	t_vec3		point_to_light_dir;
	t_vec3		surface_normal;
	t_ray		ray;
	double		t;
	double 		theta;
}	t_shade_info;

typedef struct s_data
{
	int			fd;
	int			a;
	int			c;
	int			l;
	int			obj_count;
	t_mlx		*mlx;
	t_cam		*cam;
	t_screen	*screen;
	t_ambient	*ambient_light;
	t_light		*light;
	t_obj		*obj_set;
	t_cam		*default_cam;
}	t_data;

void	initialize(t_data *data, char *str);
void	init_fd(t_data *data, char *str);
void	close_fd(t_data *data);

int		init_ambient(t_data *data, char *line);
int		init_cam(t_data *data, char *line);
int		init_light(t_data *data, char *line);
int		init_obj(t_data *data, char *line, int type);

int		ft_assign_color(char **ptr, t_color *clr);
int		ft_assign_vec(char **ptr, t_vec3 *vec, int flag);

int		size_2d(char **ptr);
int		ft_strcmp(char *s1, char *s2);
void	free_2d(char **d);
double	ft_atod(const char *str);

int		control_rgb(t_color clr);
void	control_elements(t_data *data);
int		control_extension(char *str);

void	set_sphere(t_data *d, t_sphere *s, int i);
void	set_stuffs(t_data *d);
void	main_loop(t_data *d);
void	new_mlx(t_data *d);
int		handle_key(int keycode, t_data *d);

double	intersects_sphere(const t_ray *ray, const t_sphere *sp);
double	f_intersects(const t_ray *ray, const t_obj *obj);
t_color	f_get_color(const t_obj *obj);
t_vec3	f_get_normal(const t_obj *obj, t_vec3 *hit_point);
void	render(t_data *data);
void	cam_move(t_data *genel, int keycode);

void	assign_cy_vectors(const t_ray *ray, const t_cylinder *cy, t_cy_vec *d);
void	find_x1(const t_ray *ray, const t_cylinder *cy, t_cy_vec *data);
void	find_x2_x3(const t_ray *ray, const t_cylinder *cy, t_cy_vec *data);
double	find_min_2(double a, double b);
double	find_min_3(double a, double b, double c);

void	free_objects(t_data *genel);
int		free_exit(t_data *genel);

t_color	color_multiply(t_color *color, double factor);
void	compute_illumination(t_data *data, t_shade_info *si);
void	color_mix(t_color *color, t_color ambient, t_color light);
#endif
