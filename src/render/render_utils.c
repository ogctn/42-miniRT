/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 21:44:02 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/22 03:08:43 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/keys.h"
#include "../../inc/minirt.h"

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

void	free_objects(t_data *genel)
{
	int	i;

	i = -1;
	while (++i < (int) genel->obj_count)
	{
		if (genel->obj_set[i].obj)
			free(genel->obj_set[i].obj);
	}
	if (genel->obj_set)
		free(genel->obj_set);
}

int	free_exit(t_data *genel)
{
	if (genel)
	{
		if (genel->mlx->img_p)
		{
			mlx_destroy_image(genel->mlx->mlx_p, genel->mlx->img_p);
			if (genel->mlx->win_p)
				mlx_destroy_window(genel->mlx->mlx_p, genel->mlx->win_p);
		}
		free_objects(genel);
		if (genel->cam)
			free(genel->cam);
		if (genel->screen)
			free(genel->screen);
		if (genel->mlx)
			free(genel->mlx);
	}
	exit (0);
}
