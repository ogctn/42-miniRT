/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 21:20:00 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/24 21:31:17 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

void	compute_illumination(t_data *data, t_shade_info *si);
void	my_mlx_pixel_put(t_mlx *a, int b, int c, int d);
int		rgb_to_int(const t_color *rgb);

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
