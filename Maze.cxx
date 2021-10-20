#include "Maze.hxx"

int main(int argc, char const *argv[]){
    int algorithm = std::atoi(argv[1]);
    int resX = std::atoi(argv[2]);
    int resY = std::atoi(argv[3]);
    Maze gra(resX,resY,algorithm);
    
    return 0;
}
