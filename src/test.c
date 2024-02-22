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
    return ((t_vec3){(*x)*V_WIDTH / C_W, (*y)*V_HEIGHT/C_H, V_D});
}


t_roots	r_intersects_sphere(const t_ray *r, double *t, const t_sphere *s);


t_color	trace_ray(t_vec3 o, t_vec3 d, double t_min, double t_max) // O the origin of the ray, 
{
	double closest_t = INF;
	t_sphere closest_sp = NULL;



}

int rgb_to_int(t_color *rgb)
{
	int color;
	color = rgb->r;
	color = (color << 8) | rgb->g;
	color = (color << 8) | rgb->b;
	return (color);
}

int rgb1(int r, int g, int b) {
  int c = r;
  c = (c << 8) | g;
  c = (c << 8) | b;
  return c;
}

void render_sphere(t_general *genel)
{
	t_vec3 D;
	t_color color;
	t_ray *r = genel->cam->ray;

	int x = -C_W/2;
	int y;

	while (x <= C_W/2)
	{
		y = -C_H/2;
		while (y <= C_H/2)
		{
			D = canvas_to_viewport(&x, &y);
			color = trace_ray(r->origin, r->dir, 1, INF);
			my_pixel_put( genel->mlx->img, x, y, rgb_to_int(&color) );
			y++;
		}
		x++;
	}

}



void	test_handler( int keycode, t_general *genel ) {

	cam_movements(keycode, genel);
    
	//render_background( &(m->img), 0X18181AA * keycode / 100 + 300 );


	render_sphere(genel);



	mlx_put_image_to_window( genel->mlx->mlx_p, genel->mlx->win_p, genel->mlx->img.img, 0, 0 );
}



int main() {
	t_general genel;

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
		.origin = (t_vec3){0, 0, 0},
		.dir = (t_vec3){0, 0, 1},
		.fov = 70,
		.ray = &r
	};
	genel.cam = &cam;
	genel.objs = &s1;

	mlx_stuffs( &genel);

	//t_viewport	vp = {};

}

t_roots	r_intersects_sphere(const t_ray *r, double *t, const t_sphere *s)
{
	t_vec3	co = v_substract(&r->origin, &s->center);
	
	double	a = v_dot(&r->dir, &r->dir);
	double 	b = 2 * v_dot(&co, &r->dir);
	double	c = v_dot(&co, &co) - (s->r * s->r);

	double	discriminant = b * b - 4 * a * c;
	if (discriminant < TOL)
		return ((t_roots){INF, INF});
	double t1 = (-b + sqrt(discriminant)) / (2 * a);
	double t2 = (-b - sqrt(discriminant)) / (2 * a);
	return ((t_roots){t1, t2});
}
