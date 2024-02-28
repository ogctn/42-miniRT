/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:05:35 by sgundogd          #+#    #+#             */
/*   Updated: 2024/02/28 19:44:38 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

void	control_elements(t_data *data)
{
	if (data->a != 1 || data->c != 1 || data->l != 1)
	{
		printf("Error!\nMissing or extra Elements\n");
		exit(1);
	}
	if (data->ambient_light->brightness < 0.0
		|| data->ambient_light->brightness > 1.0)
	{
		printf("Error!\nWrong Definiton\n");
		exit(1);
	}
	if (data->light->brightness < 0.0
		|| data->light->brightness > 1.0)
	{
		printf("Error!\nWrong Definiton\n");
		exit(1);
	}
	if (data->cam->fov < 0.0
		|| data->light->brightness > 180.0)
	{
		printf("Error!\nWrong Definiton\n");
		exit(1);
	}
}

int	control_rgb(t_color clr)
{
	if (clr.r > 255 || clr.r < 0 || clr.g > 255
		|| clr.g < 0 || clr.b > 255 || clr.b < 0)
		return (0);
	return (1);
}

int	control_extension(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (ft_strcmp(ft_substr(str, i - 3, 3), ".rt"));
}
