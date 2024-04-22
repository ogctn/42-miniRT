/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_normal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:51:49 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/22 02:52:45 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"
#include <stdio.h>

t_vec3	f_get_normal_sphere(const t_obj *obj, t_vec3 *hit_point)
{
	t_sphere	*sp;
	t_vec3		normal;

	sp = obj->obj;
	normal = v_substract(hit_point, &sp->center);
	normal = v_normalize(&normal);
	return (normal);
}

t_vec3	f_get_normal_plane(const t_obj *obj, t_vec3 *hit_point)
{
	t_plane	*pl;
	t_vec3	normal;

	(void)hit_point;
	pl = (t_plane *)obj->obj;
	normal = pl->normal;
	normal = v_normalize(&normal);
	return (normal);
}

t_vec3	f_get_normal_cylinder(const t_obj *obj, t_vec3 *hit_point)
{
	t_cylinder	*cyl;
	t_vec3		normal;
	t_vec3		oc;
	t_vec3		tmp2;

	cyl = (t_cylinder *)obj->obj;
	oc = v_substract(hit_point, &cyl->origin);
	tmp2 = v_multiply(&cyl->dir, v_dot(&oc, &cyl->dir));
	normal = v_substract(&oc, &tmp2);
	normal = v_normalize(&normal);
	return (normal);
}

t_vec3	f_get_normal(const t_obj *obj, t_vec3 *hit_point)
{
	t_vec3	normal;

	if (obj->type == SPHERE)
		normal = f_get_normal_sphere(obj, hit_point);
	else if (obj->type == PLANE)
		normal = f_get_normal_plane(obj, hit_point);
	else if (obj->type == CYLINDER)
		normal = f_get_normal_cylinder(obj, hit_point);
	else
		normal = (t_vec3){0, 0, 0};
	return (normal);
}
