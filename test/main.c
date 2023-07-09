/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 17:29:46 by clorin            #+#    #+#             */
/*   Updated: 2023/07/09 11:28:21 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "../includes/malloc.h"
#include "colors.h"

#define M 1024 * 1024

static void	print(char *s)
{
	write(1, s, strlen(s));
}

#include <stdio.h>
#include <unistd.h>

static void test_malloc_null()
{
    void *t = malloc(0);
    if (t)
        print("malloc(0) should return NULL\n");
	else
		print("Ok\n");
    free(t);
}

static void test_malloc_one()
{
    char *t = (char *)malloc(1);
    if (!t) {
        printf("malloc(1) should return ptr\n");
        return;
    }
    t[0] = 'a';
    show_alloc_mem_hex();
    free(t);
}

static void test_malloc_getpagesize()
{
    void *t = malloc(getpagesize());
	show_alloc_mem(true);
    free(t);
}

static void test_malloc_limits()
{
    void	*t	= malloc(1);
    void	*t0	= malloc(TINY_BLOCK_SIZE);
    void	*t00	= malloc(TINY_BLOCK_SIZE);
    void	*t000	= malloc(TINY_BLOCK_SIZE);
    void	*t1	= malloc(SMALL_BLOCK_SIZE);
    void	*t2	= malloc(SMALL_BLOCK_SIZE + 1);

    show_alloc_mem(true);
    free(t0);
	print(YELLOW);
	print("--- free(t0) ---\n");
	print(RESET);
	show_alloc_mem(true);
    t0 = malloc(TINY_BLOCK_SIZE - sizeof(t_block));
	print(YELLOW);
	print("--- malloc(96) ---\n");
	print(RESET);
    show_alloc_mem(true);
    free(t0);
    free(t00);
    free(t000);
    free(t1);
    free(t2);
	free(t);
}

static void test_malloc_free_size()
{
    void *t = malloc(SMALL_BLOCK_SIZE + 1);

    // heap should have 0 free_space
    show_alloc_mem(true);
    free(t);
}

static void test_realloc_mix(void) {
	void *test = malloc(10);
	void *test2 = malloc(10);
	void *test3 = malloc(10);

	free(test);
	show_alloc_mem(true);
	print(YELLOW);
	print("--- realloc ---\n");
	print(RESET);
	test2 = realloc(test2, 10);
	test2 = realloc(test2, 11);
	test2 = realloc(test2, 9);
	show_alloc_mem(true);
	free(test2);
	free(test3);
}

static void realloc_null_ptr() {
	char *t = realloc(NULL, 5);

	if (!t)
	{
		print(RED);
		print("Realloc(NULL, 5) should return ptr\n");
	}
	else
	{
		print(GREEN);
		print("Ok\n");
	}
	print(RESET);
	free(t);
}

static void realloc_0() {
	char *t = malloc(4);
	t = realloc(t, 0);

	if (t)
	{
		print(RED);
		print("Realloc(ptr, 0) should return NULL\n");
	}
	else
	{
		print(GREEN);
		print("Ok\n");
	}
	print(RESET);
	free(t);
}

static void realloc_1() {
	char *t = malloc(1);
	t = realloc(t, 1);
	t = realloc(t, 10);
	t[0] = 'A';
	t[9] = 'A';
	t = realloc(t, TINY_BLOCK_SIZE);
	t = realloc(t, SMALL_BLOCK_SIZE);
	t = realloc(t, SMALL_BLOCK_SIZE + 2);
	t = realloc(t, 10);

	if (t[0] != 'A' || t[9] != 'A')
	{
		print(RED);
		print("Realloc should copy data");
	}
	else
	{
		print(GREEN);
		print("Ok\n");
	}
	print(RESET);
	free(t);
}

static void realloc_large()
{
	char    *addr1;
	char    *addr2;
	char    *addr3;

	show_alloc_mem(true);
	addr1 = (char *)malloc(1 * M);
	show_alloc_mem(true);
	strcpy(addr1, "Bonjours\n");
	addr2 = (char *)malloc(16 * M);
	show_alloc_mem(false);
	print(YELLOW);
	print("--- realloc(addr1) ---\n");
	print(RESET);
	addr3 = (char *)realloc(addr1, 128* M);
	addr3[127 * M] = 42;
	show_alloc_mem(false);
	print(addr3);
	free(addr3);
	free(addr2);
}

static void info( int amalloc)
{
	size_t size_heap, size_block;
	int page_size;

	size_heap = sizeof(t_heap);
	size_block = sizeof(t_block);
	page_size = getpagesize();

	printf("***************************\n");
	printf("*       %sS I Z E O F%s       *\n", BLUE, RESET);
	printf("***************************\n");
	printf("*  t_heap   *   t_block   *\n");
	printf("***************************\n");
	printf("*   %s%ld%s      *     %s%ld%s      *\n", BLUE, size_heap, RESET, BLUE, size_block, RESET);
	printf("***************************\n");
	printf("*           %s%ld%s            *\n", BLUE,size_heap + size_block, RESET);
	printf("***************************\n\n");

	printf("-------------\n");
	printf("| %sPage Size%s |\n", BLUE, RESET);
	printf("|-----------|\n");
	printf("|   %s%d%s    |\n", YELLOW, page_size, RESET);
	printf("-------------\n");

	printf("-------------------------------------------------------------\n");
	printf("|         | BLOCK SIZE | HEAP ALLOCATION SIZE |  x 100 |    |\n");
	printf("|------------------------------------------------------|----|\n");
	printf("|  TINY   |    %s%d%s     |        %s%d%s         | %s%ld%s  | ", BLUE,TINY_BLOCK_SIZE, RESET,BLUE, TINY_HEAP_ALLOCATION_SIZE, RESET,BLUE, (100 * (TINY_BLOCK_SIZE + size_block) + size_heap), RESET);
	if ((100 * (TINY_BLOCK_SIZE + size_block) + size_heap) <= TINY_HEAP_ALLOCATION_SIZE)
	{
		printf(GREEN);
		printf("OK");
	}
	else
	{
		printf(RED);
		printf("KO");
	}
	printf(RESET);
	printf(" |\n|------------------------------------------------------|----|\n");
	printf("|  SMALL  |   %s%d%s     |       %s%d%s         | %s%ld%s | ", YELLOW, SMALL_BLOCK_SIZE, RESET, YELLOW, SMALL_HEAP_ALLOCATION_SIZE, RESET, YELLOW, (100 * (SMALL_BLOCK_SIZE + size_block) + size_heap), RESET);
	if ((100 * (TINY_BLOCK_SIZE + size_block) + size_heap) <= TINY_HEAP_ALLOCATION_SIZE)
	{
		printf(GREEN);
		printf("OK");
	}
	else
	{
		printf(RED);
		printf("KO");
	}
	printf(RESET);
	printf(" |\n-------------------------------------------------------------\n");
	
	printf("\nfor a malloc(%s%d%s) get_heap_groupe = ", BLUE, amalloc, RESET);
	if (get_heap_group(amalloc) == 0)
		printf("%sTINY%s", GREEN, RESET);
	else if (get_heap_group(amalloc) == 1)
		printf("%sSMALL%s", GREEN, RESET);
	else
		printf("%sLARGE%s", GREEN, RESET);
	printf("-> get_heap_size = %s%ld%s [ %s%ld%s x %d ]\n", YELLOW, get_heap_size(amalloc), RESET, MAGENTA, (get_heap_size(amalloc) / page_size), RESET, page_size);
}

int main(int argc, char **argv)
{
	int amalloc = 1024 * 1024;

	if (argc > 1)
		amalloc = atoi(argv[1]);
	info(amalloc); 
	return (0);
}

// gcc -shared -o libft_malloc.so srcs/malloc.c -fPIC
// export LD_PRELOAD="$PWD/libft_malloc.so"