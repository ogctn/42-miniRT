/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 15:47:32 by ogcetin           #+#    #+#             */
/*   Updated: 2024/04/22 02:49:48 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/vector.h"

t_vec3	v_add(const t_vec3 *u, const t_vec3 *v)
{
	return ((t_vec3){u->x + v->x, u->y + v->y, u->z + v->z});
}

t_vec3	v_substract(const t_vec3 *u, const t_vec3 *v)
{
	return ((t_vec3){u->x - v->x, u->y - v->y, u->z - v->z});
}

t_vec3	v_multiply(const t_vec3 *u, double s)
{
	if (s == INFINITY)
		return ((t_vec3){INFINITY, INFINITY, INFINITY});
	return ((t_vec3){u->x * s, u->y * s, u->z * s});
}

double	v_dot(const t_vec3 *u, const t_vec3 *v)
{
	return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));
}

t_vec3	v_cross(const t_vec3 *u, const t_vec3 *v)
{
	return ((t_vec3){(u->y * v->z - u->z * v->y), (u->z * v->x - u->x * v->z)
		, (u->x * v->y - u->y * v->x)});
}
