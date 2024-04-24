/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 01:00:55 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/24 21:08:20 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"
#include <stdio.h>

void	mlx_stuffs(t_data *a);
void	my_mlx_pixel_put(t_mlx *a, int b, int c, int d);
void	render_background( t_mlx *a, int b);
void	render(t_data *data);
int		rgb_to_int(const t_color *rgb);
void	init_viewport(t_data *d);

void	reset_camera(t_data *d)
{
	d->cam->origin = d->default_cam->origin;
	d->cam->dir = d->default_cam->dir;
	d->cam->fov = d->default_cam->fov;
}

void	change_cam_dir(t_data *d, int keycode)
{
	t_vec3	tmp;
	double	d_theta;

	if (keycode == KEY_UP || keycode == KEY_DOWN
		|| keycode == KEY_LEFT || keycode == KEY_RIGHT)
	{
		tmp = (t_vec3){0, 0, 0};
		d_theta = 0.1;
		if (keycode == KEY_UP)
			tmp = v_rotate(&d->cam->dir, &d->screen->right, -d_theta);
		else if (keycode == KEY_DOWN)
			tmp = v_rotate(&d->cam->dir, &d->screen->right, d_theta);
		else if (keycode == KEY_LEFT)
			tmp = v_rotate(&d->cam->dir, &d->screen->up, -d_theta);
		else if (keycode == KEY_RIGHT)
			tmp = v_rotate(&d->cam->dir, &d->screen->up, d_theta);
		d->cam->dir = v_normalize(&tmp);
		init_viewport(d);
	}
}

void	cam_move(t_data *d, int keycode)
{
	t_vec3	tmp;

	tmp = (t_vec3){0, 0, 0};
	if (keycode == KEY_R)
		reset_camera(d);
	else if (keycode == KEY_D)
		tmp = v_multiply(&d->screen->right, SHIFT_VAL);
	else if (keycode == KEY_A)
		tmp = v_multiply(&d->screen->right, -SHIFT_VAL);
	else if (keycode == KEY_W)
		tmp = v_multiply(&d->cam->dir, SHIFT_VAL);
	else if (keycode == KEY_S)
		tmp = v_multiply(&d->cam->dir, -SHIFT_VAL);
	else if (keycode == KEY_C)
		tmp = v_multiply(&d->screen->up, SHIFT_VAL);
	else if (keycode == KEY_V)
		tmp = v_multiply(&d->screen->up, -SHIFT_VAL);
	d->cam->origin = v_add(&d->cam->origin, &tmp);
	change_cam_dir(d, keycode);
}

const t_obj	*find_first_obj(t_data *data, const t_ray *ray, double *t)
{
	const t_obj	*ret = NULL;
	const t_obj	*obj;
	double		distance;
	double		min_distance;
	int			i;

	i = -1;
	min_distance = INF;
	while (++i < data->obj_count)
	{
		obj = &data->obj_set[i];
		distance = obj->f_intersects(ray, obj);
		if (distance > 0 && distance < min_distance)
		{
			min_distance = distance;
			ret = obj;
		}
	}
	*t = min_distance;
	return (ret);
}

void	pixel_to_virtual(t_data *d, int *x, int *y, t_vec3 *mapped_coords)
{
	t_vec3	tmp1;
	t_vec3	tmp2;
	double	ndc_x;
	double	ndc_y;

	ndc_x = ((2 / (d->screen->x_pix_max - d->screen->x_pix_min)) * (*x + 0.5))
		- ((2 * d->screen->x_pix_min
				/ (d->screen->x_pix_max - d->screen->x_pix_min)) + 1);
	ndc_y = ((-2.0 / HEIGHT) * (*y + 0.5)) + 1;
	tmp1 = v_multiply(&d->screen->right, ndc_x);
	tmp2 = v_multiply(&d->screen->up, ndc_y);
	tmp1 = v_add(&tmp1, &tmp2);
	*mapped_coords = v_add(&d->cam->dir, &tmp1);
}

bool	in_shadow(t_data *data, t_shade_info *si)
{
	t_ray		ray;
	const t_obj	*target_obj;
	double		t2;
	int			i;

	ray.origin = v_multiply(&si->point_to_light_dir, 0.001);
	ray.origin = v_add(&si->hit_point, &ray.origin);
	ray.dir = si->point_to_light_dir;
	i = -1;
	while (++i < data->obj_count)
	{
		target_obj = &data->obj_set[i];
		if (target_obj == si->obj)
			continue ;
		t2 = target_obj->f_intersects(&ray, target_obj);
		if (t2 >= TOL && t2 < v_length(&si->point_to_light))
			return (true);
	}
	return (false);
}

t_shade_info	fill_info(t_obj *obj, t_vec3 *hit_point
	, t_vec3 *light_origin, t_ray *ray)
{
	t_shade_info	shade_info;
	t_vec3			p_to_l;

	shade_info.c_diffuse = (t_color){0, 0, 0};
	shade_info.c_specular = (t_color){0, 0, 0};
	shade_info.color_final = (t_color){0, 0, 0};
	p_to_l = v_substract(light_origin, hit_point);
	shade_info.point_to_light = p_to_l;
	shade_info.point_to_light_dir = v_normalize(&p_to_l);
	shade_info.hit_point = *hit_point;
	shade_info.obj = obj;
	shade_info.surface_normal = obj->f_get_normal(obj, hit_point);
	shade_info.surface_normal = v_normalize(&shade_info.surface_normal);
	shade_info.base_color = obj->f_get_color(obj);
	shade_info.light_origin = *light_origin;
	shade_info.theta = acos(v_dot(&shade_info.surface_normal,
				&shade_info.point_to_light_dir));
	shade_info.ray = *ray;
	return (shade_info);
}

void	set_hitpoint(t_vec3 *hit_point, const t_ray ray, double t)
{
	*hit_point = v_multiply(&ray.dir, t);
	*hit_point = v_add(&ray.origin, hit_point);
}

t_color	trace_ray(t_data *d, t_ray ray)
{
	const t_obj		*hitted_obj;
	t_vec3			hit_point;
	t_shade_info	s_i;
	double			t;

	t = INF;
	hitted_obj = find_first_obj(d, &ray, &t);
	if (hitted_obj)
	{
		set_hitpoint(&hit_point, ray, t);
		s_i.t = t;
		s_i = fill_info((t_obj *)hitted_obj, &hit_point,
				&d->light->origin, &ray);
		if (!in_shadow(d, &s_i))
			compute_illumination(d, &s_i);
		else
			s_i.color_final = color_multiply(&s_i.base_color,
					d->ambient_light->brightness);
	}
	else
		s_i.color_final = (t_color){0, 0, 0};
	return (s_i.color_final);
}

void	render(t_data *data)
{
	t_ray	ray;
	t_vec3	mapped_coords;
	t_color	c;
	int		pix_y;
	int		pix_x;

	ray.origin = data->cam->origin;
	pix_x = data->screen->x_pix_min - 1;
	while (++pix_x < data->screen->x_pix_max)
	{
		pix_y = -1;
		while (++pix_y < HEIGHT)
		{
			pixel_to_virtual(data, &pix_x, &pix_y, &mapped_coords);
			ray.dir = v_normalize(&mapped_coords);
			c = trace_ray(data, ray);
			my_mlx_pixel_put(data->mlx, pix_x, pix_y, rgb_to_int(&c));
		}
	}
}

void	init_viewport(t_data *d)
{
	if (!d->screen || !d->cam)
		free_exit(d);
	d->cam->dir = v_normalize(&d->cam->dir);
	d->screen->focal_length = 1.0 / tan((d->cam->fov / 2) * (M_PI / 180));
	d->screen->aspect_ratio = (double)ASPECT_RATIO_X / (double)ASPECT_RATIO_Y;
	d->screen->x_pix_min = ((double)WIDTH
			- (HEIGHT * d->screen->aspect_ratio)) / 2;
	d->screen->x_pix_max = (double)WIDTH - d->screen->x_pix_min;
	d->screen->forward = v_multiply(&d->cam->dir, -d->screen->focal_length);
	d->screen->origin = v_substract(&d->cam->origin, &d->screen->forward);
	d->screen->forward = v_normalize(&d->screen->forward);
	d->screen->up = (t_vec3){0, 1, 0};
	d->screen->right = v_cross(&d->screen->forward, &d->screen->up);
	d->screen->right = v_normalize(&d->screen->right);
	d->screen->up = v_cross(&d->screen->right, &d->screen->forward);
	d->screen->up = v_normalize(&d->screen->up);
}

void	main_loop(t_data *d)
{
	init_viewport(d);
	render(d);
	mlx_put_image_to_window(d->mlx->mlx_p, d->mlx->win_p, d->mlx->img_p, 0, 0);
	mlx_hook(d->mlx->win_p, 17, 1, &free_exit, d);
	mlx_hook(d->mlx->win_p, EVENT_KEY_PRESS, 1, &handle_key, d);
	mlx_loop(d->mlx->mlx_p);
}
