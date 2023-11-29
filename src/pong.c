#include "pong.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

char **create_char_matrix(int rows, int cols) {
    char **ptrArray = calloc(rows, sizeof(char *));
    for (int i = 0; i < rows && ptrArray != NULL; i++) {
        ptrArray[i] = calloc(cols, sizeof(char));
        if (ptrArray[i] == NULL) {
            free_matrix(ptrArray, i);
        }
    }
    return ptrArray;
}

void free_matrix(char **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void init_field(char **field, Ball *ball, Racket *racket_left, Racket *racket_right) {
    for (int row = TOP; row < FIELD_HEIGHT; row++) {
        if (row == TOP || row == BOTTOM) {
            for (int col = LEFT_BOUND; col < FIELD_WIDTH; col++) {
                field[row][col] = '-';
            }
        } else {
            field[row][LEFT_BOUND] = '|';
            field[row][CENTER_X] = '|';
            field[row][RIGHT_BOUND] = '|';
        }
    }
    ball->x = CENTER_X, ball->y = CENTER_Y, ball->cur_dir_x = LEFT, ball->cur_dir_y = UP;
    racket_left->x = LEFT_BOUND + BOUNDARY_OFFSET, racket_left->y = CENTER_Y;
    racket_right->x = RIGHT_BOUND - BOUNDARY_OFFSET, racket_right->y = CENTER_Y;

    field[ball->y][ball->x] = 'O';
    for (int y = CENTER_Y - 1; y <= CENTER_Y + 1; y++) {
        field[y][racket_left->x] = '|';
        field[y][racket_right->x] = '|';
    }

    field[SCORE_Y][SCORE1_X] = '0';
    field[SCORE_Y][SCORE1_X + 1] = '0';
    field[SCORE_Y][SCORE2_X] = '0';
    field[SCORE_Y][SCORE2_X + 1] = '0';
}

void output(char **field) {
    for (int row = TOP; row < FIELD_HEIGHT; row++) {
        for (int col = LEFT_BOUND; col < FIELD_WIDTH; col++) {
            if (field[row][col] == 0) {
                printw(" ");
            } else {
                printw("%c", field[row][col]);
            }
        }
        printw("\n");
    }
}

void init_curses(void) {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
}

int game_over(int score1, int score2) {
    int exit_status = 0;
    if (score1 == GAMEOVER_SCORE) {
        printw("Player 1 wins!");
        exit_status = 1;
    } else if (score2 == GAMEOVER_SCORE) {
        printw("Player 2 wins!");
        exit_status = 1;
    }
    return exit_status;
}

void update_ball_dir(Ball *ball, Racket racket_left, Racket racket_right) {
    ball->prev_x = ball->x;
    ball->prev_y = ball->y;
    if (ball->y == TOP + 1 || ball->y == BOTTOM - 1) {  // top/bottom
        ball->cur_dir_y *= -1;
    }
    if (ball->x - 1 == racket_left.x &&
        (ball->y + ball->cur_dir_y >= racket_left.y - 1 && ball->y <= racket_left.y + 1)) {
        check_racket_collision(ball, racket_left);
    } else if (ball->x + 1 == racket_right.x &&
               (ball->y + ball->cur_dir_y >= racket_right.y - 1 && ball->y <= racket_right.y + 1)) {
        check_racket_collision(ball, racket_right);
    }
    
}

void move_ball(Ball *ball, int *score1, int *score2) {
    ball->x += ball->cur_dir_x;
    ball->y += ball->cur_dir_y;
    if (ball->x == LEFT_BOUND) {  // leftbound
        ball->x = CENTER_X;
        (*score2)++;
    } else if (ball->x == RIGHT_BOUND) {  // rightbound
        ball->x = CENTER_X;
        (*score1)++;
    }
}

void check_racket_collision(Ball *ball, Racket racket) {
    if (ball->cur_dir_y == STRAIGHT) {  // straight line of movement
        if (ball->y == racket.y - 1) {  // top racket
            ball->cur_dir_y = UP;
        } else if (ball->y == racket.y) {  // mid racket
            ball->cur_dir_y = STRAIGHT;
        } else if (ball->y == racket.y + 1) {  // bottom racket
            ball->cur_dir_y = DOWN;
        }
        ball->cur_dir_x *= -1;
    } else {  // diagonal movement
        if ((ball->cur_dir_y == DOWN && ball->y + ball->cur_dir_y == racket.y - 1) ||
            (ball->cur_dir_y == UP && ball->y + ball->cur_dir_y == racket.y + 1)) {  // closest hit
            ball->cur_dir_y *= -1;
            // ball->cur_dir_x *= -1;
        } else if (ball->y == racket.y) {  // center hit
            ball->cur_dir_y = STRAIGHT;
            // ball->cur_dir_x *= -1;
        }
        // else if ((ball->cur_dir_y == DOWN && (ball->y == racket.y + 1)) ||
        //             (ball->cur_dir_y == UP && (ball->y == racket.y - 1))) {  //
        //             furthest hit
        //     ball->cur_dir_x *= -1;
        // }
        ball->cur_dir_x *= -1;
        if (ball->y == TOP + 1 || ball->y == BOTTOM - 1) {  // top/bottom - corner check
            ball->cur_dir_y *= -1;
        }
    }
}

void update_field(char **field, Ball *ball, Racket *racket_left, Racket *racket_right, int score1,
                  int score2) {
    for (int y = TOP + 1; y < BOTTOM; y++) {
        field[y][racket_left->x] = ' ';
        field[y][racket_right->x] = ' ';
    }
    if (ball->prev_x == CENTER_X) {
        field[ball->prev_y][ball->prev_x] = '|';
    } else {
        field[ball->prev_y][ball->prev_x] = ' ';
    }
    field[ball->y][ball->x] = 'O';
    for (int y = racket_left->y - 1; y <= racket_left->y + 1; y++) {
        field[y][racket_left->x] = '|';
    }
    for (int y = racket_right->y - 1; y <= racket_right->y + 1; y++) {
        field[y][racket_right->x] = '|';
    }

    field[SCORE_Y][SCORE1_X] = '0' + score1 / 10;
    field[SCORE_Y][SCORE1_X + 1] = '0' + score1 % 10;
    field[SCORE_Y][SCORE2_X] = '0' + score2 / 10;
    field[SCORE_Y][SCORE2_X + 1] = '0' + score2 % 10;
}

void controls(char key, int *speed, Racket *racket_left, Racket *racket_right, int *quit) {
    racket_left->prev_y = racket_left->y;
    racket_right->prev_y = racket_right->y;
    switch (key) {
        case '-':
            if (*speed + TIME_INTERVAL <= MAX_INTERVAL) {
                *speed += TIME_INTERVAL;
            }
            break;

        case '+':
            if (*speed - TIME_INTERVAL >= TIME_INTERVAL) {
                *speed -= TIME_INTERVAL;
            }
            break;

        case 'a':
            if (racket_left->y != TOP + 2) {
                racket_left->y--;
            }
            break;

        case 'z':
            if (racket_left->y != BOTTOM - 2) {
                racket_left->y++;
            }
            break;

        case 'k':
            if (racket_right->y != TOP + 2) {
                racket_right->y--;
            }
            break;

        case 'm':
            if (racket_right->y != BOTTOM - 2) {
                racket_right->y++;
            }
            break;

        case 'q':
            *quit = 1;

        default:
            break;
    }
    flushinp();
}

// void copy_matrix(char **matrix_src, char **matrix_dest) {
//     for (int row = 0; row < FIELD_HEIGHT; row++) {
//         for (int col = 0; col < FIELD_WIDTH; col++) {
//             matrix_dest[row][col] = matrix_src[row][col];
//         }
//     }
// }