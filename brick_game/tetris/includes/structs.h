    #ifndef STRUCTS_H
#define STRUCTS_H

//#include <ctime>
#include <sys/types.h>
#include <time.h>

#define X_SIZE 10
#define Y_SIZE 20

typedef enum  //user Actions structure
{
    Nosig = 0,
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action,

} UserAction_t;

typedef struct //struct for time storage
{
    clock_t last_shift;
    double delay;

} TimeInfo_t;

typedef struct { //structure from exercise
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;

} GameInfo_t;

typedef struct //structure for storage figure data
{
    int type;
    int **bitmap;
    int x;
    int y;

} Figure_t;

typedef enum
{
    SPAWN = 0,
    MOVING,
    SHIFTING,
    // FLICKERING,
    ATTACHING,
    GAMEOVER,
    PAUSE,

} GameState_t;

typedef enum
{
    UP_BTN = 0,
    RIGHT_BTN,
    LEFT_BTN,
    SPACE_BTN,
    ESCAPE_BTN,
    PAUSE_BTN,
    NOSIG

} Signal_t;

typedef struct //all game data
{
    GameInfo_t info;
    Figure_t figure;
    Figure_t next_figure;
    Signal_t sig;
    GameState_t state;
    TimeInfo_t time_data;
    int **ground;

} game_data_t;

#endif
