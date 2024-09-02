#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#define MAP_WIDTH 100
#define MAP_HEIGHT 60

int map[MAP_WIDTH][MAP_HEIGHT];
int LOS[MAP_WIDTH][MAP_HEIGHT];

void init_map() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[x][y] = 1;
        }
    }
}

void clearLOS() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            LOS[x][y] = 0;
        }
    }
}


void generate_dungeon(int open_tiles_percentage) {
    int total_tiles = MAP_WIDTH * MAP_HEIGHT;
    int open_tiles = (total_tiles * open_tiles_percentage) / 100;
    int carved_tiles = 0;
    int preDir = 0;
    int x = rand() % MAP_WIDTH;
    int y = rand() % MAP_HEIGHT;
    while (carved_tiles < open_tiles) {
        if (map[x][y] == 1) {
            map[x][y] = 0;
            carved_tiles++;
        }
        int direction = rand() % 15;
        if (direction > 4) {
            direction = (preDir) % 15;
        }
        preDir = direction;
        switch (direction) {
            case 0: if (x > 0) x--; break;
            case 1: if (x < MAP_WIDTH - 1) x++; break;
            case 2: if (y > 0) y--; break;
            case 3: if (y < MAP_HEIGHT - 1) y++; break;
        }
    }
}

int is_within_bounds(int x, int y) {
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
}

int line_of_sightV(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int blockedL = 0;
    int blockedR = 0;
    int blockedT = 0;
    int blockedD = 0;
    int start = 1;
    while (1) {
        if (x0 == x1 && y0 == y1) return 1;
        if ((map[x0][y0] == 1) && (!start)) return 0;
        LOS[x0][y0] = 1;
        if (!is_within_bounds(x0, y0)) return 0;
        if (!start) {
            if (map[x0-1][y0] == 1) blockedL = 1;
            if (map[x0+1][y0] == 1) blockedR = 1;
            if (map[x0][y0-1] == 1) blockedT = 1;
            if (map[x0][y0+1] == 1) blockedD = 1;
        }
        int e2 = err * 2;
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
            if (sy > 0 && blockedD == 1) return 0;
            if (sy < 0 && blockedT == 1) return 0;
        } else if (e2 >= -dy) {
            err -= dy;
            x0 += sx;
            if (sx > 0 && blockedR == 1) return 0;
            if (sx < 0 && blockedL == 1) return 0;
        }
        start = 0;
    }
}

int line_of_sightH(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int blockedL = 0;
    int blockedR = 0;
    int blockedT = 0;
    int blockedD = 0;
    int start = 1;
    while (1) {
        if (map[x0][y0] == 1) return 0;
        if (!is_within_bounds(x0, y0)) return 0;
        LOS[x0][y0] = 1;
        if (x0 == x1 && y0 == y1) return 1;
        if (!start) {
            if (map[x0-1][y0] == 1) blockedL = 1;
            if (map[x0+1][y0] == 1) blockedR = 1;
            if (map[x0][y0-1] == 1) blockedT = 1;
            if (map[x0][y0+1] == 1) blockedD = 1;
        }
        int e2 = err * 2;
        if (e2 >= -dy) {
            err -= dy;
            x0 += sx;
            if (sx > 0 && blockedR == 1) return 0;
            if (sx < 0 && blockedL == 1) return 0;
        } else if (e2 <= dx) {
            err += dx;
            y0 += sy;
            if (sy > 0 && blockedD == 1) return 0;
            if (sy < 0 && blockedT == 1) return 0;
        }
        start = 0;
    }
}

void generateLOS(int x0, int y0) {

    for (int x = 0; x < MAP_WIDTH; x++) {
        line_of_sightH(x0, y0, x, 0);
        line_of_sightV(x0, y0, x, 0);
    }

    for (int x = 0; x < MAP_WIDTH; x++) {
        line_of_sightH(x0, y0, x, MAP_HEIGHT-1);
        line_of_sightV(x0, y0, x, MAP_HEIGHT-1);
    }

    for (int y = 0; y < MAP_HEIGHT; y++)  {
        line_of_sightH(x0, y0, 0, y);
        line_of_sightV(x0, y0, 0, y);
    }

    for (int y = 0; y < MAP_HEIGHT; y++)  {
        line_of_sightH(x0, y0, MAP_WIDTH-1, y);
        line_of_sightV(x0, y0, MAP_WIDTH-1, y);
    }
}

int line_of_sight(int x, int y) {
    if (!is_within_bounds(x, y)) return 0;
    return (LOS[x][y]);
}

void print_map_with_los(int px, int py, int tx, int ty) {
    clearLOS();
    generateLOS(px, py);
    system("cls");
    char buffer[(MAP_WIDTH * 2 + 1) * MAP_HEIGHT + 20];
    int buffer_index = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            char tile;
            if (x == px && y == py) {
                tile = '@';
            } else if (x == tx && y == ty) {
                tile = 'T';
            } else if (map[x][y] == 1 && (line_of_sight(x, y) || line_of_sight(x-1, y) || line_of_sight(x+1, y) || line_of_sight(x, y-1) || line_of_sight(x, y+1))) {
                tile = '#';
            } else if (map[x][y] == 0 && (line_of_sight(x, y) || line_of_sight(x-1, y) || line_of_sight(x+1, y) || line_of_sight(x, y-1) || line_of_sight(x, y+1))) {
                tile = '.';
            } else {
                tile = ' ';
            }
            buffer[buffer_index++] = tile;
            buffer[buffer_index++] = ' ';
        }
        buffer[buffer_index++] = '\n';
    }
    buffer[buffer_index] = '\0';
    printf("%s", buffer);
}

void move_player(int *px, int *py, char direction) {
    int new_x = *px, new_y = *py;
    switch (direction) {
        case 'w': new_y--; break;
        case 's': new_y++; break;
        case 'a': new_x--; break;
        case 'd': new_x++; break;
    }
    if (is_within_bounds(new_x, new_y) && map[new_x][new_y] == 0) {
        *px = new_x;
        *py = new_y;
    }
}

int main() {
    srand(320000);
    init_map();
    generate_dungeon(72);
    int player_x = 5, player_y = 5;
    int target_x = 15, target_y = 10;
    char input;
    while (1) {
        print_map_with_los(player_x, player_y, target_x, target_y);
        printf(" q to quit: ");
        input = getch();
        if (input == 'q') break;
        move_player(&player_x, &player_y, input);
        print_map_with_los(player_x, player_y, target_x, target_y);
    }
    return 0;
}
