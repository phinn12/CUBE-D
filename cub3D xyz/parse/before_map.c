#include "cub3d.h"

bool contains_valid_map_characters(const char *line)
{
    int i = 0;
    while (line[i])
	{
        if (line[i] != '0' && line[i] != '1' && line[i] != 'N' &&
            line[i] != 'S' && line[i] != 'E' && line[i] != 'W' &&
            line[i] != ' ' && line[i] != '\n' && line[i] != 'A' && line[i] != 'F' && line[i] != 'C')
			{
            return false;
        }
        i++;
    }
    return true;
}

bool check_map_headers(int fd, t_map_info *map_info)
{
    char *line;

    while ((line = get_next_line(fd)))
    {
		if(line[0] == '\n')
			continue;
        if ((ft_strnstr(line, "NO ", 3)))
        {
            map_info->north_texture = ft_strdup(line + 3);
            map_info->no_exists = true;
        }
        if ((ft_strnstr(line, "SO ", 3)))
        {
            map_info->south_texture = ft_strdup(line + 3);
            map_info->so_exists = true;
        }
        if ((ft_strnstr(line, "WE ", 3)))
        {
            map_info->west_texture = ft_strdup(line + 3);
            map_info->we_exists = true;
        }
        if ((ft_strnstr(line, "EA ", 3)))
        {
            map_info->east_texture = ft_strdup(line + 3);
            map_info->ea_exists = true;
        }
        if ((ft_strnstr(line, "F ", 2)))
        {
            map_info->floor_color = ft_strdup(line + 2);
            map_info->f_exists = true;
        }
        if ((ft_strnstr(line, "C ", 2)))
        {
            map_info->ceiling_color = ft_strdup(line + 2);
            map_info->c_exists = true;
        }
        free(line);
    }
    return (map_info->no_exists && map_info->so_exists && map_info->we_exists &&
            map_info->ea_exists && map_info->f_exists && map_info->c_exists);
}


char *ft_strstr(const char *haystack, const char *needle)
{
    if (*needle == '\0') {
        return (char *)haystack;
    }

    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;

        while (*n && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char *)haystack;
        }

        haystack++;
    }

    return NULL;
}
