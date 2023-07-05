/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 14:44:36 by clorin            #+#    #+#             */
/*   Updated: 2023/07/05 10:58:19 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/malloc.h"
#include <string.h>

void	print(char *s)
{
	write(1, s, strlen(s));
}

int	main(void)
{
	char	*addr;

	addr = malloc(16);
	free(NULL);
	free((void *)addr + 5);
	if (realloc((void *)addr + 5, 10) == NULL)
		print("Bonjours\n");
	return (0);
}
