/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 18:50:52 by clorin            #+#    #+#             */
/*   Updated: 2023/07/03 15:17:44 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

#include <sys/mman.h>
#include <sys/resource.h>

#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include "log.h"

#define HEAP_OFFSET(start) ((void *)start + sizeof(t_heap))
#define BLOCK_OFFSET(start) ((void *)start + sizeof(t_block))

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (16 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)


typedef enum	e_heap_group {
	TINY,
	SMALL,
	LARGE
}				t_heap_group;

typedef struct s_heap {
    struct s_heap   *prev;
    struct s_heap   *next;
    t_heap_group    group;
    size_t          total_size;
    size_t          free_size;
    size_t          block_count;
} t_heap;

typedef struct s_block {
    struct s_block  *prev;
    struct s_block  *next;
    size_t          data_size;
    bool            freed;
} t_block;


//Globals
extern pthread_mutex_t		g_malloc_mutex;
extern t_heap				*g_heap;

//utils blocks
/*void			find_available_block(size_t size,
	t_heap **res_heap, t_block **res_block);
*/
// t_block			*last_block(t_block *block);
t_block			*fill_block(size_t size);
void			*append_block(t_heap *heap, size_t size);
// void			init_block(t_block *block, size_t size);
t_block			*merge_block(t_heap *heap, t_block *block);
void			remove_block(t_heap *heap, t_block *block);
size_t	        print_block(t_block *block);

//utils heap
t_heap_group	get_heap_group(size_t size);
t_heap			*get_heap(const size_t size);
size_t			get_heap_size(size_t size);
void	        print_heap_header(char *name, t_heap *heap, bool verbose);

// void			*malloc_main(size_t size);
// void			start_free(void *ptr);
void			*start_realloc(void *ptr, size_t size);
t_heap			*get_last_heap(t_heap *heap);

//log
void            log_report(int event, void* result, size_t size);

// void			print_heap_group(t_heap *heap);
void			reinit_freed_block(t_block *block, size_t size, t_heap *heap);

void			delete_heap_if_empty(t_heap *heap);

// t_heap			*create_heap(t_heap_group group, size_t block_size);
void			search_ptr(t_heap **found_heap, t_block **found_block, void *ptr);


//libft
void			ft_bzero(void *s, size_t n);
void			ft_putstr(char *str);
void			ft_putstr_fd(char const *s, int fd);
size_t			ft_strlen(const char *s);
void			ft_putnbr_fd(size_t nb, int base, int fd, bool prefix);
void			*ft_memset(void*, int, size_t);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t n);

// main functions
void			*malloc(size_t size);
void    		free(void *ptr);
void            *realloc(void *prt, size_t size);
void            show_alloc_mem(bool);

//Bonus functions
void			*calloc(size_t count, size_t size);
void	        show_alloc_mem_hex(void);

#endif