#include <ncurses.h>

#include "pong.h"

int main(void) {
    int exit_status = 0;
    char **field = create_char_matrix(FIELD_HEIGHT, FIELD_WIDTH);
    if (field != NULL) {
        Ball ball = create_ball();
        Racket racket_left = create_racket(LEFT_BOUND + BOUNDARY_OFFSET);
        Racket racket_right = create_racket(RIGHT_BOUND - BOUNDARY_OFFSET);
        int score1 = 0, score2 = 0, quit = 0;
        int speed = STARTING_SPEED;

        init_field(field, &ball, &racket_left, &racket_right);
        // copy_matrix(field, field_next);
        init_curses();
        // int counter = 0;
        while (!game_over(score1, score2) && !quit) {
            int key = getch();
            controls(key, &speed, &racket_left, &racket_right, &quit);
            // if (counter >= speed) {
                update_ball_dir(&ball, racket_left, racket_right);
                if (move_ball(&ball, &score1, &score2)) {
                    free_matrix(field, FIELD_HEIGHT);
                    endwin();
                    printf("last ball.x is %d, ball.y is %d, dirx %d, diry %d", ball.x, ball.y, ball.cur_dir_x, ball.cur_dir_y);
                    return 1;
                }
                // counter = 0;
            // }
            update_field(field, &ball, &racket_left, &racket_right, score1, score2);
            output(field);
            printw("\nThe current speed is %d\n", 11 - (speed / TIME_INTERVAL));
            printw("\nRacket 1 tcb: %d %d %d, x %d.\nRacket 2 tcb: %d %d %d, x %d.\nlast ball.x is %d, ball.y is %d, dirx %d, diry %d\n", racket_left.top, racket_left.center, racket_left.bottom, racket_left.x, racket_right.top, racket_right.center, racket_right.bottom, racket_right.x, ball.x, ball.y, ball.cur_dir_x, ball.cur_dir_y);
            refresh();
            napms(speed);
            // counter += TIME_INTERVAL;
            clear();
        }
        endwin();
        // free_matrix(field_next, FIELD_HEIGHT);
    } else {
        exit_status = 1;
    }
    free_matrix(field, FIELD_HEIGHT);
    // } else {
    //     exit_status = 1;
    // }
    return exit_status;
}

// char **field_next = create_char_matrix(FIELD_HEIGHT, FIELD_WIDTH);
//         if (field_next != NULL) {
