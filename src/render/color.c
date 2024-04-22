/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 03:02:34 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/22 03:02:39 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

t_color	color_multiply(t_color *color, double factor)
{
	t_color	ret;

	ret.r = color->r * factor;
	if (ret.r > 255)
		ret.r = 255;
	ret.g = color->g * factor;
	if (ret.g > 255)
		ret.g = 255;
	ret.b = color->b * factor;
	if (ret.b > 255)
		ret.b = 255;
	return (ret);
}

t_color	color_add(t_color *color1, t_color *color2)
{
	t_color	ret;

	ret.r = color1->r + color2->r;
	if (ret.r > 255)
		ret.r = 255;
	ret.g = color1->g + color2->g;
	if (ret.g > 255)
		ret.g = 255;
	ret.b = color1->b + color2->b;
	if (ret.b > 255)
		ret.b = 255;
	return (ret);
}

void	compute_illumination(t_data *data, t_color *color,
	t_vec3 *hit_point, t_vec3 *normal)
{
	t_vec3	point_to_light_dir;
	double	theta;
	double	intensity;

	point_to_light_dir = v_substract(&data->light->origin, hit_point);
	point_to_light_dir = v_normalize(&point_to_light_dir);
	theta = acos(v_dot(normal, &point_to_light_dir));
	if (theta >= M_PI / 2.)
	{
		*color = (t_color){0, 0, 0};
		intensity = 0;
	}
	else
		intensity = data->light->brightness * (1 - (theta / (M_PI / 2.)));
	if (intensity > 0)
	{
		color->r = color->r * intensity;
		color->g = color->g * intensity;
		color->b = color->b * intensity;
	}
}

void	color_mix(t_color *color, t_color *ambient,
	t_color *light, double ratio)
{
	color->r = ambient->r * (1 - ratio) + light->r * ratio;
	if (color->r > 255)
		color->r = 255;
	color->g = ambient->g * (1 - ratio) + light->g * ratio;
	if (color->g > 255)
		color->g = 255;
	color->b = ambient->b * (1 - ratio) + light->b * ratio;
	if (color->b > 255)
		color->b = 255;
}
