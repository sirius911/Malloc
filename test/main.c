/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 17:29:46 by clorin            #+#    #+#             */
/*   Updated: 2023/07/03 15:12:57 by clorin           ###   ########.fr       */
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
#include "./test.h"
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

static void test_hundred()
{
	// size_t size_heap, size_block;

	// size_heap = sizeof(t_heap);
	// size_block = sizeof(t_block);
	// printf("sizeof(t_heap) = %ld ", size_heap);
	// printf("sizeof(t_block) = %ld ", size_block);
	// printf("\ttotal = %ld\n\n", size_heap + size_block);
	// printf("TINY_BLOCK_SIZE = %d \n", TINY_BLOCK_SIZE);
	// printf("TINY_HEAP_ALLOCATION_SIZE = %d \n", TINY_HEAP_ALLOCATION_SIZE);
	// printf("100 * TINY_BLOCK_SIZE = %ld <= TINY_HEAP_ALLOCATION_SIZE = ", (100 * (TINY_BLOCK_SIZE + size_block) + size_heap));
	// if ((100 * (TINY_BLOCK_SIZE + size_block) + size_heap) <= TINY_HEAP_ALLOCATION_SIZE)
	// {
	// 	printf(GREEN);
	// 	printf("OK");
	// }
	// else
	// {
	// 	printf(RED);
	// 	printf("KO");
	// }
	// printf(RESET);
	// printf("\n\nSMALL_BLOCK_SIZE = %d ", SMALL_BLOCK_SIZE);
	// printf("SMALL_HEAP_ALLOCATION_SIZE = %d \n", SMALL_HEAP_ALLOCATION_SIZE);
	// printf("100 * SMALL_BLOCK_SIZE = %ld <= SMALL_HEAP_ALLOCATION_SIZE = ", (100 * (SMALL_BLOCK_SIZE + size_block) + size_heap));
	// if ((100 * (SMALL_BLOCK_SIZE + size_block) + size_heap) <= SMALL_HEAP_ALLOCATION_SIZE)
	// {
	// 	printf(GREEN);
	// 	printf("OK\n");
	// }
	// else
	// {
	// 	printf(RED);
	// 	printf("KO\n");
	// }
	// printf(RESET);

	// int amalloc = 1024;
	// printf("\n for a malloc(%i) get_heap_groupe = %i\n", amalloc, get_heap_group(amalloc));
	// printf("for a malloc(%i) get_heap_size = %li\n", amalloc, get_heap_size(amalloc));
	char *a = (char*)malloc(1024);
	show_alloc_mem(true);
	char *b = (char*)malloc(1024);
	show_alloc_mem(true);
	char *c = (char*)malloc(1024);
	show_alloc_mem(true);
	free(b);
	show_alloc_mem(true);
	char *d = (char*)malloc(1024);
	show_alloc_mem(true);
	free(c);
	show_alloc_mem(true);
	char *e = (char*)malloc(1024);
	show_alloc_mem(true);
}

int main(void)
{
	test_hundred();

	char *str = calloc(10, 255);
	show_alloc_mem(true);
	free(str);
    int nb_chaine = 10;
	print("*** malloc(0)***\n");
	test_malloc_null();
	print("****************\n");
	print("\n*** malloc(1)***\n");
	test_malloc_one();
	print("****************\n");
	print("\n***get_size()***\n");
	test_malloc_getpagesize();

	print("****************\n");
	print("\n***limits()***\n");
    test_malloc_limits();
	print("****************\n");

	print("\n***malloc_free_size()***\n");
    test_malloc_free_size();
	print("****************\n");
	print("\n***realloc()***\n");
	test_realloc_mix();
	print("****************\n");
	print("\n***realloc_null_ptr()***\n");
	realloc_null_ptr();
	print("****************\n");
	print("\n*** realloc_0() ***\n");
	realloc_0();
	print("****************\n");
	print("\n*** realloc_1() ***\n");
	realloc_1();
	print("****************\n");

	print("\n*** realloc_large() ***\n");
	realloc_large();
	print("****************\n");
	return 0;
	print("*** E N D ***\n");
	show_alloc_mem(true);

    char s[17] = "ABCDEFGHIJKLMNOPQ";

   

	return (0);
}

// gcc -shared -o libft_malloc.so srcs/malloc.c -fPIC
// export LD_PRELOAD="$PWD/libft_malloc.so"