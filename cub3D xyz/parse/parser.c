#include "cub3d.h"

int check_texture(char *path)
{
    int fd;

    fd = open(path, O_RDONLY);
    if (fd == -1)
        return (-1);
    close(fd);
    return (0);
}

int check_rgb_values(char *rgb)
{
    int value;

    if (!rgb)
        return (-1);
    value = ft_atoi(rgb);
    if (value < 0 || value > 255)
        return (-1);
    return (value);
}

int parse_and_validate_variables(char **lines, t_map_info *map_info)
{
    char **tokens;
    int i = 0;
    int texture_or_color_checked = 0;
    int line_count = 0;

    map_info->fclor = malloc(sizeof(int) * 3);
    map_info->cclor = malloc(sizeof(int) * 3);
    if (!map_info->fclor || !map_info->cclor)
    {
        printf("Hata: Bellek tahsisi başarısız oldu!\n");
        return (-1);
    }

    while (lines[i])
    {
        if (lines[i][0] == '\0' || lines[i][0] == '\n')
        {
            i++;
            continue;
        }

        if ((lines[i][0] == 'N' || lines[i][0] == 'S' || lines[i][0] == 'W' || lines[i][0] == 'E') && line_count < 6)
        {
            tokens = ft_split(lines[i], ' ');
            set_texture_or_color(tokens, map_info);
            freedouble(tokens); // Bellek sızıntısını önlemek için
            texture_or_color_checked = 1;
        }

        if (lines[i][0] == 'F' || lines[i][0] == 'C')
        {
            char **rgb_values = ft_split(lines[i] + 1, ',');
            int j = 0;

            while (rgb_values[j])
            {
                char *trimmed_value = ft_strtrim(rgb_values[j], " ");
                if (!trimmed_value || !is_positive_number(trimmed_value))
                {
                    printf("Hata: Renk değeri pozitif bir sayı değil: %s\n", trimmed_value);
                    free(trimmed_value);
                    freedouble(rgb_values);
                    return (-1);
                }

                int value = check_rgb_values(trimmed_value);
                if (value == -1)
                {
                    printf("Hata: Renk değeri 0-255 aralığında değil: %s\n", trimmed_value);
                    free(trimmed_value);
                    freedouble(rgb_values);
                    return (-1);
                }

                if (lines[i][0] == 'F')
                    map_info->fclor[j] = value;
                else if (lines[i][0] == 'C')
                    map_info->cclor[j] = value;

                free(trimmed_value);
                j++;
            }

            if (j != 3)
            {
                printf("Hata: Renk değerleri eksik veya fazla!\n");
                freedouble(rgb_values);
                return (-1);
            }

            freedouble(rgb_values);
        }
        else if (lines[i][0] == '1' || lines[i][0] == '0')
        {
            if (!texture_or_color_checked)
            {
                printf("Hata: Harita başlamadan önce texture/renk bilgisi eksik!\n");
                return (-1);
            }
        }

        line_count++;
        if (line_count >= 6)
        {
            texture_or_color_checked = 1;
        }
        i++;
    }

    return check_map_textures_and_colors(map_info);
}



int check_map_textures_and_colors(t_map_info *map_info)
{

    printf("Texture ve renk bilgileri kontrol ediliyor...\n");

    if (!map_info->north_texture || !map_info->west_texture ||
        !map_info->south_texture || !map_info->east_texture) 
    {
        printf("Eksik texture dosyası!\n");
        return -1;
    }

    if ((map_info->floor_color && ft_atoi(map_info->floor_color) == -1)
        || (map_info->ceiling_color && ft_atoi(map_info->ceiling_color) == -1)) 
    {
        printf("Eksik renk bilgisi! Floor: %s, Ceiling: %s\n", map_info->floor_color, map_info->ceiling_color);
        return -1;
    }

    printf("Tüm texture ve renk bilgileri geçerli.\n");

    int i = -1;
        while(++i < 3)
            printf("ccolor[%d]: %d\n",i,map_info->cclor[i]);
        i = -1;
        while(++i < 3)
            printf("fcolor[%d]: %d\n",i,map_info->fclor[i]);
    return 0;
}


void set_texture_or_color(char **tokens, t_map_info *map_info)
{
    printf("Ayarlanan değer: %s, %s\n", tokens[0], tokens[1]);

    if (ft_strcmp(tokens[0], "NO") == 0)
        map_info->north_texture = ft_strdup(tokens[1]);
    else if (ft_strcmp(tokens[0], "SO") == 0)
        map_info->south_texture = ft_strdup(tokens[1]);
    else if (ft_strcmp(tokens[0], "WE") == 0)
        map_info->west_texture = ft_strdup(tokens[1]);
    else if (ft_strcmp(tokens[0], "EA") == 0)
        map_info->east_texture = ft_strdup(tokens[1]);
    else if (ft_strcmp(tokens[0], "F") == 0)
        map_info->floor_color = ft_strdup(tokens[1]);
    else if (ft_strcmp(tokens[0], "C") == 0)
        map_info->ceiling_color = ft_strdup(tokens[1]);
}


void freedouble(char **array)
{
    int i;

    if (!array)
        return;

    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

char **read_lines_from_file(int fd)
{
    char **lines = malloc(sizeof(char *) * 10000);
    char *line;
    int i = 0;

    while ((line = get_next_line(fd)) != NULL) {
        lines[i] = line;
        i++;
    }
    lines[i] = NULL;
    return lines;
}

int is_positive_number(const char *str)
{
    int i = 0;

    if (!str || *str == '\0')
        return 0;
    if (str[0] == '-')
        return 0;
    while (str[i])
    {
        if (!ft_atoi(str))
            return 0;
        i++;
    }
    return 1;
}
