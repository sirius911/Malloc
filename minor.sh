#!/bin/sh

echo "  __  __      _ _         ";
echo " |  \/  |__ _| | |___  __ ";
echo " | |\/| / _\` | | / _ \/ _|";
echo " |_|  |_\__,_|_|_\___/\__|";
echo "                          ";
output0=$(/usr/bin/time -v ./test0 2>&1 | grep "Minor (reclaiming a frame)")
output1=$(/usr/bin/time -v ./test1 2>&1 | grep "Minor (reclaiming a frame)")
output2=$(/usr/bin/time -v ./test2 2>&1 | grep "Minor (reclaiming a frame)")

value0=$(echo "$output0" | awk '{print $NF}')
value1=$(echo "$output1" | awk '{print $NF}')
value2=$(echo "$output2" | awk '{print $NF}')

echo "***** T e s t  0 (no malloc)*****"
echo "Minor (reclaiming a frame) : \033[0;33m$value0\033[0m\n"
echo "***** T e s t  1 (1024 x Malloc(1024)) *****"
echo "Minor (reclaiming a frame) : \033[0;33m$value1\033[0m"
diff=$(expr $value1 - $value0)
echo "Pages used (Test1 - Test 0) = \033[0;32m$diff\033[0m\n"
echo "***** T e s t  2 (1024 x [malloc(1024) - free()] *****"
echo "Minor (reclaiming a frame) : \033[0;33m$value2\033[0m"

echo "  __  __      _ _           _ _ ___ ";
echo " |  \/  |__ _| | |___  __  | | |_  )";
echo " | |\/| / _\` | | / _ \/ _| |_  _/ / ";
echo " |_|  |_\__,_|_|_\___/\__|   |_/___|";
echo "                                    ";
output0=$(./run_linux.sh /usr/bin/time -v ./test0 2>&1 | grep "Minor (reclaiming a frame)")
output1=$(./run_linux.sh /usr/bin/time -v ./test1 2>&1 | grep "Minor (reclaiming a frame)")
output2=$(./run_linux.sh /usr/bin/time -v ./test2 2>&1 | grep "Minor (reclaiming a frame)")

value0=$(echo "$output0" | awk '{print $NF}')
value1=$(echo "$output1" | awk '{print $NF}')
value2=$(echo "$output2" | awk '{print $NF}')

echo "***** T e s t  0 (no malloc)*****"
echo "Minor (reclaiming a frame) : \033[0;33m$value0\033[0m\n"
echo "***** T e s t  1 (1024 x Malloc(1024)) *****"
echo "Minor (reclaiming a frame) : \033[0;33m$value1\033[0m"
diff=$(expr $value1 - $value0)
echo "Pages used (Test1 - Test 0) = \033[0;32m$diff\033[0m\n"
echo "***** T e s t  2 (1024 x [malloc(1024) - free()] *****"
echo "Minor (reclaiming a frame) : \033[0;33m$value2\033[0m"
free=$(expr $value2 - $value0)
echo -n "\tFree is working ? -> "
if [ $value2 -lt $value1 ]; then
    echo "\033[0;32mOK\033[0m"
else
    echo "\033[0;31mKO\033[0m"
fi
echo -n "\tQuality of free = \033[0;33m$free\033[0m -> "
if [ $free -lt 10 ]; then
    echo "\033[0;32mOK\033[0m"
else
    echo "\033[0;31mKO\033[0m"
fi

