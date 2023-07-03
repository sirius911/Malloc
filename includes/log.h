/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 09:16:16 by clorin            #+#    #+#             */
/*   Updated: 2023/06/30 10:35:29 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# define LOGS_PATH "/tmp/malloc.log"
# define EVENT_COUNT 5

enum e_events {
	MALLOC,
	FREE,
	MMAP,
	MUNMAP,
	REALLOC
};

# define MALLOC_LOG "Call malloc() for "
# define FREE_LOG "Call free("
# define MMAP_LOG "Call mmap() for "
# define MUNMAP_LOG "Call munmap("
# define CALLOC_LOG "Call realloc() for &ptr = "

#endif