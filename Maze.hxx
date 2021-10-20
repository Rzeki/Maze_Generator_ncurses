#include <iostream>
#include <ncurses.h>
#include <ctime>
#include <vector>
#include "stack.hxx"
#include <algorithm>
#include <random>
#include <fstream>

constexpr int max_timeout = 2048;
constexpr int min_timeout = 128;

constexpr int down = 1;
constexpr int right = 2;
constexpr int left = 4;
constexpr int up = 8;


struct Pointer{
    int x;
    int y;
    Pointer() = default;
    ~Pointer() = default;
    Pointer(int _x,int _y): x(_x),y(_y) {}
};

class Maze{
    public:
    const int width;
    const int height;
    typedef void (Maze::*Maze_ptr_f)(int**);
    Pointer starting_point;
    Pointer current_point;
    Stack<int,1000> stack_to_dfs_x;
    Stack<int,1000> stack_to_dfs_y;
    Maze_ptr_f algorithm_to_choose;
    std::vector<int> grid_row;
    std::vector<int> grid_column;
    std::vector<int> grid_direction;
    int** grid;
    bool running;
    int tick;
    Maze(int X,int Y,int algo) : running(false), tick(256), width(X), height(Y){
        srand(time(NULL));
        grid = new int*[height];
        for(int i=0;i<height;i++){
            grid[i]=new int[width];
            for(int j=0;j<width;j++){
                grid[i][j]=1;                  //1 - wall | 2 - path  | 0 - starting point
            }
        }
        if(algo==1){
            algorithm_to_choose = &Maze::next_grid_PRIM;
        }
        else if(algo==2){
            algorithm_to_choose = &Maze::next_grid_DFS;
        }
        current_point = Pointer(rand()%width,rand()%height);
        starting_point = current_point;
        initscr();            
        noecho();
        nodelay(stdscr, TRUE); 
        if (has_colors() == FALSE) {
            endwin();
            printf("Your terminal does not support color\n");
            exit(1);
        }
        init_colors();
        draw_border();
        grid[starting_point.y][starting_point.x]=0;
        stack_to_dfs_x.push(starting_point.x);
        stack_to_dfs_y.push(starting_point.y);
        count_walls();
        timeout(tick);
        start();
    }

    ~Maze(){
        endwin();
    }

    void drawboard(){
        for(int i=0;i<height;i++){
            for(int j=0;j<width;j++){
                move(i,j);
                if(grid[i][j]==2){
                    if(i==current_point.y && j==current_point.x){
                        attron(COLOR_PAIR(5));
                        printw(" ");
                        attroff(COLOR_PAIR(5));
                    }
                    else{
                        attron(COLOR_PAIR(2));
                        printw(" ");
                        attroff(COLOR_PAIR(2));
                    }
                }
                if(grid[i][j]==1){
                    attron(COLOR_PAIR(4));
                    printw(" ");
                    attroff(COLOR_PAIR(4));
                }
                if(i==starting_point.y && j==starting_point.x){
                    attron(COLOR_PAIR(3));
                    printw(" ");
                    attroff(COLOR_PAIR(3));
                }
            }
        }
        move(current_point.y,current_point.x);
    }

    void init_colors(){
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLUE);
        init_pair(2, COLOR_RED, COLOR_WHITE); 
        init_pair(3, COLOR_GREEN, COLOR_GREEN); 
        init_pair(4, COLOR_BLACK, COLOR_BLACK); 
        init_pair(5, COLOR_RED, COLOR_RED); 
    }

    void draw_border(){
        attron(COLOR_PAIR(1));
        mvhline(height, 0, ' ', width+1);
        mvvline(0, width, ' ', height);
        attroff(COLOR_PAIR(1));
    }

    int getdirs(int** current_grid){
        std::vector<int> temp;
        if(current_point.y>1){
            if(current_grid[current_point.y-2][current_point.x]!=2 && current_grid[current_point.y-2][current_point.x]!=0){
                temp.push_back(1);
            }
        }
        if(current_point.y<height-2){
            if(current_grid[current_point.y+2][current_point.x]!=2 && current_grid[current_point.y+2][current_point.x]!=0){
                temp.push_back(2);
            }
        }
        if(current_point.x>1){
            if(current_grid[current_point.y][current_point.x-2]!=2 && current_grid[current_point.y][current_point.x-2]!=0){
                temp.push_back(3);
            }
        }
        if(current_point.x<width-2){
            if(current_grid[current_point.y][current_point.x+2]!=2 && current_grid[current_point.y][current_point.x+2]!=0){
                temp.push_back(4);
            }
        }
        if(temp.empty()){
            return 5;
        }
        std::random_shuffle(temp.begin(),temp.end());
        return temp.at(0);
    }

    bool anyneighbours(int** current_grid){
        bool check = false;
        if(current_point.y>1){
            if(current_grid[current_point.y-2][current_point.x]==1){
                check=true;
            }
        }
        if(current_point.y<height-2){
            if(current_grid[current_point.y+2][current_point.x]==1){
                check=true;
            }
        }
        if(current_point.x>1){
            if(current_grid[current_point.y][current_point.x-2]==1){
                check=true;
            }
        }
        if(current_point.x<width-2){
            if(current_grid[current_point.y][current_point.x+2]==1){
                check=true;
            }
        }

        return check;
    }

    void next_grid_DFS(int** current_grid){
        if(!stack_to_dfs_x.empty()){
            current_point.y=stack_to_dfs_y.pop();
            current_point.x=stack_to_dfs_x.pop();
            if(anyneighbours(current_grid)){
                stack_to_dfs_y.push(current_point.y);
                stack_to_dfs_x.push(current_point.x); 
                int direc = getdirs(current_grid);
                switch (direc){
                    case 1:
                        stack_to_dfs_x.push(current_point.x);
                        stack_to_dfs_y.push(current_point.y-2);
                        current_grid[current_point.y-1][current_point.x]=2;
                        current_grid[current_point.y-2][current_point.x]=2;
                        break;
                    case 2:
                        stack_to_dfs_x.push(current_point.x);
                        stack_to_dfs_y.push(current_point.y+2);
                        current_grid[current_point.y+1][current_point.x]=2;
                        current_grid[current_point.y+2][current_point.x]=2;
                        break;

                    case 3:
                        stack_to_dfs_x.push(current_point.x-2);
                        stack_to_dfs_y.push(current_point.y);
                        current_grid[current_point.y][current_point.x-1]=2;
                        current_grid[current_point.y][current_point.x-2]=2;
                        break;

                    case 4:
                        stack_to_dfs_x.push(current_point.x+2);
                        stack_to_dfs_y.push(current_point.y);
                        current_grid[current_point.y][current_point.x+1]=2;
                        current_grid[current_point.y][current_point.x+2]=2;
                        break;
                    
                    default:
                        stack_to_dfs_y.pop();
                        stack_to_dfs_x.pop();
                        break;
                }
            }
        }
    }

    void next_grid_PRIM(int** current_grid){
        int number=grid_row.size();
        int rand_number=rand()%number;
        if(grid_row.size()!=0){
            current_point.y=grid_row[rand_number];
            current_point.x=grid_column[rand_number];
            switch(grid_direction[rand_number]){
                case down:{
                    current_point.y=grid_row[rand_number]+1;
                    current_point.x=grid_column[rand_number];
                    break;
                }
                case right:{
                    current_point.y=grid_row[rand_number];
                    current_point.x=grid_column[rand_number]+1;
                    break;  
                }
                case left:{
                    current_point.y=grid_row[rand_number];
                    current_point.x=grid_column[rand_number]-1;
                    break;
                }
                case up:{
                    current_point.y=grid_row[rand_number]-1;
                    current_point.x=grid_column[rand_number];
                    break;
                }
            }
        
            if(current_grid[current_point.y][current_point.x]==1){
                current_grid[grid_row[rand_number]][grid_column[rand_number]]=2;
                current_grid[current_point.y][current_point.x]=2;
                count_walls();
            }           
            grid_row.erase(grid_row.begin()+rand_number);
            grid_column.erase(grid_column.begin()+rand_number);
            grid_direction.erase(grid_direction.begin()+rand_number);
        }
    }

    void push_to_vectors(int x, int y, int direction){
        grid_row.push_back(x);
        grid_column.push_back(y);
        grid_direction.push_back(direction);
    }

    int count_walls(){
        int count=0;
        if(current_point.y<=height-3){
            push_to_vectors(current_point.y+1,current_point.x,down);
            count++;
        }
        if(current_point.x<=width-3){
            push_to_vectors(current_point.y,current_point.x+1,right);
            count++;
        }
        if(current_point.y>=3){
            push_to_vectors(current_point.y-1,current_point.x,up);
            count++;
        }
        if(current_point.x>=3){
            push_to_vectors(current_point.y,current_point.x-1,left);
            count++;
        }
        return count;
    }

    void nextGeneration(){
        if(running){
            clear();
            (this->*algorithm_to_choose)(grid);
            draw_border();
        }
        refresh();
    }

    void start(){
        while(true){
            drawboard();
            auto input = getch();
            if(input!=ERR){
                if(input=='q'){
                    break;
                    clear();
                }
                if(input=='='){
                    if(tick>=min_timeout){
                        tick/=2;
                        timeout(tick);
                    } 
                }
                if(input=='p'){
                    running=!running;
                }
                if(input=='-'){
                    if(tick<=max_timeout){
                        tick*=2;
                        timeout(tick);
                    } 
                }
            }
            if(grid_row.size()==0){
                running=false;
            }
            nextGeneration();
        }
    }

};
