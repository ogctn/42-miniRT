/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 01:00:55 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/22 03:04:27 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"
#include <stdio.h>

void	mlx_stuffs(t_data *a);
void	my_mlx_pixel_put(t_mlx *a, int b, int c, int d);
void	render_background( t_mlx *a, int b);
void	render(t_data *data);
int		rgb_to_int(const t_color *rgb);

void	reset_camera(t_data *genel)
{
	genel->cam->origin = genel->default_cam->origin;
	genel->cam->dir = genel->default_cam->dir;
	genel->cam->fov = genel->default_cam->fov;
}

void	cam_move(t_data *genel, int keycode)
{
	if (keycode == KEY_D)
		genel->cam->origin.x += SHIFT_VAL;
	else if (keycode == KEY_A)
		genel->cam->origin.x -= SHIFT_VAL;
	else if (keycode == KEY_C)
		genel->cam->origin.z += SHIFT_VAL;
	else if (keycode == KEY_V)
		genel->cam->origin.z -= SHIFT_VAL;
	else if (keycode == KEY_W)
		genel->cam->origin.y += SHIFT_VAL;
	else if (keycode == KEY_S)
		genel->cam->origin.y -= SHIFT_VAL;
	else if (keycode == KEY_R)
		reset_camera(genel);
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

bool	any_obj_between_light_and_hit_point(t_data *data,
	t_vec3 *hit_point, t_vec3 *point_to_light, double t)
{
	t_ray		ray;
	const t_obj	*hitted_obj;
	double		t2;
	int			i;

	ray.origin = *hit_point;
	ray.dir = v_substract(point_to_light, hit_point);
	ray.dir = v_normalize(&ray.dir);
	i = -1;
	while (++i < data->obj_count)
	{
		hitted_obj = &data->obj_set[i];
		t2 = hitted_obj->f_intersects(&ray, hitted_obj);
		if (t2 >= 0 && t2 < t)
			return (true);
	}
	return (false);
}


// void	compute_illumination(t_data *data, t_shade_info *s)
// {
// 	double theta;
// 	double intensity;

// 	theta = acos(v_dot(s->surface_normal, s->point_to_light_dir));
// 	if (theta > M_PI / 2.)
// 		intensity = 0;
// 	else
// 		intensity = data->light->brightness * (1 - (theta / (M_PI / 2.)));
// 	if (intensity > 0)
// 		color_multiply(s->color, 0.5 * intensity);
// }

t_shade_info	fill_shade_info(t_color *color, t_vec3 *hit_point,
	t_vec3 *surface_normal, t_vec3 *point_to_light_dir)
{
	t_shade_info	shade_info;

	shade_info.color = color;
	shade_info.hit_point = hit_point;
	shade_info.surface_normal = surface_normal;
	shade_info.point_to_light_dir = point_to_light_dir;
	return (shade_info);
}

void	render(t_data *data)
{
	t_ray			ray;
	const t_obj		*hitted_obj;
	double			t;
	t_color			color;
	t_color			color1;
	t_color			color2;
	t_vec3			mapped_coords;
	t_vec3			hit_point;
	t_vec3			surface_normal;
	t_vec3			point_to_light_dir;
	t_shade_info	shade_info;
	int				pix_y;
	int				pix_x;

	hit_point = (t_vec3){0, 0, 0};
	surface_normal = (t_vec3){0, 0, 0};
	point_to_light_dir = (t_vec3){0, 0, 0};
	ray.origin = data->cam->origin;
	pix_x = data->screen->x_pix_min - 1;
	while (++pix_x < data->screen->x_pix_max)
	{
		pix_y = -1;
		while (++pix_y < HEIGHT)
		{
			t = INF;
			pixel_to_virtual(data, &pix_x, &pix_y, &mapped_coords);
			ray.dir = v_normalize(&mapped_coords);
			hitted_obj = find_first_obj(data, &ray, &t);
			hit_point = v_multiply(&ray.dir, t);
			hit_point = v_add(&ray.origin, &hit_point);
			if (hitted_obj)
			{
				color = hitted_obj->f_get_color(hitted_obj);
				color1 = color_multiply(&color,
						data->ambient_light->brightness);
				point_to_light_dir = v_substract(&hit_point,
						&data->light->origin);
				point_to_light_dir = v_normalize(&point_to_light_dir);
				surface_normal = hitted_obj->f_get_normal(hitted_obj,
						&hit_point);
				shade_info = fill_shade_info(&color, &hit_point,
						&surface_normal, &point_to_light_dir);
				color2 = color;
				compute_illumination(data, &color2,
					&hit_point, &surface_normal);
				if (any_obj_between_light_and_hit_point(data,
						&hit_point, &data->light->origin, t))
					color2 = (t_color){0, 0, 0};
				color_mix(&color, &color1, &color2, COLOR_MIX_RATIO);
			}
			else
				color = (t_color){0, 0, 0};
			my_mlx_pixel_put(data->mlx, pix_x, pix_y, rgb_to_int(&color));
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

// void __attribute__((destructor)) a(){
// 	system("leaks test.out");
// }
