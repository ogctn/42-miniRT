/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_elements.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 15:39:23 by sgundogd          #+#    #+#             */
/*   Updated: 2024/02/28 20:05:18 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

int	init_ambient(t_data *data, char *line)
{
	char	**ptr;
	char	**ptr_2;

	ptr = ft_split(line, ' ');
	if (size_2d(ptr) != 3)
		return (free_2d(ptr), printf("Wrong Definition"), 0);
	ptr_2 = ft_split(ptr[2], ',');
	if (!ft_assign_color(ptr_2, &data->ambient_light->color))
		return (free_2d(ptr), printf("Wrong Definition"), 0);
	data->ambient_light->brightness = ft_atod(ptr[1]);
	data->a += 1;
	return (free_2d(ptr), 1);
}

int	init_cam(t_data *data, char *line)
{
	char	**ptr;
	char	**ptr_2;
	char	**ptr_3;

	ptr = ft_split(line, ' ');
	if (size_2d(ptr) != 4)
		return (free_2d(ptr), printf("Wrong Definition"), 0);
	ptr_2 = ft_split(ptr[1], ',');
	if (!ft_assign_vec(ptr_2, &data->cam->origin, 0))
		return (free_2d(ptr), printf("Wrong Definition"), 0);
	ptr_3 = ft_split(ptr[2], ',');
	if (!ft_assign_vec(ptr_3, &data->cam->dir, 1))
		return (free_2d(ptr), printf("Wrong Definition"), 0);
	data->cam->fov = ft_atod(ptr[3]);
	data->c += 1;
	return (free_2d(ptr), 1);
}

int	init_light(t_data *data, char *line)
{
	char	**ptr;
	char	**ptr_2;

	ptr = ft_split(line, ' ');
	if (size_2d(ptr) != 3)
		return (free_2d(ptr), printf("Wrong Definition"), 0);
	ptr_2 = ft_split(ptr[1], ',');
	if (!ft_assign_vec(ptr_2, &data->light->origin, 0))
		return (free_2d(ptr), printf("Wrong Definition"), 0);
	data->light->brightness = ft_atod(ptr[2]);
	data->l += 1;
	return (free_2d(ptr), 1);
}
