#!/bin/bash         

make all
clear
echo "Welcome in Maze Generator!"
echo "Select algorith to create maze:"
echo "1) Randomized Prim's algorithm"
echo "2) Randomized DFS"
echo -e "Choose: \c"
read -r algorithm_to_create
clear
echo "Now type in width:"
echo -e "Choose: \c"
read -r sizeX
clear
echo "Now type in height:"
echo -e "Choose: \c"
read -r sizeY
clear
echo "CONTROLS:"
echo "p - starts/pause the animation"
echo "q - exit"
echo "-/= - make animation slower/faster"
read -n 1 -s -r -p "Press any key to continue..."
clear
./Maze.x $algorithm_to_create $sizeX $sizeY
