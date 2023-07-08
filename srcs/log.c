/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 08:52:21 by clorin            #+#    #+#             */
/*   Updated: 2023/07/05 10:42:51 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <time.h>

static bool	log_status(void)
{
	return (getenv("ft_malloc"));
}

static void	log_free(const char *event, int fd, size_t ptr, size_t size)
{
	ft_putstr_fd(event, fd);
	ft_putnbr_fd((size_t)ptr, 16, fd, true);
	if (size)
		ft_putstr_fd(") -> FAIL\n", fd);
	else
		ft_putstr_fd(")\n", fd);
}

static void	log_malloc(const char *event, int fd, size_t size, size_t result)
{
	ft_putstr_fd(event, fd);
	ft_putnbr_fd((size_t)size, 10, fd, true);
	ft_putstr_fd(" bytes -> ", fd);
	ft_putnbr_fd((size_t)result, 16, fd, true);
	ft_putstr_fd("\n", fd);
}

static void	log_realloc(const char *event, int fd, size_t size, size_t ptr)
{
	ft_putstr_fd(event, fd);
	if (size && ptr)
	{
		ft_putnbr_fd((size_t)ptr, 16, fd, true);
		ft_putstr_fd(" : new size = ", fd);
		ft_putnbr_fd((size_t)size, 10, fd, true);
		ft_putstr_fd(" bytes\n", fd);
	}
	else
	{
		ft_putnbr_fd((size_t)ptr, 16, fd, true);
		ft_putstr_fd(" -> FAIL\n", fd);
	}
}

static void	log_mmap(const char *event, int fd, size_t size, size_t result)
{
	size_t			heap_size;
	t_heap_group	heap_group;

	heap_size = get_heap_size(size);
	heap_group = get_heap_group(size);
	ft_putstr_fd(event, fd);
	ft_putstr_fd(" a block of ", fd);
	ft_putnbr_fd(size, 10, fd, true);
	ft_putstr_fd(" bytes -> reserve ", fd);
	if (heap_group == TINY)
		ft_putstr_fd("[TINY] ", fd);
	else if (heap_group == SMALL)
		ft_putstr_fd("[SMALL] ", fd);
	else
		ft_putstr_fd("[LARGE] ", fd);
	ft_putnbr_fd(heap_size, 10, fd, true);
	ft_putstr_fd(" bytes -> ", fd);
	ft_putnbr_fd((size_t)result, 16, fd, true);
	ft_putstr_fd("\n", fd);
}

void	log_report(int event, void *result, size_t size)
{
	int				fd;
	const char		*event_tab[EVENT_COUNT] = {MALLOC_LOG, FREE_LOG,
		MMAP_LOG, MUNMAP_LOG, CALLOC_LOG};

	if (log_status())
	{
		fd = open(LOGS_PATH, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			return ;
		if (event == FREE || event == MUNMAP)
			log_free(event_tab[event], fd, (size_t)size, (size_t)result);
		else if (event == MMAP)
			log_mmap(event_tab[event], fd, (size_t)size, (size_t)result);
		else if (event == MALLOC)
			log_malloc(event_tab[event], fd, (size_t)size, (size_t)result);
		else if (event == REALLOC)
			log_realloc(event_tab[event], fd, (size_t)size, (size_t)result);
		close(fd);
	}
}
