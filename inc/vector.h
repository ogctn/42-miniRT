/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 01:02:06 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/23 04:28:43 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <math.h>

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

t_vec3	v_add(const t_vec3 *u, const t_vec3 *v);
t_vec3	v_substract(const t_vec3 *u, const t_vec3 *v);
t_vec3	v_multiply(const t_vec3 *u, double s);
double	v_dot(const t_vec3 *u, const t_vec3 *v);
t_vec3	v_cross(const t_vec3 *u, const t_vec3 *v);
t_vec3	v_normalize(const t_vec3 *u);
double	v_length(const t_vec3 *u);
t_vec3	v_rotate(t_vec3 *v, t_vec3 *axis, double theta);

#endif
