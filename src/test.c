#include "../inc/minirt.h"
#include <stdio.h>

void	mlx_stuffs(t_data * );
void	my_mlx_pixel_put( t_mlx *, int, int, int );
void	render_background( t_mlx *, int );


void	free_objects( t_data *genel) {
	int i = -1;

	 while (++i < (int) genel->obj_count) {
	 	if (genel->obj_set[i].obj)
	 		free(genel->obj_set[i].obj);
	 }
	 if (genel->obj_set)
	 	free(genel->obj_set);

}

int	free_exit( t_data *genel) {
	//(void)genel;
	if (genel)
	{
		if (genel->mlx->img_p)
		{
			mlx_destroy_image( genel->mlx->mlx_p, genel->mlx->img_p );
			if (genel->mlx->win_p)
				mlx_destroy_window( genel->mlx->mlx_p, genel->mlx->win_p );
		}
		free_objects(genel);
		if (genel->cam)
			free(genel->cam);
		if (genel->screen)
			free(genel->screen);
		if (genel->mlx)
			free(genel->mlx);
	}
	exit( 0 );
}

void	reset_camera( t_data *genel) {
	genel->cam->origin = genel->default_cam->origin;
	genel->cam->dir = genel->default_cam->dir;
	genel->cam->fov = genel->default_cam->fov;
}

void	cam_move( t_data *genel, int keycode ) {
	if ( keycode == KEY_D ) genel->cam->origin.x += SHIFT_VAL;
	else if ( keycode == KEY_A ) genel->cam->origin.x -= SHIFT_VAL;
	else if ( keycode == KEY_C ) genel->cam->origin.z += SHIFT_VAL;
	else if ( keycode == KEY_V ) genel->cam->origin.z -= SHIFT_VAL;
	else if ( keycode == KEY_W ) genel->cam->origin.y += SHIFT_VAL;
	else if ( keycode == KEY_S ) genel->cam->origin.y -= SHIFT_VAL;
	else if ( keycode == KEY_R ) reset_camera( genel );
}

void render(t_data *data);

int rgb_to_int(const t_color *rgb);

int handle_key(int keycode, t_data *genel)
{
	if ( keycode == KEY_ESC ) free_exit( genel );	//printf( "%d\n", keycode );
	cam_move( genel, keycode );

	render( genel );
	printf("camera position: x:%f y:%f z:%f\n", genel->cam->origin.x, genel->cam->origin.y, genel->cam->origin.z);

	mlx_put_image_to_window( genel->mlx->mlx_p, genel->mlx->win_p, genel->mlx->img_p, 0, 0 );
	return (0);
}

const t_obj	*find_first_obj(t_data *data, const t_ray *ray, double *t)
{
	const t_obj *ret = NULL;
	const t_obj *obj;
	double distance;
	double min_distance;
	int i;

	i = -1;
	min_distance = INF;
	while (++i < data->obj_count)
	{
		obj = &data->obj_set[i];
		distance = obj->f_intersects(ray, obj);
		if (distance > 0 && distance < min_distance)
		{
			min_distance = distance;
			ret = obj;
		}
	}
	*t = min_distance;
	return (ret);
}

int rgb_to_int(const t_color *rgb)
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	r = (unsigned char)rgb->r;
	g = (unsigned char)rgb->g;
	b = (unsigned char)rgb->b;
	return (r << 16 | g << 8 | b);
}

void	pixel_to_virtual(t_screen *s, int *x, int *y, t_vec3 *iter_coords)
{
	iter_coords->x = -1 + (2 * (*x + 0.5) / WIDTH) ;
	iter_coords->y = 1 - 2.0 * ((*y + 0.5 - s->y_pix_min) / HEIGHT);
	iter_coords->z = s->focal_length;
}

t_vec3 f_get_normal_sphere(const t_obj *obj, t_vec3 *hit_point)
{
	t_sphere *sp = obj->obj;
	t_vec3 normal;

	normal = v_substract(hit_point, &sp->center);
	normal = v_normalize(&normal);
	return (normal);
}

t_vec3 f_get_normal_plane(const t_obj *obj, t_vec3 *hit_point)
{
	t_plane *pl = (t_plane *)obj->obj;
	t_vec3 normal;

	(void)hit_point;
	normal = pl->normal;
	normal = v_normalize(&normal);
	return (normal);
}

t_vec3	f_get_normal_cylinder(const t_obj *obj, t_vec3 *hit_point)
{
	t_cylinder *cyl = (t_cylinder *)obj->obj;
	t_vec3 normal;
	t_vec3 oc;
	t_vec3 tmp2;

	oc = v_substract(hit_point, &cyl->origin);
	tmp2 = v_multiply(&cyl->dir, v_dot(&oc, &cyl->dir));
	normal = v_substract(&oc, &tmp2);
	normal = v_normalize(&normal);
	return (normal);
}

t_vec3 f_get_normal(const t_obj *obj, t_vec3 *hit_point)
{
	t_vec3 normal;

	if (obj->type == SPHERE)
		normal = f_get_normal_sphere(obj, hit_point);
	else if (obj->type == PLANE)
		normal = f_get_normal_plane(obj, hit_point);
	else if (obj->type == CYLINDER)
		normal = f_get_normal_cylinder(obj, hit_point);
	else
		normal = (t_vec3){0, 0, 0};
	return (normal);
}

bool any_obj_between_light_and_hit_point(t_data *data, t_vec3 *hit_point, t_vec3 *point_to_light, double t)
{
	t_ray ray;
	ray.origin = *hit_point;
	ray.dir = v_substract(point_to_light, hit_point);
	ray.dir = v_normalize(&ray.dir);
	
	const t_obj *hitted_obj;
	double t2;

	int i = -1;
	while (++i < data->obj_count)
	{
		hitted_obj = &data->obj_set[i];
		t2 = hitted_obj->f_intersects(&ray, hitted_obj);
		if (t2 >= -TOL && t2 < t)
			return (true);
	}
	return (false);
}

void	compute_illumination(t_data *data, t_color *color, t_vec3 *hit_point, t_vec3 *normal)
{
	t_vec3 point_to_light_dir;
	double theta;
	double intensity;

	point_to_light_dir = v_substract(&data->light->origin, hit_point);
	point_to_light_dir = v_normalize(&point_to_light_dir);
	theta = acos(v_dot(normal, &point_to_light_dir));
	if (theta > M_PI / 2.){
		*color = (t_color){0, 0, 0};
		intensity = 0;
	}
	else
		intensity = data->light->brightness * (1 - (theta / (M_PI / 2.)));
	if (intensity > 0)
	{
		color->r = color->r * intensity;
		color->g = color->g * intensity;
		color->b = color->b * intensity;
	}
}

void render(t_data *data)
{
	t_ray ray;
	ray.origin = data->cam->origin;

	const t_obj *hitted_obj;
	double t;

	t_color color;
	t_vec3 mapped_coords;

	t_vec3 hit_point = {0, 0, 0};
	t_vec3 normal = {0, 0, 0};
	t_vec3 point_to_light = {0, 0, 0};

	int pix_vertical_margin = (double)HEIGHT * (1 - (1 / data->screen->aspect_ratio)) / 2;

	int pix_y;
	int pix_x;


	pix_y = pix_vertical_margin - 1;
	while (++pix_y < data->screen->y_pix_max)
	{
		pix_x = -1;
		while (++pix_x < WIDTH)
		{
			t = INF;

			pixel_to_virtual(data->screen, &pix_x, &pix_y, &mapped_coords);

			ray.dir = v_normalize(&mapped_coords);

			hitted_obj = find_first_obj(data, &ray, &t);

			hit_point = v_multiply(&ray.dir, t);
			hit_point = v_add(&ray.origin, &hit_point);

			if (hitted_obj){
				color = hitted_obj->f_get_color(hitted_obj);
			
				point_to_light = v_substract(&hit_point, &data->light->origin);
				point_to_light = v_normalize(&point_to_light);
				normal = hitted_obj->f_get_normal(hitted_obj, &hit_point);
			
				compute_illumination(data, &color, &hit_point, &normal);
				

				// if(!any_obj_between_light_and_hit_point(data, &hit_point, &data->light->origin, t) ){
				// 	color = (t_color){}
				// }


				
			}
			else
				color = (t_color){118, 118, 118};
			

			my_mlx_pixel_put( data->mlx, pix_x, pix_y, rgb_to_int(&color) );

			pix_x++;
		}
		pix_y++;
	}

}

void	new_mlx(t_data *d) {
	if (!d->mlx)
		free_exit(d);
	d->mlx->mlx_p = mlx_init();
	if (!d->mlx->mlx_p)
		free_exit(d);
	d->mlx->win_p = mlx_new_window( d->mlx->mlx_p, WIDTH, WIDTH, "tnoyan's team" );
	if (!d->mlx->win_p)
		free_exit(d);
	d->mlx->img_p = mlx_new_image( d->mlx->mlx_p, WIDTH, WIDTH );
	if (!d->mlx->img_p)
		free_exit(d);
	d->mlx->addr = mlx_get_data_addr( d->mlx->img_p, &d->mlx->bpp, &d->mlx->line_len, &d->mlx->endian );
	if (!d->mlx->addr)
		free_exit(d);
}

void	init_camera_up_right(t_data *d) {
	d->screen->up = (t_vec3){0, 1, 0};
	d->screen->right = v_cross(&d->cam->dir, &d->screen->up);
	d->screen->up = v_cross(&d->screen->right, &d->cam->dir);
	d->screen->up = v_normalize(&d->screen->up);
	d->screen->right = v_normalize(&d->screen->right);
}

void	init_screen(t_data *d) {
	if (!d->screen)
		free_exit(d);
	d->screen->aspect_ratio = (double)ASPECT_RATIO_X / (double)ASPECT_RATIO_Y;
	d->screen->focal_length = 1 / tan((d->cam->fov / 2) * (M_PI / 180));
	d->screen->y_pix_min = ((double)HEIGHT - (WIDTH / d->screen->aspect_ratio)) / 2;
	d->screen->y_pix_max = (WIDTH / d->screen->aspect_ratio) + d->screen->y_pix_min;
	d->screen->forward = v_substract(&d->cam->dir, &d->cam->origin);
	d->screen->forward = v_normalize(&d->screen->forward);
	// https://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
	d->screen->up = (t_vec3){d->screen->forward.x, -d->screen->forward.z, d->screen->forward.y};
	d->screen->up = v_normalize(&d->screen->up);
	d->screen->right = v_cross(&d->screen->forward, &d->screen->up);
	d->screen->right = v_normalize(&d->screen->right);
}


void	main_loop(t_data *d) {

	init_screen(d);
	init_camera_up_right(d);

	render( d );
	mlx_put_image_to_window( d->mlx->mlx_p, d->mlx->win_p, d->mlx->img_p, 0, 0 );

	mlx_hook( d->mlx->win_p, 17, 1, &free_exit, d);
	mlx_hook( d->mlx->win_p, EVENT_KEY_PRESS, 1, &handle_key, d );
	mlx_loop( d->mlx->mlx_p );

}




// void __attribute__((destructor)) a(){
// 	system("leaks test.out");
// }
