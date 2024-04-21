/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 23:20:08 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/21 19:39:18 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_color	f_get_color(const t_obj *obj)
{
	if (obj->type == SPHERE)
		return (((t_sphere *)obj->obj)->color);
	else if (obj->type == PLANE)
		return (((t_plane *)obj->obj)->color);
	else if (obj->type == CYLINDER)
		return (((t_cylinder *)obj->obj)->color);
	return ((t_color){0, 0, 0});
}

double	intersects_sphere(const t_ray *ray, const t_sphere *sp);
double	intersects_plane(const t_ray *ray, const t_plane *pl);
double	intersects_cylinder(const t_ray *ray, const t_cylinder *cy);

double	f_intersects(const t_ray *ray, const t_obj *obj)
{
	if (obj->type == SPHERE)
		return (intersects_sphere(ray, (t_sphere *)obj->obj));
	else if (obj->type == PLANE)
		return (intersects_plane(ray, (t_plane *)obj->obj));
	else if (obj->type == CYLINDER)
		return (intersects_cylinder(ray, (t_cylinder *)obj->obj));
	return (0.0);
}

double	intersects_sphere(const t_ray *ray, const t_sphere *sp)
{
	t_vec3	o_to_c;
	double	b;
	double	c;
	double	h;
	double	t;

	o_to_c = v_substract(&ray->origin, &sp->center);
	b = v_dot(&o_to_c, &ray->dir);
	c = v_dot(&o_to_c, &o_to_c) - sp->r * sp->r;
	h = b * b - c;
	if (h < 0.0)
		return (INF);
	h = sqrt(h);
	t = -b - h;
	if (t < TOL && t > -TOL)
		return (0.0);
	return (t);
}

double	intersects_plane(const t_ray *ray, const t_plane *pl)
{
	t_vec3	tmp;
	double	denom;
	double	t;

	denom = v_dot(&pl->normal, &ray->dir);
	if (denom <= TOL && denom >= -TOL)
		return (INF);
	tmp = v_substract(&pl->point, &ray->origin);
	t = v_dot(&tmp, &pl->normal) / denom;
	if (t <= TOL && t >= -TOL)
		return (INF);
	return (t);
}

double	intersects_cylinder(const t_ray *ray, const t_cylinder *cy)
{
	t_cy_vec	data;

	assign_cy_vectors(ray, cy, &data);
	find_x1(ray, cy, &data);
	find_x2_x3(ray, cy, &data);
	if (data.values.x1_flag == 0
		&& data.values.x2_flag == 0 && data.values.x3_flag == 0)
		return (INF);
	else if (data.values.x2_flag == 0 && data.values.x3_flag == 0 )
		return (data.values.x1);
	else if (data.values.x1_flag == 0 && data.values.x2_flag == 0)
		return (data.values.x3);
	else if (data.values.x1_flag == 0 && data.values.x3_flag == 0)
		return (data.values.x2);
	else if (data.values.x1_flag == 0)
		return (find_min_2(data.values.x2, data.values.x3));
	else if (data.values.x2_flag == 0)
		return (find_min_2(data.values.x1, data.values.x3));
	else if (data.values.x3_flag == 0)
		return (find_min_2(data.values.x1, data.values.x2));
	else
		return (find_min_3(data.values.x1, data.values.x2, data.values.x3));
}
