/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 01:20:40 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/23 04:28:53 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/vector.h"

double	v_length(const t_vec3 *u)
{
	return (sqrtf(u->x * u->x + u->y * u->y + u->z * u->z));
}

t_vec3	v_normalize(const t_vec3 *u)
{
	double	len;

	len = v_length(u);
	if (len == 0)
		return ((t_vec3){0, 0, 0});
	return ((t_vec3){u->x / len, u->y / len, u->z / len});
}

double	r_squared_distance(const t_vec3 *p, const t_vec3 *c)
{
	return ((p->x - c->x * p->x - c->x)
		+ (p->y - c->y * p->y - c->y) + (p->z - c->z * p->z - c->z));
}

double	r_distance(const t_vec3 *p, const t_vec3 *c)
{
	return (sqrtf(r_squared_distance(p, c)));
}

t_vec3	v_rotate(t_vec3 *v, t_vec3 *axis, double theta)
{
	t_vec3	r;
	double	cos_theta;
	double	sin_theta;

	cos_theta = cos(theta);
	sin_theta = sin(theta);
	r.x = (cos_theta + (1 - cos_theta) * axis->x * axis->x) * v->x;
	r.x += ((1 - cos_theta) * axis->x * axis->y - axis->z * sin_theta) * v->y;
	r.x += ((1 - cos_theta) * axis->x * axis->z + axis->y * sin_theta) * v->z;
	r.y = ((1 - cos_theta) * axis->x * axis->y + axis->z * sin_theta) * v->x;
	r.y += (cos_theta + (1 - cos_theta) * axis->y * axis->y) * v->y;
	r.y += ((1 - cos_theta) * axis->y * axis->z - axis->x * sin_theta) * v->z;
	r.z = ((1 - cos_theta) * axis->x * axis->z - axis->y * sin_theta) * v->x;
	r.z += ((1 - cos_theta) * axis->y * axis->z + axis->x * sin_theta) * v->y;
	r.z += (cos_theta + (1 - cos_theta) * axis->z * axis->z) * v->z;
	return (r);
}
