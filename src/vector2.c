/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgundogd <sgundogd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 01:20:40 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/22 02:31:08 by sgundogd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/vector.h"

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
