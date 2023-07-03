/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 21:23:17 by clorin            #+#    #+#             */
/*   Updated: 2023/06/06 14:21:05 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif
#include "malloc.h"

void* malloc(size_t sz)
{
    void *(*libc_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
    write(1,"I",1);
    return libc_malloc(sz);
}

void free(void *p)
{
    void (*libc_free)(void*) = dlsym(RTLD_NEXT, "free");
    write(1,"O",1);
    libc_free(p);
}

// gcc -shared -o libft_malloc.so srcs/malloc.c -fPIC
// export LD_PRELOAD="$PWD/libft_malloc.so"