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
	double	t3;
	double	t4;
	double	min;
	t_vec3	oc;
	t_vec3	tmp;
	t_vec3	tmp2;
	t_vec3	p1;
	t_vec3	pi;
	t_vec3	q3;
	t_vec3	q4;
	t_vec3	p2;
	t_vec3	va;
	int t1_flag = 1;
	int t3_flag = 1;
	int t4_flag = 1;

	tmp = v_multiply(&cy->dir,cy->height);
	p2  = v_add(&cy->origin,&tmp);
	//p1  = v_substract(&cy->origin,&tmp);
	p1 = cy->origin;
	va = v_substract(&p2,&p1);
	va = v_normalize(&va);

	oc = v_substract(&ray->origin, &cy->origin);
	tmp = v_multiply(&va, v_dot(&ray->dir, &va));
	tmp = v_substract(&ray->dir, &tmp);
	a = v_dot(&tmp, &tmp);
	tmp2 = v_multiply(&va, v_dot(&oc, &va));
	tmp2 = v_substract(&oc, &tmp2);
	b = 2 * v_dot(&tmp, &tmp2);
	tmp2 = v_multiply(&va, v_dot(&oc, &va));
	tmp2 = v_substract(&oc, &tmp2);
	c = v_dot(&tmp2, &tmp2) - cy->r * cy->r;
	d = b * b - 4 * a * c;
	if (d < 0)
		t1_flag = 0;
	t = (-b - sqrt(d)) / (2 * a);
	if (t < TOL && t > -TOL)
		t1_flag = 0;
	tmp = v_multiply(&ray->dir,t); 
	pi = v_add(&ray->origin, &tmp);
	tmp = v_substract(&pi,&p1);
	a = v_dot(&va,&tmp);
	tmp = v_substract(&pi,&p2);
	b = v_dot(&va,&tmp);
	if (a < 0 || b > 0)
		t1_flag = 0;
	b = v_dot(&va,&ray->origin);
	c = v_dot(&va,&p1);
	a = v_dot(&va,&ray->dir);
	t3 = - ((b-c) / a); 
	tmp = v_substract(&ray->origin,&p2);
	c = v_dot(&va,&p2);
	t4 = - ((b-c) / a); 
	tmp = v_multiply(&ray->dir,t3);
	q3 = v_add(&ray->origin,&tmp);
	tmp = v_multiply(&ray->dir,t4);
	q4 = v_add(&ray->origin,&tmp);
	tmp = v_substract(&q3,&p1);
	tmp2 = v_substract(&q4,&p2);
	a = v_dot(&tmp,&tmp);
	b = v_dot(&tmp2,&tmp2);
	if(t3 < 0)
		t3_flag = 0;
	if(t4 < 0)
		t4_flag = 0;	
	if(a >= (cy->r*cy->r))
		t3_flag = 0;
	if(b >= (cy->r*cy->r))
		t4_flag = 0;
	if(t3_flag == 0 && t4_flag == 0 && t1_flag == 0)
		return (INF);
	else if(t3_flag == 0 && t4_flag == 0 ) 
		return(t);
	else if(t1_flag == 0 && t3_flag == 0)
		return(t4);
	else if(t1_flag == 0 && t4_flag == 0)
		return(t3);
	else if(t1_flag == 0)
	{
		if(t3 < t4)
			return(t3);
		else
			return(t4);
	}
	else if(t3_flag == 0)
	{
		if(t < t4)
			return(t);
		else
			return(t4);
	}
	else if(t4_flag == 0)
	{
		if(t < t3)
			return(t);
		else
			return(t3);
	}
	else
	{
		if(t3 < t4)
			min = t3;
		else
			min = t4;
		if(t < min)
			return(t);
		else
			return(min);
	}
}
