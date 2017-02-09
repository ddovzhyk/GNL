/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddovzhik <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/05 15:07:46 by ddovzhik          #+#    #+#             */
/*   Updated: 2017/02/05 15:07:48 by ddovzhik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_fd	*search_fd_create(t_fd **first, const int fd)
{
	t_fd	*elem;

	elem = *first;
	while (elem && elem->fd != fd)
		elem = elem->next;
	if (!elem && (elem = (t_fd *)malloc(sizeof(t_fd))))
	{
		elem->fd = fd;
		elem->tmp = NULL;
		elem->next = *first;
		*first = elem;
	}
	return (elem);
}

int		func_for_newline(char **line, char **s)
{
	char	*tmp;

	if (!(*line))
	{
		if (!(*line = ft_strdup(*s)))
			return (-1);
	}
	else if (**s != '\n')
	{
		if (!(tmp = ft_strjoin(*line, *s)))
		{
			ft_strdel(line);
			return (-1);
		}
		ft_strdel(line);
		*line = tmp;
	}
	ft_strdel(s);
	return (0);
}

int		newline(char **line, char **s, char *c, char *tmp)
{
	unsigned int	j;

	j = 0;
	while ((*s)[j] && (*s)[j] != '\n')
		j++;
	if ((*s)[j] == '\n')
	{
		if (!(tmp = ft_strsub(*s, 0, j)))
			return (-1);
		if (!(*line = (*line) ? ft_strjoin(c, tmp) : ft_strdup(tmp)))
		{
			ft_strdel(&tmp);
			return (-1);
		}
		ft_strdel(&tmp);
		if (!(tmp = ft_strsub(*s, j + 1, ft_strlen(*s) - 1)))
			return (-1);
		ft_strdel(s);
		*s = tmp;
		ft_strdel(&c);
		return (1);
	}
	else if (func_for_newline(line, s))
		return (-1);
	return (0);
}

int		gnl(const int fd, char **line, char **tmp, char *buff)
{
	int			q;
	char		*f;

	*line = NULL;
	while (read(fd, buff, BUFF_SIZE))
	{
		f = *tmp;
		if (!(*tmp = (*tmp) ? ft_strjoin(f, buff) : ft_strdup(buff)))
			return (-1);
		ft_strdel(&f);
		if ((q = newline(line, tmp, *line, NULL)))
			return (q);
		else
			ft_strclr(buff);
	}
	if (*tmp && (q = newline(line, tmp, *line, NULL)))
		return (q);
	if (*line && **line)
		return (1);
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	static t_fd	*first;
	char		*buff;
	int			q;

	if (BUFF_SIZE < 1 || !line || fd < 0 || !(buff = ft_strnew(BUFF_SIZE)) ||
		read(fd, buff, 0) < 0)
		return (-1);
	q = gnl(fd, line, &(search_fd_create(&first, fd)->tmp), buff);
	ft_strdel(&buff);
	return (q);
}
