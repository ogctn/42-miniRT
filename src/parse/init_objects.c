/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 00:52:24 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/21 14:58:30 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

int	init_pl(t_data *data, char *line, int index)
{
	t_plane	*new;
	char	**ptr;
	char	**ptr_2;
	char	**ptr_3;
	char	**ptr_4;

	new = ft_calloc(sizeof(t_plane), 1);
	ptr = ft_split(line, ' ');
	if (size_2d(ptr) != 4)
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	ptr_2 = ft_split(ptr[1], ',');
	if (!ft_assign_vec(ptr_2, &new->point, 0))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	ptr_3 = ft_split(ptr[2], ',');
	if (!ft_assign_vec(ptr_3, &new->normal, 1))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	ptr_4 = ft_split(ptr[3], ',');
	if (!ft_assign_color(ptr_4, &new->color))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	free_2d(ptr);
	data->obj_set[index].obj = new;
	return (1);
}

int	init_cy(t_data *data, char *line, int index)
{
	t_cylinder	*new;
	char		**ptr;
	char		**ptr_2;
	char		**ptr_3;

	new = ft_calloc(sizeof(t_cylinder), 1);
	ptr = ft_split(line, ' ');
	if (size_2d(ptr) != 6)
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	ptr_2 = ft_split(ptr[1], ',');
	if (!ft_assign_vec(ptr_2, &new->origin, 0))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	ptr_3 = ft_split(ptr[2], ',');
	if (!ft_assign_vec(ptr_3, &new->dir, 1))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	if (!ft_assign_color(ft_split(ptr[5], ','), &new->color))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	new->r = ft_atod(ptr[3]) / 2;
	new->height = ft_atod(ptr[4]);
	if (new->r <= 0 || new->height <= 0)
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	data->obj_set[index].obj = new;
	return (free_2d(ptr), 1);
}

int	init_sp(t_data *data, char *line, int index)
{
	t_sphere	*new;
	char		**ptr;
	char		**ptr_2;
	char		**ptr_3;

	new = ft_calloc(sizeof(t_sphere), 1);
	ptr = ft_split(line, ' ');
	if (size_2d(ptr) != 4)
		return (printf("Error!\nWrong Definition\n"), 0);
	ptr_2 = ft_split(ptr[1], ',');
	if (!ft_assign_vec(ptr_2, &new->center, 0))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	ptr_3 = ft_split(ptr[3], ',');
	if (!ft_assign_color(ptr_3, &new->color))
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	new->r = ft_atod(ptr[2]) / 2;
	if (new->r <= 0)
		return (free_2d(ptr), printf("Error!\nWrong Definition\n"), 0);
	free_2d(ptr);
	data->obj_set[index].obj = new;
	return (1);
}

int	init_obj(t_data *data, char *line, int type)
{
	int	i;

	i = 0;
	if (data->obj_count == 0)
		return (1);
	if ((data->obj_set))
	{
		while (data->obj_set[i].obj)
			i++;
	}
	else
		data->obj_set = ft_calloc(sizeof(t_obj), data->obj_count + 1);
	data->obj_set[i].idx = i;
	data->obj_set[i + 1].obj = NULL;
	data->obj_set[i].type = type;
	data->obj_set[i].f_intersects = f_intersects;
	data->obj_set[i].f_get_color = f_get_color;
	data->obj_set[i].f_get_normal = f_get_normal;
	if (type == 0)
		return (init_pl(data, line, i));
	if (type == 1)
		return (init_sp(data, line, i));
	if (type == 2)
		return (init_cy(data, line, i));
	return (0);
}
