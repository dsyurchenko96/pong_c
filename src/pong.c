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

    field[ball->y][ball->x] = 'O';
    for (int y = CENTER_Y - 1; y <= CENTER_Y + 1; y++) {
        field[y][racket_left->x] = '|';
        field[y][racket_right->x] = '|';
    }
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
    curs_set(FALSE);
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
    check_top_bottom(ball);
    if (ball_hits_racket(ball, racket_left)) {
        change_dir_on_collision(ball, racket_left);
    } else if (ball_hits_racket(ball, racket_right)) {
        change_dir_on_collision(ball, racket_right);
    }
    check_top_bottom(ball);
}

int ball_hits_racket(Ball *ball, Racket racket) {
    return (ball->x + ball->cur_dir_x == racket.x || ball->x == racket.x) &&
           ball->y >= racket.top && ball->y <= racket.bottom;
}

void change_dir_on_collision(Ball *ball, Racket racket) {
    if (ball->cur_dir_y == STRAIGHT) {
        if (ball->y == racket.top) {
            ball->cur_dir_y = UP;
        } else if (ball->y == racket.center) {
            ball->cur_dir_y = STRAIGHT;
        } else if (ball->y == racket.bottom) {
            ball->cur_dir_y = DOWN;
        }
    } else {
        if ((ball->cur_dir_y == DOWN && ball->y == racket.top) ||
            (ball->cur_dir_y == UP && ball->y == racket.bottom)) {
            ball->cur_dir_y *= -1;
        } else if (ball->y == racket.center) {
            ball->cur_dir_y = STRAIGHT;
        }
    }
    ball->cur_dir_x *= -1;
}

void check_top_bottom(Ball *ball) {
    if (ball->y + ball->cur_dir_y == TOP || ball->y + ball->cur_dir_y == BOTTOM) {
        ball->cur_dir_y *= -1;
    }
}

int move_ball(Ball *ball, int *score1, int *score2) {
    if (ball->y <= TOP || ball->y >= BOTTOM) {
        return 1;
    }
    ball->prev_x = ball->x;
    ball->prev_y = ball->y;
    ball->x += ball->cur_dir_x;
    ball->y += ball->cur_dir_y;
    if (ball->x == LEFT_BOUND) {
        ball->x = CENTER_X;
        (*score2)++;
    } else if (ball->x == RIGHT_BOUND) {
        ball->x = CENTER_X;
        (*score1)++;
    }
    return 0;
}

void update_field(char **field, Ball *ball, Racket *racket_left, Racket *racket_right) {
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
    for (int y = racket_left->top; y <= racket_left->bottom; y++) {
        field[y][racket_left->x] = '|';
    }
    for (int y = racket_right->top; y <= racket_right->bottom; y++) {
        field[y][racket_right->x] = '|';
    }
}

void update_field_score(char **field, int score1, int score2) {
    field[SCORE_Y][SCORE1_X] = '0' + score1 / 10;
    field[SCORE_Y][SCORE1_X + 1] = '0' + score1 % 10;
    field[SCORE_Y][SCORE2_X] = '0' + score2 / 10;
    field[SCORE_Y][SCORE2_X + 1] = '0' + score2 % 10;
}

void controls(char key, int *speed, Racket *racket_left, Racket *racket_right, int *quit) {
    // racket_left->prev_y = racket_left->y;
    // racket_right->prev_y = racket_right->y;
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
            if (racket_left->top - 1 > TOP) {
                racket_left->top--;
                racket_left->center--;
                racket_left->bottom--;
            }
            break;

        case 'z':
            if (racket_left->bottom + 1 < BOTTOM) {
                racket_left->top++;
                racket_left->center++;
                racket_left->bottom++;
            }
            break;

        case 'k':
            if (racket_right->top - 1 > TOP) {
                racket_right->top--;
                racket_right->center--;
                racket_right->bottom--;
            }
            break;

        case 'm':
            if (racket_right->bottom + 1 < BOTTOM) {
                racket_right->top++;
                racket_right->center++;
                racket_right->bottom++;
            }
            break;

        case 'q':
            *quit = 1;

        default:
            break;
    }
    flushinp();
}

Racket create_racket(int x) {
    Racket racket;
    racket.x = x;
    racket.center = CENTER_Y;
    racket.top = CENTER_Y - 1;
    racket.bottom = CENTER_Y + 1;
    return racket;
}

Ball create_ball(void) {
    Ball ball;
    ball.x = CENTER_X;
    ball.y = CORNER_SHOT_Y;
    ball.cur_dir_x = LEFT;
    ball.cur_dir_y = DOWN;
    return ball;
}

