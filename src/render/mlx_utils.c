/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 03:00:33 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/22 03:13:13 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"
#include <stdio.h>

int	rgb_to_int(const t_color *rgb)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = (unsigned char)rgb->r;
	g = (unsigned char)rgb->g;
	b = (unsigned char)rgb->b;
	return (r << 16 | g << 8 | b);
}

void	my_mlx_pixel_put(t_mlx *m, int x, int y, int color)
{
	char	*dst;

	dst = m->addr + (y * m->line_len + x * (m->bpp / 8));
	*(unsigned int *)dst = color;
}

int	handle_key(int keycode, t_data *d)
{
	if (keycode == KEY_ESC)
		free_exit(d);
	cam_move(d, keycode);
	render(d);
	mlx_put_image_to_window(d->mlx->mlx_p, d->mlx->win_p, d->mlx->img_p, 0, 0);
	return (0);
}

void	new_mlx(t_data *d)
{
	if (!d->mlx)
		free_exit(d);
	d->mlx->mlx_p = mlx_init();
	if (!d->mlx->mlx_p)
		free_exit(d);
	d->mlx->win_p = mlx_new_window(d->mlx->mlx_p, WIDTH,
			HEIGHT, "tnoyan's team");
	if (!d->mlx->win_p)
		free_exit(d);
	d->mlx->img_p = mlx_new_image(d->mlx->mlx_p, WIDTH, HEIGHT);
	if (!d->mlx->img_p)
		free_exit(d);
	d->mlx->addr = mlx_get_data_addr(d->mlx->img_p,
			&d->mlx->bpp, &d->mlx->line_len, &d->mlx->endian);
	if (!d->mlx->addr)
		free_exit(d);
}
