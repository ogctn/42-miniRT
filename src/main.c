/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 14:32:43 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/05 09:55:26 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// https://graphics.cs.wisc.edu/WP/cs559-fall2016/files/2016/12/shirley_chapter_4.pdf
// https://graphicscompendium.com/raytracing/02-camera
// https://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/basic_algo.pdf
// https://web.cse.ohio-state.edu/~shen.94/681/Site/Slides_files/shadow.pdf
// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/implementing-the-raytracing-algorithm.html
//
//


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

//  With only one parameter a window must open when launching the program and stay open during the program's whole execution.
// Parse Light should contain rgb values. Control tabs and spaces in the .rt file.
// Cylinder intersection formula is uncompleted. [intersections.c: intersects_cylinder() ] 
