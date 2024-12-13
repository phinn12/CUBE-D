
#include "cub3d.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Kullanım: %s <harita_dosyası>\n", argv[0]);
        return 1;
    }
    int len = ft_strlen(argv[1]);
    if (len < 4 || ft_strncmp(argv[1] + len - 4, ".cub", 4) != 0) {
        printf("HATA: Harita dosyasının uzantısı .cub olmalı!\n");
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("Dosya açma hatası\n");
        return 1;
    }
    t_map_info map_info;
    char **map = NULL;
    t_point *size = malloc(sizeof(t_point));
    t_point *begin = malloc(sizeof(t_point));
    size->err = 0;
    char **lines = read_lines_from_file(fd);
    
    if (parse_and_validate_variables(lines, &map_info) == -1)
	{
        printf("Hatalı harita bilgileri!\n");
        return 1;
    }
    int fd_copy = open(argv[1], O_RDONLY);
    checkMap(fd_copy, &map, size, &map_info);
    close(fd_copy);

    find_starting_point(map, size, begin);
    //printf("Flood Fill öncesi Harita:\n");
   // print_map(map, size);

    flood_fill(map, size, begin);

    //printf("Flood Fill Sonrası Harita:\n");
    //print_map(map, size);

    //printf("Harita başarıyla kontrol edildi ve dolduruldu.\n");

    return 0;
}


void	checkMap(int fd, char ***map_out, t_point *size_out, t_map_info *map_info) 
{
	char *line;
	char **temp_map = NULL;
	int row = 0;
	bool is_map_started = false;
	t_point *map_start_and_end = malloc(sizeof(t_point));

	while ((line = get_next_line(fd))) 
	{
		if(line[0] == '\n' && !is_map_started)
			continue;
		if (ft_strstr(line, "F ")) 
		{
			map_info->floor_color = ft_strdup(line + 2);
			map_info->f_exists = true;
			
		} 
		if (ft_strstr(line, "C ")) 
		{
			map_info->ceiling_color = ft_strdup(line + 2);
			map_info->c_exists = true;
		}
		
		if (ft_strstr(line, "NO ")) 
		{
			map_info->north_texture = ft_strdup(line + 3);
			map_info->no_exists = true;
		} 
		if (ft_strstr(line, "SO ")) 
		{
			map_info->south_texture = ft_strdup(line + 3);
			map_info->so_exists = true;
		} 
		if (ft_strstr(line, "EA ")) 
		{
			map_info->east_texture = ft_strdup(line + 3);
			map_info->ea_exists = true;
		}
		if (ft_strstr(line, "WE ")) 
		{
			map_info->west_texture = ft_strdup(line + 3);
			map_info->we_exists = true;
		}
		if (!is_map_started && contains_valid_map_characters(line)) 
		{
			is_map_started = true;
		}
		if (is_map_started)
		{
			map_start_and_end->x = row;
			char **new_map = malloc(sizeof(char *) * (row + 1));
			for (int i = 0; i < row; i++) 
				new_map[i] = temp_map[i];
			new_map[row] = malloc(ft_strlen(line) + 1);
			ft_strcpy(new_map[row], line);
			free(line);
			free(temp_map);
			temp_map = new_map;
			row++;
		}
		else 
			free(line);
	}
	map_start_and_end->y = row;
	if (row == 0) 
	{
		printf("HATA: Harita bulunamadı.\n");
		return;
	}
	int col = ft_strlen(temp_map[0]);
	t_point size = {row, col, 1};
	map_spaces(&temp_map, &size);
	*map_out = temp_map;
	size_out->y = size.y;
	size_out->x = size.x;
	if (!check_map_headers(fd, map_info))
		printf("HATA: Haritanın başında yanlış var.\n");
	if (!check_top_row(*map_out, col))
		printf("HATA: Üst satırda yalnızca '1' karakteri olmalı.\n");
	if (!check_row_edges(*map_out, map_start_and_end->x)) 
	 	printf("HATA: Alt satırda yalnızca '1' karakteri olmalı.\n");
	if (!check_map_characters(*map_out, map_start_and_end->x)) 
		printf("HATA: Haritada geçersiz karakter var.\n");
	if (!check_zeros(*map_out, map_start_and_end->x, map_start_and_end->y)) 
		printf("HATA: 0 ın yanıda boşlık var lo\n");
}

void print_map(char **map, t_point *size)
{
    for (int y = 0; y < size->y; y++)
        printf("%s\n", map[y]);
}

void find_starting_point(char **map, t_point *size, t_point *begin) {
    for (int i = 0; i < size->y; i++)
	{
        for (int j = 0; j < size->x; j++) {
            if (map[i][j] == 'N' || map[i][j] == 'W' || map[i][j] == 'S' || map[i][j] == 'E')
			{
                begin->y = i;
                begin->x = j;
                return;
            }
        }
    }
    printf("Başlangıç noktası bulunamadı.\n");
    exit(1);
}


void	f_fill(char **map, t_point *size, int row, int col)
{
	if (map[row][col] == '1')
		return ;
	if (row < 0 || col < 0)
		return ;
	if (row + 1 >= size->y || col + 1 >= size->x)
		return ;
	if (map[row + 1][col] == ' ' ||
		(map[row][col] == '_'  && map[row + 1][col] == 0)||
		map[row - 1][col] == ' '||
		(map[row][col] == '_'  && map[row - 1][col] == 0)||
		map[row][col + 1] == ' ' ||
		(map[row][col] == '_'  && map[row][col + 1] == 0) ||
		map[row][col - 1] == ' ' ||
		(map[row][col] == '_'  && map[row][col - 1] == 0 ))
		
	{
		size->err = -1;
		return ;
	}
	if (map[row][col] != '0' && 
		map[row][col] != 'N' && 
		map[row][col] != 'S' && 
		map[row][col] != 'E' && 
		map[row][col] != 'W')
		return ;
	map[row][col] = '_';

	f_fill(map, size, row - 1, col);
	f_fill(map, size, row + 1, col);
	f_fill(map, size, row, col - 1);
	f_fill(map, size, row, col + 1);
}

void flood_fill(char **map, t_point *size, t_point *begin)
{
    f_fill(map, size, begin->y, begin->x);
    if(size->err == -1) {
        printf("Flood fill hatası\n");
        return;
    }
    fill_flod(map, size, begin->y, begin->x);
    if (!check_filled_map(map, size))
        printf("FİLLFLOOD sonrası harita hatası bulundu!\n");
	
}

char *ft_strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while (*src) {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}

bool is_valid_character(char c)
{
    return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ' || c == '\n');
}

void fill_flod(char **map, t_point *size, int row, int col)
{
    if (!map || !size) {
        size->err = -1;
        return;
    }

    if (row < 0 || col < 0 || row >= size->y || col >= size->x)
        return;

    if (map[row][col] != '*' && map[row][col] != '0' && map[row][col] != 'N' &&
        map[row][col] != 'S' && map[row][col] != 'E' && map[row][col] != 'W' &&
        map[row][col] != '1' && map[row][col] != '_' ) 
    {
        size->err = -1;
        return;
    }

    if (map[row][col] == '0' || map[row][col] == 'N' || map[row][col] == 'S' || 
        map[row][col] == 'E' || map[row][col] == 'W' || map[row][col] == '1' || map[row][col] == '_' || map[row][col] == '0') 
    {
        map[row][col] = '*';
        fill_flod(map, size, row - 1, col);
        fill_flod(map, size, row + 1, col);
        fill_flod(map, size, row, col - 1);
        fill_flod(map, size, row, col + 1);
    }
}

bool check_filled_map(char **map, t_point *size) 
{
    for (int row = 0; row < size->y; row++) 
    {
        for (int col = 0; col < size->x; col++) 
        {
            if (map[row][col] != '*' && map[row][col] != ' ' && map[row][col] != '\n' && map[row][col] != '\0') 
                return false;
        }
    }
    return true;
}