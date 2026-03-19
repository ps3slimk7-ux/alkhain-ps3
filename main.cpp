#include <stdio.h>

// 1. Map System (0: Floor, 1: Wall, 2: Task)
const int ROWS = 10;
const int COLS = 10;
int skeldMap[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,0,1},
    {1,0,1,2,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,0,1,1,1,1,1},
    {1,2,0,0,0,0,0,0,2,1},
    {1,1,1,1,1,1,1,1,1,1}
};

// 2. Player System (Skins/Colors)
struct Player {
    int x, y;
    int colorID; // 0:Red, 1:Blue, 2:Green
};
Player p1 = {1, 1, 0};

void check_collision(int nextX, int nextY) {
    if(skeldMap[nextY][nextX] != 1) {
        p1.x = nextX;
        p1.y = nextY;
        printf("Moved to: %d,%d\n", p1.x, p1.y);
    } else {
        printf("Blocked by Wall!\n");
    }
}

int main() {
    printf("Among PS3 Engine Starting...\n");
    printf("Loading Map: The Skeld\n");
    
    // Initial State
    check_collision(1, 2);
    
    return 0;
}
 
