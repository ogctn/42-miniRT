/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 01:00:55 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/24 21:32:29 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

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
