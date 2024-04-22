/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections_cy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 18:41:56 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/22 02:49:39 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

void	assign_cy_vectors(const t_ray *ray, const t_cylinder *cy, t_cy_vec *d)
{
	t_vec3	tmp;

	tmp = v_multiply(&cy->dir, (cy->height / 2));
	d->p2 = v_add(&cy->origin, &tmp);
	d->p1 = v_substract(&cy->origin, &tmp);
	d->va = v_substract(&d->p2, &d->p1);
	d->va = v_normalize(&d->va);
	d->oc = v_substract(&ray->origin, &d->p1);
	d->values.x1_flag = 1;
	d->values.x2_flag = 1;
	d->values.x3_flag = 1;
}

void	control_x1(const t_ray *ray, t_cy_vec *data)
{
	t_vec3	tmp;
	t_vec3	pi;
	double	a;
	double	b;

	tmp = v_multiply(&ray->dir, data->values.x1);
	pi = v_add(&ray->origin, &tmp);
	tmp = v_substract(&pi, &data->p1);
	a = v_dot(&data->va, &tmp);
	tmp = v_substract(&pi, &data->p2);
	b = v_dot(&data->va, &tmp);
	if (a < 0 || b > 0)
		data->values.x1_flag = 0;
}

void	find_x1(const t_ray *ray, const t_cylinder *cy, t_cy_vec *data)
{
	t_vec3	tmp;
	t_vec3	tmp2;

	tmp = v_multiply(&data->va, v_dot(&ray->dir, &data->va));
	tmp = v_substract(&ray->dir, &tmp);
	data->metrics.a = v_dot(&tmp, &tmp);
	tmp2 = v_multiply(&data->va, v_dot(&data->oc, &data->va));
	tmp2 = v_substract(&data->oc, &tmp2);
	data->metrics.b = 2 * v_dot(&tmp, &tmp2);
	tmp2 = v_multiply(&data->va, v_dot(&data->oc, &data->va));
	tmp2 = v_substract(&data->oc, &tmp2);
	data->metrics.c = v_dot(&tmp2, &tmp2) - cy->r * cy->r;
	data->metrics.d = data->metrics.b * data->metrics.b - 4
		* data->metrics.a * data->metrics.c;
	if (data->metrics.d < 0)
		data->values.x1_flag = 0;
	data->values.x1 = (-data->metrics.b - sqrt(data->metrics.d))
		/ (2 * data->metrics.a);
	if (data->values.x1 < TOL && data->values.x1 > -TOL)
		data->values.x1_flag = 0;
	control_x1(ray, data);
}

void	control_x2_x3(const t_ray *ray, const t_cylinder *cy, t_cy_vec *data)
{
	t_vec3	tmp;
	t_vec3	tmp2;
	t_vec3	q3;
	t_vec3	q4;

	tmp = v_multiply(&ray->dir, data->values.x2);
	q3 = v_add(&ray->origin, &tmp);
	tmp = v_multiply(&ray->dir, data->values.x3);
	q4 = v_add(&ray->origin, &tmp);
	tmp = v_substract(&q3, &data->p1);
	tmp2 = v_substract(&q4, &data->p2);
	data->metrics.a = v_dot(&tmp, &tmp);
	data->metrics.b = v_dot(&tmp2, &tmp2);
	if (data->metrics.a >= (cy->r * cy->r))
		data->values.x2_flag = 0;
	if (data->metrics.b >= (cy->r * cy->r))
		data->values.x3_flag = 0;
}

void	find_x2_x3(const t_ray *ray, const t_cylinder *cy, t_cy_vec *data)
{
	t_vec3	tmp;
	double	a;
	double	b;
	double	c;

	b = v_dot(&data->va, &ray->origin);
	c = v_dot(&data->va, &data->p1);
	a = v_dot(&data->va, &ray->dir);
	data->values.x2 = - ((b - c) / a);
	tmp = v_substract(&ray->origin, &data->p2);
	c = v_dot(&data->va, &data->p2);
	data->values.x3 = - ((b - c) / a);
	if (data->values.x2 < 0)
		data->values.x2_flag = 0;
	if (data->values.x3 < 0)
		data->values.x3_flag = 0;
	control_x2_x3(ray, cy, data);
}
