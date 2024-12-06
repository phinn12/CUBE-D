#ifndef CUB3D_H
# define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "gnl/get_next_line.h"
#include "libft/libft.h"

typedef struct t_point
{
    int y;
    int x;
	int err;
}
t_point;typedef struct s_rgb
{
    int r;
    int g;
    int b;

} t_rgb;

typedef struct s_map_info {
    char *north_texture;
    char *south_texture;
    char *west_texture;
    char *east_texture;
    char *floor_color;
    char *ceiling_color;
    bool no_exists;
    bool so_exists;
    bool we_exists;
    bool ea_exists;
    bool f_exists;
    bool c_exists;
    int *fclor;
    int *cclor;
} t_map_info;


char	*ft_strstr(const char *haystack, const char *needle);
bool	contains_valid_map_characters(const char *line);
void	f_fill(char **map, t_point *size, int row, int col);
void	print_map(char **map, t_point *size);
void	checkMap(int fd, char ***map_out, t_point *size_out, t_map_info *map_info); 
char	*ft_strcpy(char *dest, const char *src);
bool	check_top_row(char **map, int col);
bool	check_bottom_row(char **map, int row, int col);
bool	check_zeros(char **map, int row, int col);
bool	is_valid_character(char c);
bool	check_map_characters(char **map, int row);
void	map_spaces(char ***map, t_point *size);
void	flood_fill(char **map, t_point *size, t_point *begin);
bool	check_map_headers(int fd, t_map_info *map_info);
void	find_starting_point(char **map, t_point *size, t_point *begin);
void	fill_flod(char **map, t_point *size, int row, int col);
bool	check_filled_map(char **map, t_point *size);
int		check_texture(char *path);
int     check_rgb_values(char *rgb);
int		check_map_textures_and_colors(t_map_info *map_info);
void	set_texture_or_color(char **tokens, t_map_info *map_info);
int		parse_and_validate_variables(char **lines, t_map_info *map_info);
void	freedouble(char **array);
int		ft_strcmp(const char *s1, const char *s2);
char	**read_lines_from_file(int fd);
bool    check_row_edges(char **map, int row);
int     is_positive_number(const char *str);
#endif