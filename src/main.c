/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:32:43 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/22 01:45:32 by ogcetin          ###   ########.fr       */
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
// Fix cam_move function 
