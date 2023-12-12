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

        init_curses();
        init_field(field, &ball, &racket_left, &racket_right);
        update_field_score(field, score1, score2);

        int ball_timer = 0;
        while (!game_over(score1, score2) && !quit) {
            int key = getch();
            controls(key, &speed, &racket_left, &racket_right, &quit);
            if (ball_timer >= speed) {
                update_ball_dir(&ball, racket_left, racket_right);
                move_ball(&ball, &score1, &score2);
                ball_timer = 0;
            }
            update_field(field, &ball, &racket_left, &racket_right);
            update_field_score(field, score1, score2);

            output(field, speed);
            refresh();
            napms(TIME_INTERVAL);
            ball_timer += TIME_INTERVAL;
            clear();
        }
        endwin();
    } else {
        exit_status = 1;
    }
    free_matrix(field, FIELD_HEIGHT);
    return exit_status;
}