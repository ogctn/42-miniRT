/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:44:02 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/21 19:41:51 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/keys.h"
#include "../inc/minirt.h"

void	my_mlx_pixel_put(t_mlx *m, int x, int y, int color)
{
	char	*dst;

	dst = m->addr + (y * m->line_len + x * (m->bpp / 8));
	*(unsigned int *)dst = color;
}

double	find_min_2(double a, double b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

double	find_min_3(double a, double b, double c)
{
	double	min;

	if (a < b)
		min = a;
	else
		min = b;
	if (c < min)
		return (c);
	else
		return (min);
}
