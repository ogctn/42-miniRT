/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_features.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogcetin <ogcetin@student.42istanbul.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:17:41 by sgundogd          #+#    #+#             */
/*   Updated: 2024/04/25 00:38:20 by ogcetin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirt.h"

int	ft_assign_color(char **ptr, t_color *clr)
{
	if (size_2d(ptr) != 3)
		return (free_2d(ptr), 0);
	clr->r = ft_atod(ptr[0]);
	clr->g = ft_atod(ptr[1]);
	clr->b = ft_atod(ptr[2]);
	if (control_rgb(*clr) == 0)
		return (free_2d(ptr), 0);
	free_2d(ptr);
	return (1);
}

int	ft_assign_vec(char **ptr, t_vec3 *vec, int flag)
{
	if (size_2d(ptr) != 3)
		return (free_2d(ptr), 0);
	vec->x = ft_atod(ptr[0]);
	vec->y = ft_atod(ptr[1]);
	vec->z = ft_atod(ptr[2]);
	if (vec->x == INFINITY || vec->y == INFINITY || vec->z == INFINITY)
		return (free_2d(ptr), 0);
	if (flag == 1)
	{
		if (vec->x < -1.0 || vec->x > 1.0 || vec->y < -1.0
			|| vec->y > 1.0 || vec->z < -1.0 || vec->z > 1.0)
			return (free_2d(ptr), 0);
	}
	free_2d(ptr);
	return (1);
}

int	control_after_comma(char *str)
{
	int		i;
	int		j;

	i = 0;
	while (str[i])
	{
		if (str[i] == '.')
		{
			j = i + 1;
			while (str[j])
			{
				if (str[j] != '0')
					return (1);
				j++;
			}
		}
		i++;
	}
	return (0);
}

int	cntrldec1(char *str)
{
	int		i;
	char	**ptr;

	i = 0;
	if (ft_atod(str) != 1 || ft_atod(str) != -1)
		return (0);
	ptr = ft_split(str, ',');
	while (ptr[i])
	{
		if (control_after_comma(ptr[i]))
		{
			free_2d(ptr);
			return (1);
		}
		i++;
	}
	free_2d(ptr);
	return (0);
}

int	cntrldec0(char *str)
{
	if (ft_atod(str) == 1)
	{
		if (control_after_comma(str))
			return (1);
	}
	if (ft_atod(str) == 0 && str[0] == '-')
	{
		if (control_after_comma(str))
			return (1);
	}
	return (0);
}
