/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 22:00:31 by clorin            #+#    #+#             */
/*   Updated: 2023/07/03 10:48:34 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	ft_putnbr_fd(size_t nb, int base, int fd, bool prefix)
{
	char	*str;

	str = "0123456789ABCDEFGHIJKLMNOPQRSTUIVWXYZ";
	if (nb / base)
		ft_putnbr_fd(nb / base, base, fd, prefix);
	else
	{
		if (base == 16 && prefix)
			ft_putstr_fd("0x", fd);
	}
	write(fd, &str[nb % base], 1);
}
