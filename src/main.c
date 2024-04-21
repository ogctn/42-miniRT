/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:32:43 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/21 14:17:51 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2 || control_extension(av[1]))
		return (printf("Error!\nWrong argument"), 1);
	initialize(&data, av[1]);
	main_loop(&data);
	return (0);
}

// Camera direction and rotation will be implemented. [ test.c: pixel_to_virtual() ]
//light in cylinder
//virgülden sonra tab
// Cylinder intersection formula is uncompleted. [intersections.c: intersects_cylinder() ]
