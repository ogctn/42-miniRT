/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 03:02:34 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/24 21:23:28 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

void	color_clamp(t_color *c)
{
	if (c->r > 255)
		c->r = 255;
	if (c->r < 0)
		c->r = 0;
	if (c->g > 255)
		c->g = 255;
	if (c->g < 0)
		c->g = 0;
	if (c->b > 255)
		c->b = 255;
	if (c->b < 0)
		c->b = 0;
}

t_color	color_multiply(t_color *color, double factor)
{
	t_color	ret;

	color_clamp(color);
	ret.r = color->r * factor;
	ret.g = color->g * factor;
	ret.b = color->b * factor;
	color_clamp(&ret);
	return (ret);
}

t_color	color_add(t_color *color1, t_color *color2)
{
	t_color	ret;

	ret.r = color1->r + color2->r;
	ret.g = color1->g + color2->g;
	ret.b = color1->b + color2->b;
	color_clamp(&ret);
	return (ret);
}

void	compute_illumination(t_data *data, t_shade_info *si)
{
	double	dot;
	t_vec3	temp;

	si->color_final = color_multiply(&si->base_color,
			data->ambient_light->brightness);
	dot = v_dot(&si->point_to_light_dir, &si->surface_normal);
	if (dot > 0)
	{
		si->c_diffuse = color_multiply(&si->base_color, dot);
		si->c_diffuse = color_multiply(&si->c_diffuse, data->light->brightness);
		si->color_final = color_add(&si->color_final, &si->c_diffuse);
	}
	dot = v_dot(&si->surface_normal, &si->point_to_light_dir);
	temp = v_multiply(&si->surface_normal, 2 * dot);
	temp = v_substract(&si->point_to_light_dir, &temp);
	temp = v_normalize(&temp);
	dot = v_dot(&temp, &si->ray.dir);
	if (dot > 0)
	{
		dot = pow(dot, 400);
		dot *= data->light->brightness;
		si->c_specular = color_multiply(&data->light->color, dot);
		si->color_final = color_add(&si->color_final, &si->c_specular);
	}
}

void	color_mix(t_color *color, t_color ambient, t_color light)
{
	double	ratio;

	color_clamp(&ambient);
	color_clamp(&light);
	ratio = COLOR_MIX_RATIO;
	color->r = ambient.r * (1 - ratio) + light.r * ratio;
	color->g = ambient.g * (1 - ratio) + light.g * ratio;
	color->b = ambient.b * (1 - ratio) + light.b * ratio;
	color_clamp(color);
}
