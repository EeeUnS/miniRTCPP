#include "MlxManager.h"
#include "stdio.h"
#include "SceneEditer.h"

# define ESC (53)
# define ENTER (36)
# define SPACE (49)
# define LARROW (123)
# define RARROW (124)
# define UARROW (126)
# define DARROW (125)
# define KEY_A (0)
# define KEY_B (11)
# define KEY_C (8)
# define KEY_D (2)
# define KEY_E (14)
# define KEY_F (3)
# define KEY_G (5)
# define KEY_H (4)
# define KEY_I (34)
# define KEY_J (38)
# define KEY_K (40)
# define KEY_L (37)
# define KEY_M (46)
# define KEY_N (10)
# define KEY_O (31)
# define KEY_P (35)
# define KEY_Q (12)
# define KEY_R (15)
# define KEY_S (1)
# define KEY_T (17)
# define KEY_U (32)
# define KEY_V (9)
# define KEY_W (13)
# define KEY_X (7)
# define KEY_Y (16)
# define KEY_Z (6)

# define KEY_1 (18)
# define KEY_2 (19)
# define KEY_3 (20)
# define KEY_4 (21)
# define KEY_5 (23)
# define KEY_6 (22)
# define KEY_7 (26)
# define KEY_8 (28)
# define KEY_9 (25)
# define KEY_0 (29)

# define MOUSE_LEFT        (1)
# define MOUSE_RIGHT       (2)
# define MOUSE_WHELL_CLICK (3)
# define MOUSE_WHELL_DOWN  (4)
# define MOUSE_WHELL_UP    (5)

typedef struct s_mlx_manager
{
	void* mlx;
	void* win;
	t_image				image;
}					t_mlx_manager;

void	mlx_renew_image(void)
{
	//t_mlx_manager* mlx;

	//mlx = get_mlx();
	//mlx_destroy_image(mlx->mlx, mlx->image.img);
	//mlx->image.img = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT);
	//mlx->image.addr = mlx_get_data_addr(mlx->image.img, &(mlx->image.bpp), \
	//									& (mlx->image.line), &(mlx->image.endian));
	//ray_cast();
	//mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image.img, 0, 0);
}


static int	chage_to_editmode(int keycode)
{
	SceneEditer* editer;

	editer = GetSceneEditer();
	if (keycode == KEY_D)
	{
		editer->debug = (eDebugMode)(((int(editer->debug) + 1) % 3);
		if (editer->debug == eDebugMode::NONE)
			printf("\033[1;3m\tDEBUG MODE OFF\033[0m\n");
		else if (editer->debug == eDebugMode::SIMPLE)
			printf("\033[1;3m\tDEBUG MODE ON : SIMPLE mode\033[0m\n");
		else if (editer->debug == eDebugMode::DETAIL)
			printf("\033[1;3m\tDEBUG MODE ON : DETAIL mode\033[0m\n");
		if (editer->debug != eDebugMode::NONE)
			printf("\033[1;3m\tclick pixels with mouse\n \nright button to see detail\033[0m\n");
		return (true);
	}
	editer->debug = eDebugMode::NONE;
	editer->edit = ceil(fmax(WIN_WIDTH, WIN_HEIGHT) / 500);
	printf("\033[1;3m\tNow in editting mode\n");
	printf("\tSelect scene to edit -> C : cam, L : light, O : objs\n");
	printf("\tPress R to render image\033[0m\n");
	mlx_renew_image();
	return (true);
}

static int	change_to_rendermode(void)
{
	SceneEditer* scene_editer;

	scene_editer = GetSceneEditer();
	scene_editer->edit = 0;
	scene_editer->targetScene = eTarget::NONE;
	scene_editer->selectedObject = NULL;
	scene_editer->selectedObject = NULL;
	printf("\033[1;3m\tREDERING.....\033[0m\n");
	mlx_renew_image();
	printf("\033[1;3m\tDONE\n");
	printf("\tpress E to edit scene or press D to change debug mode\033[0m\n");
	return (true);
}

static int	set_edit_scene(int keycode)
{
	SceneEditer* editer;

	editer = GetSceneEditer();
	if (keycode == KEY_C)
		editer->targetScene = CAM;
	else if (keycode == KEY_L)
		editer->targetScene = LIGHT;
	else if (keycode == KEY_O)
		editer->targetScene = OBJ;
	else
		return (false);
	if (keycode == KEY_C)
		edit_mode_info_camera();
	else if (keycode == KEY_L)
	{
		edit_mode_info_light();
		editer->selectedObject = get_scene()->light;
		print_single_light(editer->selectedObject);
	}
	else if (keycode == KEY_O)
		edit_mode_info_object();
	mlx_renew_image();
	return (true);
}

static int	move_target_scene(int keycode)
{
	const enum e_target	targetScene = GetSceneEditer()->targetScene;

	if (targetScene == CAM)
		return (mlx_move_cam(keycode));
	else if (targetScene == LIGHT)
		return (mlx_move_light(keycode));
	else if (targetScene == OBJ)
		return (mlx_move_obj(keycode));
	else
		return (false);
}

int	keydown(int keycode)
{
	SceneEditer* editer;

	editer = GetSceneEditer();
	if (keycode == ESC)
		//terminate_mlx(NULL);
	else if (editer->edit == 0 && (keycode == KEY_E || keycode == KEY_D))
		return (chage_to_editmode(keycode));
	else if (editer->edit != 0 && keycode == KEY_R)
		return (change_to_rendermode());
	else if (editer->edit != 0 && editer->targetScene == eTarget::NONE)
		return (set_edit_scene(keycode));
	else if (editer->edit != 0 && keycode == ENTER)
	{
		editer->targetScene = eTarget::NONE;
		printf("\033[1;3m\tSCENE EDITING DONE! press R to render\n");
		printf("\tor Select scene to edit -> C : cam, L : light, \
O : objs\033[0m\n");
		mlx_renew_image();
		return (1);
	}
	else if (editer->edit != 0 && editer->targetScene == eTarget::LIGHT \
			 && keycode == SPACE)
		mlx_replace_light();
	return (move_target_scene(keycode));
}


static int	move_pos(Vector4D* pos, int keycode)
{
	const int	move_stride = 1;

	if (keycode == KEY_D)
		pos->SetX(pos->GetX() + move_stride);
	else if (keycode == KEY_A)
		pos->SetX(pos->GetX() - move_stride);
	else if (keycode == KEY_W)
		pos->SetZ(pos->GetZ() + move_stride);
	else if (keycode == KEY_S)
		pos->SetZ(pos->GetZ() - move_stride);
	else if (keycode == UARROW)
		pos->SetY(pos->GetY() - move_stride);
	else if (keycode == DARROW)
		pos->SetY(pos->GetY() + move_stride);
	else
		return (false);
	return (true);
}

int	mlx_move_cam(int keycode)
{
	t_mat33		transform;
	t_scene* scene;

	scene = get_scene();
	if (keycode == KEY_Q)
	{
		transform.r1 = rotate_vec3_deg(make_v3(0, 0, 1), 1, make_v3(1, 0, 0));
		transform.r2 = rotate_vec3_deg(make_v3(0, 0, 1), 1, make_v3(0, 1, 0));
		transform.r3 = make_v3(0, 0, 1);
		transform_to_cam_cord(scene, mat33_trans(transform));
	}
	else if (keycode == KEY_E)
	{
		transform.r1 = rotate_vec3_deg(make_v3(0, 0, 1), -1, make_v3(1, 0, 0));
		transform.r2 = rotate_vec3_deg(make_v3(0, 0, 1), -1, make_v3(0, 1, 0));
		transform.r3 = make_v3(0, 0, 1);
		transform_to_cam_cord(scene, mat33_trans(transform));
	}
	else if (move_pos(&(scene->cam->pos), keycode) == false)
		return (false);
	mlx_renew_image();
	return (true);
}

int	mlx_move_light(int keycode)
{
	if (move_pos(&(GetSceneEditer()->selectedObject->o), keycode) == false)
		return (false);
	mlx_renew_image();
	return (true);
}

int	mlx_move_obj(int keycode)
{
	const t_vec3	axis = make_v3(0, 0, 1);
	Object* selectedObject;

	selectedObject = GetSceneEditer()->selectedObject;
	if (selectedObject == NULL)
		return (false);
	else if (keycode == KEY_C)
		selectedObject->disruption ^= 0b1;
	else if (keycode == KEY_Q)
		rotate_quadrics(selectedObject, axis, -3);
	else if (keycode == KEY_E)
		rotate_quadrics(selectedObject, axis, 3);
	else if (move_pos(&(selectedObject->org), keycode) == false)
		return (false);
	mlx_renew_image();
	return (true);
}

void	mlx_replace_light(void)
{
	t_light** selectedObject;

	selectedObject = &(GetSceneEditer()->selectedObject);
	*selectedObject = (*selectedObject)->next;
	if (*selectedObject == NULL)
		*selectedObject = get_scene()->light;
	printf("light changed\n");
	print_single_light(*selectedObject);
}


void	edit_mode_info_camera(void)
{
	printf("\033[1;3m\t edit mode : CAMERA\n");
	printf("\t\t<MOVE>\n");
	printf("\t\tpress W to move cam inward\n");
	printf("\t\tpress S to move cam outward\n");
	printf("\t\tpress A to move cam left\n");
	printf("\t\tpress D to move cam right\n");
	printf("\t\tpress ⬆︎ to move cam up\n");
	printf("\t\tpress ⬇ to move cam down\n");
	printf("\t\t<ROTATE>\n");
	printf("\t\tpress Q to rotate cam_view counter_clockwise( ⤺ )\n");
	printf("\t\tpress E to rotate cam_view clockwise( ⤼ )\n");
	printf("\t\tdrag mouse with left button to rotate cam_view\n");
	printf("\t\t<FOV>\n");
	printf("\t\tuse mouse wheel to change fov\n");
	printf("\tPress Enter to finish edit camera\033[0m\n");
}

void	edit_mode_info_object(void)
{
	printf("\033[1;3m\t edit mode : OBJECT\n");
	printf("\t\t<SELECT>\n");
	printf("\t\tclick object to select\n");
	printf("\t\t<MOVE>\n");
	printf("\t\tpress W to move obj inward\n");
	printf("\t\tpress S to move obj outward\n");
	printf("\t\tpress A to move obj left\n");
	printf("\t\tpress D to move obj right\n");
	printf("\t\tpress ⬆︎ to move obj up\n");
	printf("\t\tpress ⬇ to move obj down\n");
	printf("\t\t<ROTATE>\n");
	printf("\t\tpress Q to rotate obj counter_clockwise( ⤺ )\n");
	printf("\t\tpress E to rotate obj clockwise( ⤼ )\n");
	printf("\t\tdrag mouse with left button to rotate obj\n");
	printf("\t\t<CHECKER BOARD>\n");
	printf("\t\tpress C to disrupt color\n");
	printf("\t\t<SIZE>\n");
	printf("\t\tuse mouse wheel to change size\n");
	printf("\tPress Enter to finish edit object\033[0m\n");
}

void	edit_mode_info_light(void)
{
	printf("\033[1;3m\t edit mode : LIGHT\n");
	printf("\t\t<SELECT>\n");
	printf("\t\tpress SPACE to change light\n");
	printf("\t\t<MOVE>\n");
	printf("\t\tpress W to move light inward\n");
	printf("\t\tpress S to move light outward\n");
	printf("\t\tpress A to move light left\n");
	printf("\t\tpress D to move light right\n");
	printf("\t\tpress ⬆︎ to move light up\n");
	printf("\t\tpress ⬇ to move light down\n");
	printf("\t\t<BRIGHTNESS>\n");
	printf("\t\tuse mouse wheel to change brightness\n");
	printf("\tPress Enter to finish edit light\033[0m\n");
}


static int	debug_single_raycast(int x, int y)
{
	const Cam* cam = get_scene()->cam;
	t_ray		ray;
	float		d;

	printf("DEBUG MODE\n");
	d = ((float) WIN_WIDTH / 2) / tan(cam->hfov / 2);
	ray.dir = make_v3(x - WIN_WIDTH / 2, y - WIN_HEIGHT / 2, d);
	ray.dir = v3_normalize(ray.dir);
	ray.org = cam->pos;
	single_ray_cast(ray, RECURSE);
	return (true);
}

static Object* select_object(int x, int y)
{
	const t_scene* scene = get_scene();
	SceneEditer* editor;
	
	float			d;

	editor = GetSceneEditer();
	editor->selectedObject = NULL;
	d = ((float) WIN_WIDTH / 2) / tan(scene->cam->hfov / 2);
	
	Ray	ray;
	ray.dir = make_v3(x - WIN_WIDTH / 2, y - WIN_HEIGHT / 2, d);
	ray.dir = v3_normalize(ray.dir);
	ray.org = scene->cam->pos;
	get_intersect_distance(scene->quads, \
						   & (editor->selectedObject), ray);
	print_single_quadrics(editor->selectedObject);
	return (editor->selectedObject);
}

int	mousedown(int button, int x, int y)
{
	SceneEditer* editer;

	editer = GetSceneEditer();
	if (x > 0 && y > 0)
		printf("mouse clicked\n");
	if (x < 0 || y < 0 || x > WIN_WIDTH || y > WIN_HEIGHT || \
		(((editer->edit == false) || (editer->targetScene == eTarget::NONE)) \
		 && (editer->debug == false)))
		return (false);
	editer->clicked = button;
	editer->prev_pixel[0] = x;
	editer->prev_pixel[1] = y;
	if (editer->debug != NONE && button == MOUSE_RIGHT)
		return (debug_single_raycast(x, y));
	if (button == MOUSE_LEFT && editer->targetScene == OBJ)
		select_object(x, y);
	else if (button == MOUSE_WHELL_DOWN || button == MOUSE_WHELL_UP)
	{
		if (mlx_mouse_wheel(button) == false)
			return (false);
	}
	else
		return (false);
	mlx_renew_image();
	return (true);
}

int	mouseup(int button, int x, int y)
{
	if (x > 0 && y > 0 && button > 0)
	{
		GetSceneEditer()->clicked = 0;
		printf("mouse unclicked\n");
	}
	return (false);
}

int	mousemove(int x, int y)
{
	t_vec3			axis;
	SceneEditer* editer;

	editer = GetSceneEditer();
	if (editer->edit == false || editer->clicked != 1)
		return (false);
	double dx = x - editer->prev_pixel[0];
	double dy = y - editer->prev_pixel[1];
	editer->prev_pixel[0] = x;
	editer->prev_pixel[1] = y;
	if (dx == 0 && dy == 0)
		return (0);
	Vector4D axis = v3_normalize(make_v3(-dy, dx, 0));
	if (fabs(dx) > fabs(dy))
		axis = v3_normalize(make_v3(0, dx, 0));
	if (editer->targetScene == eTarget::CAM)
		transform_to_cam_cord(get_scene(), mat33_trans(rotation_mat33(axis, -3)));
	else if (editer->targetScene == eTarget::OBJ && editer->selectedObject != NULL)
		rotate_quadrics(editer->selectedObject, axis, -3);
	print_info_camera(get_scene()->cam);
	mlx_renew_image();
	return (1);
}


static void	wheel_camera(int keycode)
{
	int		stride;
	t_cam* cam;

	cam = get_scene()->cam;
	stride = 5;
	cam->hfov *= (180 / M_PI);
	if (keycode == 5)
		stride = -5;
	if (cam->hfov + stride > 20 \
		&& cam->hfov + stride < 160)
		cam->hfov += stride;
	printf("FOV : %.2f(deg)\n", (cam->hfov));
	cam->hfov /= (180 / M_PI);
	cam->cam_proportion = (WIN_WIDTH / 2) / tan(cam->hfov / 2);
}

static int	wheel_plane(Object* obj, float stride)
{
	if (obj == NULL)
		return (false);
	if (v3_l2norm(obj->coefs.col4) < EPSILON)
		return (true);
	obj->coefs.col4 = v4_mul(obj->coefs.col4, stride);
	obj->coefs.col1.w *= stride;
	obj->coefs.col2.w *= stride;
	obj->coefs.col3.w *= stride;
	print_single_quadrics(obj);
	return (true);
}

static int	wheel_object(Object* obj, int keycode)
{
	double	stride;
	t_mat44	mat;

	stride = 0.1;
	if (obj == NULL)
		return (false);
	if (keycode == 5)
		stride = 1 - stride;
	else
		stride = 1 + stride;
	stride = 1 / stride;
	if (obj->type == Q_PLANE)
		return (wheel_plane(obj, stride));
	mat.col1 = make_v4(1, 0, 0, 0);
	mat.col2 = make_v4(0, 1, 0, 0);
	mat.col3 = make_v4(0, 0, 1, 0);
	mat.col4 = make_v4(0, 0, 0, 1 / stride);
	if (obj->range_z[1] - obj->range_z[0] > EPSILON)
	{
		obj->range_z[0] /= stride;
		obj->range_z[1] /= stride;
		obj->coefs = mul_mat44(mat, mul_mat44(obj->coefs, mat));
		obj->bounding_radius_sphere /= (stride * stride);
	}
	print_single_quadrics(obj);
	return (true);
}

static int	wheel_light(t_light* light, int keycode)
{
	double	stride;

	if (light == NULL)
		return (false);
	if (keycode == 5)
		stride = -0.1;
	else
		stride = 0.1;
	if (light->bright + stride >= 0 \
		&& light->bright + stride <= 1)
	{
		light->bright += stride;
		printf("\tbrightness : \033[2;38;2;%d;%d;%dm%.2f\033[0m\n", \
			   light->color.red, \
			   light->color.green, \
			   light->color.blue, \
			   light->bright);
	}
	return (true);
}

int	mlx_mouse_wheel(int keycode)
{
	if (keycode < 4)
		return (false);
	else if (GetSceneEditer()->targetScene == CAM)
		wheel_camera(keycode);
	else if (GetSceneEditer()->targetScene == OBJ)
		return (wheel_object(GetSceneEditer()->selectedObject, keycode));
	else if (GetSceneEditer()->targetScene == LIGHT)
		return (wheel_light(GetSceneEditer()->selectedObject, keycode));
	else
		return (false);
	return (true);
}



void	run_mlx(void)
{
	t_mlx_manager* mlx;

	printf("\033[3;32m\tRUN MLX\033[0m\n");
	mlx = get_mlx();
	printf("\033[3;32m\tRay Cast\033[0m\n");
	ray_cast();
	printf("\033[3;32m\tPut image\033[0m\n");
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->image.img, 0, 0);
	printf("\033[3;32m\tSetting Hooks\033[0m\n");
	mlx_hook(mlx->win, 17, 0, terminate_mlx, NULL);
	mlx_hook(mlx->win, 2, (1L << 0), keydown, mlx);
	mlx_hook(mlx->win, 4, (1L << 2), mousedown, mlx);
	mlx_hook(mlx->win, 5, (1L << 3), mouseup, mlx);
	mlx_hook(mlx->win, 6, (1L << 6), mousemove, mlx);
	printf("\033[3;32m\tLoop start\033[0m\n");
	time_check_end_all();
	printf("\033[1;3m\tpress E to edit scene or press D \
to change debug mode\033[0m\n");
	mlx_loop(mlx->mlx);
}