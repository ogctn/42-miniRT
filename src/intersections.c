/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 23:20:08 by ogcetin           #+#    #+#             */
/*   Updated: 2024/03/01 06:01:24 by ogcetin          ###   ########.fr       */
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

// Only works for infinite cylinders
double	intersects_cylinder(const t_ray *ray, const t_cylinder *cy)
{
	double	a;
	double	b;
	double	c;
	double	d;
	double	t;
	t_vec3	oc;
	t_vec3	tmp;
	t_vec3	tmp2;

	oc = v_substract(&ray->origin, &cy->origin);
	tmp = v_multiply(&cy->dir, v_dot(&ray->dir, &cy->dir));
	tmp = v_substract(&ray->dir, &tmp);
	a = v_dot(&tmp, &tmp);
	tmp2 = v_multiply(&cy->dir, v_dot(&oc, &cy->dir));
	tmp2 = v_substract(&oc, &tmp2);
	b = 2 * v_dot(&tmp, &tmp2);
	tmp2 = v_multiply(&cy->dir, v_dot(&oc, &cy->dir));
	tmp2 = v_substract(&oc, &tmp2);
	tmp = v_multiply(&cy->dir, v_dot(&oc, &cy->dir));
	tmp = v_substract(&oc, &tmp);
	c = v_dot(&tmp2, &tmp) - cy->r * cy->r;
	d = b * b - 4 * a * c;
	
	if (d < 0)
		return (INF);
	t = (-b - sqrt(d)) / (2 * a);
	if (t < TOL && t > -TOL)
		return (INF);
	return (t);
}
