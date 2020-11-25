/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 17:10:32 by acastelb          #+#    #+#             */
/*   Updated: 2020/11/25 11:39:22 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char		*ft_strchr(const char *s, int c)
{
	char	*str;

	str = (char *)s;
	while (*str != c)
	{
		if (*str == '\0')
			return (NULL);
		str++;
	}
	return (str);
}

static int		ft_check_end(char *str)
{
	if (ft_strchr(str, '\n'))
	{
		ft_strcpy(str, ft_strchr(str, '\n') + 1);
		return (0);
	}
	return (1);
}

static	t_list	*ft_lstnew(int fd)
{
	t_list *elem;

	if ((elem = (t_list*)malloc(sizeof(t_list))) == NULL)
		return (NULL);
	if ((elem->content = (char *)malloc(sizeof(char))) == NULL)
	{
		free(elem);
		return (NULL);
	}
	elem->content[0] = '\0';
	elem->fd = fd;
	elem->next = NULL;
	return (elem);
}

static int		ft_check_errors(int fd, char **line, t_list **lst, char *buff)
{
	if (fd < 0 || BUFFER_SIZE < 1 || line == NULL || read(fd, buff, 0) < 0)
		return (-1);
	if (*lst == NULL)
		if ((*lst = ft_lstnew(fd)) == NULL)
			return (-1);
	return (1);
}

static	t_list	*ft_search_and_create_fd(t_list *list, int fd)
{
	t_list	*elem;

	while (list->next)
	{
		if (list->fd == fd)
			return (list);
		list = list->next;
	}
	if (list->fd == fd)
		return (list);
	if ((elem = ft_lstnew(fd)) == NULL)
		return (NULL);
	while (list->next)
		list = list->next;
	list->next = elem;
	return (elem);
}

static	int		ft_lstclear_and_exit(t_list **lst, int return_value)
{
	if (!lst || !*lst)
		return (-1);
	if ((*lst)->next != NULL)
		ft_lstclear_and_exit(&((*lst)->next), return_value);
	free(&(*lst)->content);
	free(*lst);
	*lst = NULL;
	return (return_value);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*lst;
	t_list			*tmp_lst;
	char			*tmp;
	char			buff[BUFFER_SIZE + 1];
	int				size;

	if (ft_check_errors(fd, line, &lst, buff) == -1)
		return (-1);
	if ((tmp_lst = ft_search_and_create_fd(lst, fd)) == NULL)
		return (ft_lstclear_and_exit(&lst, -1));
	while (ft_strchr(tmp_lst->content, '\n') == NULL &&
			(size = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		buff[size] = '\0';
		tmp = tmp_lst->content;
		tmp_lst->content = ft_strjoin(tmp, buff);
		free(tmp);
	}
	*line = ft_strndup(tmp_lst->content, ft_linelen(tmp_lst->content));
	if (ft_strchr(tmp_lst->content, '\n'))
	{
		ft_strcpy(tmp_lst->content, ft_strchr(tmp_lst->content + 1, '\n') + 1);
		return (1);
	}
	return (ft_lstclear_and_exit(&lst, 0));
}
