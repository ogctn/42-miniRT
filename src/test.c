#include "rt.h"

void render_background( t_img *img, int color );
void mlx_stuffs(t_general *);

void cam_movements(int keycode, t_general *genel)
{
	if ( keycode == KEY_ESC || keycode == KEY_Q ) exit( 0 );	//printf( "%d\n", keycode );

	if ( keycode == KEY_A ) genel->cam->origin.x += SHIFT_VAL;
	else if ( keycode == KEY_D ) genel->cam->origin.x -= SHIFT_VAL;
	else if ( keycode == KEY_W ) genel->cam->origin.z += SHIFT_VAL;
	else if ( keycode == KEY_S ) genel->cam->origin.z -= SHIFT_VAL;
	else if ( keycode == KEY_UP ) genel->cam->origin.y += SHIFT_VAL;
	else if ( keycode == KEY_DOWN ) genel->cam->origin.y -= SHIFT_VAL;
}


t_vec3 canvas_to_viewport(int *x, int *y) // V_D represents the distance between the camera and the projection plane.
{
    return ((t_vec3){(*x)*V_W / C_W + (V_W/2), (*y)*V_H/C_H +  (V_H/2), V_D});
}


t_roots	r_intersects_sphere(const t_vec3 *d, const t_sphere *s, const t_general *genel);


t_color	trace_ray(t_general *genel, t_vec3 *d, double t_min, double t_max) // O the origin of the ray, 
{
	t_vec3 *o = &genel->cam->origin;
	double closest_t = INF;
	t_sphere *closest_sp = NULL;
	t_sphere *iter = genel->objs->sp;
	t_roots r;
	while (iter)
	{
		r = r_intersects_sphere(d, iter, genel);
		if ((r.t1 <= t_max && r.t1 >= t_min) && r.t1 < closest_t)
		{
			closest_t = r.t1;
			closest_sp = iter;
		}
		if ((r.t2 <= t_max && r.t2 >= t_min) && r.t2 < closest_t)
		{
			closest_t = r.t2;
			closest_sp = iter;
		}
		iter = iter->next;
	}
	if (!closest_sp)
		return ((t_color){.r = 155, .g = 155, .b = 155});
	return ((t_color){.r = closest_sp->color.r, .g = closest_sp->color.g, .b = closest_sp->color.b});
}

int rgb_to_int(t_color *rgb)
{
	int c;
	c = rgb->r;
	c = (c << 8) | rgb->g;
	c = (c << 8) | rgb->b;
	return (c);
}

void	my_pixel_put( t_img *img, int x, int y, int color );

void render_sphere(t_general *genel)
{
	t_vec3	d;
	t_color color;

	int x = -C_W/2;

	int y;

	while (x < C_W/2)
	{
		y = C_H/2;
		while (y > -C_H/2)
		{
			d = canvas_to_viewport(&x, &y);
			color = trace_ray(genel, &d, 1, INF);

			printf("%f %f\n", d.x, d.y);
			my_pixel_put( &(genel->mlx->img), d.x, d.y, rgb_to_int(&color) );
			y--;
		}
		x++;
	}
}

#include <stdio.h>

void	test_handler( int keycode, t_general *genel ) {

	//cam_movements(keycode, genel);
    
	//render_background( &(m->img), 0X18181AA * keycode / 100 + 300 );


	render_sphere(genel);

	mlx_put_image_to_window( genel->mlx->mlx_p, genel->mlx->win_p, genel->mlx->img.img, 0, 0 );

}



int main() {

	t_sphere s1 = {
		.center = (t_vec3){3, -3, 12}, 
		.r = 5,
		.color = (t_color){255, 0, 0} //red
	};
	t_sphere s2 = {
		.center = (t_vec3){5, 0,15}, 
		.r = 5,
		.color = (t_color){0, 0, 255} //blue
	};
	t_sphere s3 = {
		.center = (t_vec3){-5, 0, 15}, 
		.r = 5,
		.color = (t_color){0, 255, 0} //green
	};		s1.next = &s2;	s2.next = &s3;	s3.next = NULL;

	t_ray r = {0, 0, 0};
	t_cam cam = {
		.origin = (t_vec3){0, 0, -15},
		.dir = (t_vec3){0, 0, 1},
		.fov = 70,
		.ray = &r
	};
	t_general genel;
	genel.cam = &cam;
	t_objs obj = {.sp = &s1};
	genel.objs = &obj;
	t_mlx	m;
	genel.mlx = &m;
	mlx_stuffs( &genel);

	//t_viewport	vp = {};

}

t_roots	r_intersects_sphere(const t_vec3 *d, const t_sphere *s, const t_general *genel)
{
	const t_vec3 *o = &genel->cam->origin;
	t_vec3	co = v_substract(o, &s->center);

	double	a = v_dot(d, d);
	double 	b = 2 * v_dot(&co, d);
	double	c = v_dot(&co, &co) - (s->r * s->r);

	double	discriminant = b * b - 4 * a * c;
	if (discriminant < TOL)
		return ((t_roots){INF, INF});
	double t1 = (-b + sqrt(discriminant)) / (2 * a);
	double t2 = (-b - sqrt(discriminant)) / (2 * a);
	return ((t_roots){t1, t2});
}
