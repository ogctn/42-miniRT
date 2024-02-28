/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_features.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:17:41 by sgundogd          #+#    #+#             */
/*   Updated: 2024/02/28 19:40:00 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

int	ft_assign_color(char **ptr, t_color *clr)
{
	if (size_2d(ptr) != 3)
		return (free_2d(ptr), 0);
	clr->r = ft_atoi(ptr[0]);
	clr->g = ft_atoi(ptr[1]);
	clr->b = ft_atoi(ptr[2]);
	if (control_rgb(*clr) == 0)
		return (free_2d(ptr), 0);
	free_2d(ptr);
	return (1);
}

int	ft_assign_vec(char **ptr, t_vec3 *vec, int flag)
{
	if (size_2d(ptr) != 3)
	{
		free_2d(ptr);
		return (0);
	}
	vec->x = ft_atod(ptr[0]);
	vec->y = ft_atod(ptr[1]);
	vec->z = ft_atod(ptr[2]);
	if (vec->x == INFINITY || vec->y == INFINITY || vec->z == INFINITY)
		return (free_2d(ptr), 0);
	if (flag == 1)
	{
		if (vec->x < -1.0 || vec->x > 1.0 || vec->y < -1.0
			|| vec->y > 1.0 || vec->z < -1.0 || vec->z > 1.0)
			return (free_2d(ptr), 0);
	}
	free_2d(ptr);
	return (1);
}
