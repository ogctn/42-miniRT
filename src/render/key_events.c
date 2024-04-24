/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 21:17:51 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/24 21:31:05 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

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
