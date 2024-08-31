#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // For _getch() to capture keyboard input without pressing enter

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

// Example map (0 = open, 1 = wall)
int map[MAP_WIDTH][MAP_HEIGHT];

// Function to initialize the map with walls
void init_map() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[x][y] = 1;  // Set all tiles to walls initially
        }
    }
}

// Dungeon generator using the "drunkard's walk" algorithm
void generate_dungeon(int open_tiles_percentage) {
    int total_tiles = MAP_WIDTH * MAP_HEIGHT / 1.5;
    int open_tiles = (total_tiles * open_tiles_percentage) / 100;
    int carved_tiles = 0;
    int preDir = 0;

    int x = rand() % MAP_WIDTH;
    int y = rand() % MAP_HEIGHT;

    while (carved_tiles < open_tiles) {
        if (map[x][y] == 1) {
            map[x][y] = 0;  // Carve out the tile
            carved_tiles++;
        }

        int direction = rand() % 28;
        if (direction > 4) {
            direction = (preDir) % 28;
        }
        preDir = direction;
        switch (direction) {
            case 0: if (x > 0) x--; break;       // Left
            case 1: if (x < MAP_WIDTH - 1) x++; break;  // Right
            case 2: if (y > 0) y--; break;       // Up
            case 3: if (y < MAP_HEIGHT - 1) y++; break; // Down
        }
    }
}

// Function to check if a position is within the map bounds
int is_within_bounds(int x, int y) {
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
}

// Bresenham's line algorithm for LOS
int line_of_sight(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (1) {
        if (!is_within_bounds(x0, y0)) return 0;  // Out of bounds
        if (map[x0][y0] == 1 && (x0 != x1 || y0 != y1)) return 0;  // Hit a wall

        if (x0 == x1 && y0 == y1) return 1;  // Reached the target

        int e2 = err * 2;

        // Only move horizontally or vertically, not diagonally
        if (e2 >= -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// Function to print the map with LOS visualization
void print_map_with_los(int px, int py, int tx, int ty) {
    system("cls");  // Clear the screen

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == px && y == py) {
                printf("@ ");  // Player's position
            } else if (x == tx && y == ty) {
                printf("0 ");  // Target's position
            } else if (map[x][y] == 1 && (line_of_sight(px, py, x, y) || line_of_sight(x-1, y, px, py) || line_of_sight(x+1, y, px, py) || line_of_sight(x, y-1, px, py) || line_of_sight(x, y+1, px, py))) {
                printf("# ");  // Wall within LOS
            } else if (map[x][y] == 0 && (line_of_sight(px, py, x, y) || line_of_sight(x-1, y, px, py) || line_of_sight(x+1, y, px, py) || line_of_sight(x, y-1, px, py) || line_of_sight(x, y+1, px, py))) {
                printf(". ");  // Open tile within LOS
            } else {
                printf("  ");  // Non-visible tile
            }
        }
        printf("\n");
    }
}

// Function to handle player movement
void move_player(int *px, int *py, char direction) {
    int new_x = *px, new_y = *py;

    switch (direction) {
        case 'w': new_y--; break;  // Up
        case 's': new_y++; break;  // Down
        case 'a': new_x--; break;  // Left
        case 'd': new_x++; break;  // Right
    }

    // Check for valid move
    if (is_within_bounds(new_x, new_y) && map[new_x][new_y] == 0) {
        *px = new_x;
        *py = new_y;
    }
}

int main() {
    srand(time(NULL));  // Seed the random number generator

    init_map();  // Initialize the map with walls
    generate_dungeon(50);  // Generate the dungeon with 50% open tiles

    int player_x = 5, player_y = 5;  // Player's position
    int target_x = 15, target_y = 10; // Target's position

    char input;
    while (1) {
        print_map_with_los(player_x, player_y, target_x, target_y);  // Initial render

        printf("Move with WASD keys (or 'q' to quit): ");
        input = getch();  // Get user input without waiting for Enter

        if (input == 'q') break;

        move_player(&player_x, &player_y, input);
        print_map_with_los(player_x, player_y, target_x, target_y);  // Re-render after move
    }

    return 0;
}
