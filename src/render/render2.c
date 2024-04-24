/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 21:26:12 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/24 21:26:26 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

bool	in_shadow(t_data *data, t_shade_info *si)
{
	t_ray		ray;
	const t_obj	*target_obj;
	double		t2;
	int			i;

	ray.origin = v_multiply(&si->point_to_light_dir, 0.001);
	ray.origin = v_add(&si->hit_point, &ray.origin);
	ray.dir = si->point_to_light_dir;
	i = -1;
	while (++i < data->obj_count)
	{
		target_obj = &data->obj_set[i];
		if (target_obj == si->obj)
			continue ;
		t2 = target_obj->f_intersects(&ray, target_obj);
		if (t2 >= TOL && t2 < v_length(&si->point_to_light))
			return (true);
	}
	return (false);
}

t_shade_info	fill_info(t_obj *obj, t_vec3 *hit_point
	, t_vec3 *light_origin, t_ray *ray)
{
	t_shade_info	shade_info;
	t_vec3			p_to_l;

	shade_info.c_diffuse = (t_color){0, 0, 0};
	shade_info.c_specular = (t_color){0, 0, 0};
	shade_info.color_final = (t_color){0, 0, 0};
	p_to_l = v_substract(light_origin, hit_point);
	shade_info.point_to_light = p_to_l;
	shade_info.point_to_light_dir = v_normalize(&p_to_l);
	shade_info.hit_point = *hit_point;
	shade_info.obj = obj;
	shade_info.surface_normal = obj->f_get_normal(obj, hit_point);
	shade_info.surface_normal = v_normalize(&shade_info.surface_normal);
	shade_info.base_color = obj->f_get_color(obj);
	shade_info.light_origin = *light_origin;
	shade_info.theta = acos(v_dot(&shade_info.surface_normal,
				&shade_info.point_to_light_dir));
	shade_info.ray = *ray;
	return (shade_info);
}
