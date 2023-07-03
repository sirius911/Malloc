#!/bin/sh
export LD_PRELOAD="$PWD/libft_malloc.so"
$@
