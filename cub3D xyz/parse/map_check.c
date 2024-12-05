/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibkocak < ibkocak@student.42istanbul.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 13:55:56 by ibkocak           #+#    #+#             */
/*   Updated: 2024/11/27 18:57:02 by ibkocak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

bool check_top_row(char **map, int col)
{
    for (int x = 0; x < col; x++)
	{
		if (!map[0][x] || map[0][x] == '\n')
			continue;
        if (map[0][x] != ' ' && map[0][x] != '1') {
            return false;
        }
    }
    return true;
}

bool check_bottom_row(char **map, int row, int col)
{
	for (int x = 0; x < col; x++)
	{
		if (!map[row][x] || map[row][x]  == '\n' || map[row][x] == ' ' || map[row][x] == '\0')
			continue;
		if (map[row][x] != ' ' && map[row][x] != '1' && map[row][x] == '0')
		{
			printf("c: %c\n", map[row][x]);
			printf("row: %d, col: %d\n", row, col);
			return false;
		}
	}
	return true;
}
bool check_row_edges(char **map, int row)
{
    int i = 0;
    while (i < row + 1) {
        int start = 0;
        while (map[i][start] == ' ' || map[i][start] == '\0')
            start++;
        int end = start;
        while (map[i][end] != '\0' && map[i][end] != '\n')
            end++;
        end--;

        if (map[i][start] == '0' || map[i][end] == '0') {
            printf("Error: Row %d does not start and end with 1\n", i);
            return false;
        }

        int j = 0;
        while (map[i][j] != '\0') {
            if (map[i][j] == '0') {
                if (j == 0 || map[i][j-1] == ' ' || map[i][j-1] == '\0' || map[i][j+1] == ' ' || map[i][j+1] == '\0') {
                    printf("Error: '0' at position (%d, %d) is next to a space or boundary.\n", i, j);
                    return false;
                }
            }
            j++;
        }

        i++;
    }

    return true;
}

bool check_zeros(char **map, int row, int col)
{
    for (int i = 0; i < row; i++)
	{
        col = (int)ft_strlen(map[i]); // Her satırın uzunluğunu dinamik olarak hesapla
        for (int j = 0; j < col; j++) 
		{
            if (map[i][j] == ' ') {
                if ((i > 0 && j < (int)ft_strlen(map[i - 1]) && map[i - 1][j] == '0') || 
                    (i < row - 1 && j < (int)ft_strlen(map[i + 1]) && map[i + 1][j] == '0') || 
                    (j > 0 && map[i][j - 1] == '0') || 
                    (j < col - 1 && map[i][j + 1] == '0')) {
                    printf("Error: Space found near zero at (%d, %d)\n", i, j);
                    return false;
                }
            }
        }
    }
    return true;
}





void map_spaces(char ***map, t_point *size)
{
    int max_length = 0;

    for (int i = 0; i < size->y; i++) {
        int length = ft_strlen((*map)[i]);
        if (length > max_length) {
            max_length = length;
        }
    }

    char **new_map = malloc(sizeof(char *) * (size->y + 2));
    for (int i = 0; i < size->y; i++) {
        new_map[i] = malloc(max_length + 2);

        if ((*map)[i][0] == '\n' || (*map)[i][0] == '\0') {
            for (int j = 0; j < max_length + 1; j++) {
                new_map[i][j] = ' ';
            }
            new_map[i][max_length + 1] = '\0';
        } else {
            new_map[i][0] = ' ';
            ft_strcpy(new_map[i], (*map)[i]);

            int current_length = ft_strlen((*map)[i]) + 1;
            for (int j = current_length; j < max_length + 1; j++) {
                new_map[i][j] = ' ';
            }
            new_map[i][max_length + 1] = '\0';
        }

        free((*map)[i]);
    }

    new_map[size->y] = malloc(max_length + 2);
    for (int j = 0; j < max_length + 1; j++) {
        new_map[size->y][j] = ' ';
    }
    new_map[size->y][max_length + 1] = '\0';
	new_map[size->y + 1] = NULL;
    free(*map);
    *map = new_map;
    size->x = max_length + 1;
    size->y += 1;
}



bool check_map_characters(char **map, int row)
{
    size_t map_length = ft_strlen(map[0]);

    for (int y = 1; y < row; y++) {
        size_t current_length = ft_strlen(map[y]);
        if (current_length > map_length) {
            map_length = current_length;
        }
    }

    for (int y = 0; y < row; y++) {
        size_t current_length = ft_strlen(map[y]);
        if (current_length < map_length) {
            char *new_line = malloc(map_length + 1);
            ft_strcpy(new_line, map[y]);
            for (size_t x = current_length; x < map_length; x++) {
                new_line[x] = ' ';
            }
            new_line[map_length] = '\0';
            free(map[y]);
            map[y] = new_line;
        }
    }

    for (int y = 0; y < row; y++) {
        for (size_t x = 0; x < map_length; x++) {
            char c = map[y][x];
            if (!is_valid_character(c) && c != '\0') {
                printf("Geçersiz karakter: '%c' satır: %d sütun: %zu\n", c, y + 1, x + 1);
                return false;
            }
        }
    }
    return true;
}
