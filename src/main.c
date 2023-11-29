#include <ncurses.h>

#include "pong.h"

int main(void) {
    int exit_status = 0;
    char **field = create_char_matrix(FIELD_HEIGHT, FIELD_WIDTH);
    if (field != NULL) {
        Ball ball;
        Racket racket_left;
        Racket racket_right;
        int score1 = 0, score2 = 0, quit = 0;
        int speed = STARTING_SPEED;

        init_field(field, &ball, &racket_left, &racket_right);
        // copy_matrix(field, field_next);
        init_curses();
        int counter = 0;
        while (!game_over(score1, score2) && !quit) {
            int key = getch();
            controls(key, &speed, &racket_left, &racket_right, &quit);
            if (counter >= speed) {
                update_ball_dir(&ball, racket_left, racket_right);
                move_ball(&ball, &score1, &score2);
                counter = 0;
            }
            update_field(field, &ball, &racket_left, &racket_right, score1, score2);
            output(field);
            printw("\nThe current speed is %d\n", 11 - (speed / TIME_INTERVAL));
            refresh();
            napms(TIME_INTERVAL);
            counter += TIME_INTERVAL;
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
