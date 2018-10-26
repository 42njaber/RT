/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaber <neyl.jaber@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 15:19:13 by njaber            #+#    #+#             */
/*   Updated: 2018/10/26 20:54:04 by njaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "libgxns.h"
# include "common.h"

# define DEFAULT_WIDTH 1920
# define DEFAULT_HEIGHT 1080

# define WG_SIZE 256

extern const char	*g_nul;

typedef struct		s_node {
	char	name[32];
	t_hmap	values;
	char	type;
}					t_node;

typedef struct s_btree	t_btree;

struct				s_btree {
	t_btree		*parent;
	t_btree		*b0;
	t_btree		*b1;
	short		val;
};

typedef struct		s_png {
	t_ivec	dim;
	t_uint	*buf;
	t_uchar	init;
	t_uchar	bdepth;
	t_uchar	ctype;
	t_uchar	pxsize;
	t_uchar	compression;
	t_uchar	filter;
	t_uchar	interlace;
	t_ivec	aratio;
	t_uchar	aunit;
	t_uint	block_nb_;
	t_uint	block_len_;
	t_uchar	block_type_[4];
	t_list	*idat_blocks_;
	t_list	**idat_last_;
	t_uchar	*zlib_stream_;
	size_t	zlib_len_;
	int		*codes_;
	size_t	codes_len_;
	size_t	data_size_;
	t_uchar	*data_;
	t_btree	*ltree;
	t_btree	*dtree;
	t_btree	*cltree;
}					t_png;

typedef struct		s_scene {
	t_vec3		start_pos;
	t_vec2		start_rot;
	int			obj_pbufsize;
	int			spot_pbufsize;
	int			nobjs;
	int			nspots;
	t_obj		*objs;
	t_spot		*spots;
	float		ambiant_light;
	t_color		sky_color;
}					t_scene;

typedef struct		s_view {
	t_vec3		pos;
	t_vec2		rot;
	t_ivec		size;
	t_climg		scene_buf;
	cl_mem		objbuf;
	cl_mem		spotbuf;
	int			nobjs;
	int			nspots;
	t_obj		*objs;
	t_spot		*spots;
	t_set		set;
}					t_view;

typedef struct	s_ptr {
	t_win			*win;
	t_ocl			*opencl;
	t_kernel		*kernel;
	t_climg			scene;
	char			keys[512];
	int				button;
	t_ivec			mouse_pos;
	int				update;
	t_gui			gui;
	t_hmap			textures;
	t_img			texture_mem;
	cl_mem			cl_texture_sizes;
	t_scene			*current_scene;
	t_hmap			scenes;
	t_view			view;
}				t_ptr;

t_ptr			*get_p(void);

void			create_obj_memobjs(t_view *view, cl_context context);
void			create_spot_memobjs(t_view *view, cl_context context);
t_kernel		*create_kernel(t_ptr *p);
cl_mem			cl_img2d(t_ocl *opencl, cl_mem_flags flags,
														t_ivec size, int *err);
cl_mem			create_image_array(t_ocl *opencl, int len,
											t_ivec size, int *err);

void			free_scene(void **data);
t_scene			*new_scene(t_ptr *p, char *name);

int				read_path(t_ptr *p, char *path);
int				parse_scene_file(t_scene *scene, int fd);
t_scal			parsef(char **pos);
t_vec2			parse2f(char **pos);
t_vec3			parse3f(char **pos);
t_color			parsecolor(char **pos);
int				parsetype(char **pos);
char			*parsestr(char **pos);

int				get_next_xml_node(t_node *tag, char **pos, char strict);
void			xml_set_read_buf(char *file, int len);

int				default_check_node(t_scene *scene, t_node *onode, char **pos);
int				parse_config_node(t_scene *scene, t_node *onode, char **pos);
int				parse_objlist_node(t_scene *scene, t_node *onode, char **pos);
int				parse_spotlist_node(t_scene *scene, t_node *onode, char **pos);

t_spot			*default_spot(t_scene *scene);
t_obj			*default_obj(t_scene *scene);
int				get_elem_id(t_ptr *p, const char *type, void *elem);

void			copy_scene_data(t_view *view, t_scene *scene);
void			cleanup_view(t_view *view);
void			init_scene(t_ptr *p);

void			init_textures(t_ptr *p);
void			init_struct(t_ptr *p);
void			init_guielems(t_ptr *p);
void			gen_thumbnails(t_ptr *p);
void			set_hooks(t_ptr *p);

void			key_callback(GLFWwindow *win, int key,
										int scancode, int action);
void			mouse_callback(GLFWwindow *win,
								int button, int action, int mods);
void			motion_callback(GLFWwindow *win, double xpos, double ypos);
void			*loop_hook(t_ptr *p);
void			events_loop(t_ptr *p);
void			move(t_ptr *p);

void			process_image(t_ptr *p);
void			update_scene(t_ptr *p);
void			process_scene_opencl(t_view *view, t_kernel *kernel);

int				does_intersect(t_obj *obj, t_vec3 origin, t_vec3 dir, float *t);
t_vec3			get_normal(t_obj *obj, t_vec3 v);

void			generate_cam_matrices(t_view *view);
void			generate_obj_matrices(t_view *view);

void			free_content(void *cont, size_t size);

t_png			*decode_png(char *path);
int				validate_crc(int fd, t_uchar *buf, t_png *png);
int				checksum(t_uchar *buf, t_uint len, t_uchar *alder);
int				destroy_png(t_png **png);
int				parse_ihdr(int fd, t_png *png);
int				parse_phys(int fd, t_png *png);
int				parse_idat(int fd, t_png *png);
int				parse_iend(int fd, t_png *png);
int				parse_unkown(int fd, t_png *png);

t_uint			get_next_bits(t_uchar *buf, t_uint *pos, t_uint len);
int				decompress_block(t_png *png,
									t_uchar *stream, t_uint *pos);
int				parse_zlib(t_png *png);
int				gen_default_tree(t_png *png);
int				read_tree(t_png *png, t_uchar *stream, t_uint *pos);
int				create_cltree(t_btree **cltree, t_uchar *clens);
int				create_tree(t_btree **tree,
										t_uchar *lens, t_ushort max);
short			read_next(t_png *png, t_uchar *stream,
										t_uint *pos, t_btree *tree);
void			free_tree(t_btree **tree);
int				read_codes(t_png *png, t_uchar *buf, t_uint *pos);
int				recompose_image(t_png *png);

#endif
