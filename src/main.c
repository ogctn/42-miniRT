/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:32:43 by sgundogd          #+#    #+#             */
/*   Updated: 2024/02/25 18:57:54 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2 || control_extension(av[1]))
		return (printf("Error: Wrong argument"), 1);
	initialize(&data, av[1]);
	printf("A: %f %d\n",data.ambient_light->brightness, data.ambient_light->color.r);
	printf("C: %f,%f\n",data.cam->origin.x, data.cam->fov);
	printf("sp: %d and %d\n",data.obj_set[0].type, data.obj_set[1].idx);
}

//int main(int ac, char **av) {
//	t_data	data;
//
//	if (ac != 2 || control_extension(av[1]))
//		return (printf("Error: Wrong argument"), 1);
//	initialize(&data, av[1]);
//
//	unsigned short k = -1;
//	int i = -1;	while (++i < sphere_count) set_sphere(&genel, &sphere_set[i], ++k);
//	//	i = -1;	while (++i < plane_count) set_plane(&genel, &plane_set[i], ++k);
//
//
//	set_stuffs(&data);
//
//	main_loop(&data);
//	//mlx_stuffs( &genel);
//
//}
