/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ginger <ginger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/07 11:58:48 by ginger            #+#    #+#             */
/*   Updated: 2020/05/07 16:14:40 by ginger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void		parser(char **coords, t_coords **coords_stack)
{
	while (*coords)
		push(coords_stack, new_coord(*(coords++)));
}

void		get_coords(char *file, t_coords **coords_stack)
{
	int		fd;
	int		ret;
	char	*line;
	char	**coords;

	ret = 0;
	fd = open(file, O_RDONLY, 0);
	while ((ret = get_next_line(fd, &line)) == 1)
	{
		if (!(coords = ft_strsplit(line, ' ')) || ret < 0)
			terminate(ERR_COORDS_INIT);
		parser(coords, coords_stack);
		free_arr(coords);
		free(line);
	}
	free(line);
	if (!(*coords_stack))
		terminate(ERR_COORDS_INIT);
}

int			count_height(char *file)
{
	int		height;
	int		fd;
	int		ret;
	char	*line;

	ret = 0;
	height = 0;
	fd = open(file, O_RDONLY, 0);
	while ((ret = get_next_line(fd, &line)))
	{
		if (ret < 0)
			terminate(ERR_MAP);
		height++;
		free(line);
	}
	free(line);
	close(fd);
	return (height);
}

int			count_width(int fd)
{
	int		width;
	int		*mass;
	int		ret;
	int		i;
	char	*line;

	i = 0;
	if (!(mass = (int *)ft_memalloc(sizeof(int) * 2)))
		terminate(ERR_READING);
	while ((ret = get_next_line(fd, &line)))
	{
		if (ret <= 0 || (width = ft_cl(line, ' ')) == 0)
			terminate(ERR_MAP);
		count_white_spaces(line);
		mass[i] = width;
		i++;
		checker(mass, i);
		free(line);
	}
	free(line);
	free(mass);
	return (width);
}

void		read_file(char *file, t_map *map, t_coords **coords_stack)
{
	int		fd;

	fd = open(file, O_RDONLY, 0);
	map->width = count_width(fd);
	map->height = count_height(file);
	get_coords(file, coords_stack);
	close(fd);
}
