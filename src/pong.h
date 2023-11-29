#define FIELD_HEIGHT 25
#define FIELD_WIDTH 80
#define BOUNDARY_OFFSET 5

#define TOP 0
#define BOTTOM (FIELD_HEIGHT - 1)
#define LEFT_BOUND 0
#define RIGHT_BOUND (FIELD_WIDTH - 1)

#define CENTER_X (FIELD_WIDTH / 2)
#define CENTER_Y (FIELD_HEIGHT / 2)

#define GAMEOVER_SCORE 21
#define SCORE_Y 2
#define SCORE1_X (CENTER_X - BOUNDARY_OFFSET)
#define SCORE2_X (CENTER_X + BOUNDARY_OFFSET - 1)

#define TIME_INTERVAL 25
#define STARTING_SPEED (TIME_INTERVAL * 3)
#define MAX_INTERVAL (TIME_INTERVAL * 10)

typedef enum DirectionX {
    LEFT = -1,
    RIGHT = 1
} DirectionX;

typedef enum DirectionY {
    UP = -1,
    STRAIGHT = 0,
    DOWN = 1
} DirectionY;

typedef struct Ball {
    int x;
    int y;
    DirectionX cur_dir_x;
    DirectionY cur_dir_y;
    int prev_x;
    int prev_y;
} Ball;

typedef struct Racket {
    int x;
    int y;
    int prev_y;
} Racket;

char **create_char_matrix(int rows, int cols);
void free_matrix(char **a, int rows);
void init_field(char **field, Ball *ball, Racket *racket_left, Racket *racket_right);
void output(char **field);
void init_curses(void);
int game_over(int score1, int score2);
// void gameplay_loop(char **field, Ball *ball, Racket *racket_left, Racket *racket_right, int *score1, int *score2);
void controls(char key, int *speed, Racket *racket_left, Racket *racket_right, int *quit);
void update_ball_dir(Ball *ball, Racket racket_left, Racket racket_right);
void check_racket_collision(Ball *ball, Racket racket);
// void copy_matrix(char **matrix_src, char **matrix_dest);
void update_field(char **field, Ball *ball, Racket *racket_left, Racket *racket_right, int score1, int score2);
void move_ball(Ball *ball, int *score1, int *score2);