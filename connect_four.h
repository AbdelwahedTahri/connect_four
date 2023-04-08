#ifndef CONNECT_FOUR_H
#define CONNECT_FOUR_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>


#define LOGO                printf("          ___ \n        ,\"---\". \n        :     ; \n         `-.-' \n          | | \n          | | \n          | | \n       _.-\\_/-._\n    _ / |     | \\ _\n   / /   `---'   \\ \\\n  /  `-----------'  \\ \n /,-\"\"-.       ,-\"\"-.\\ \n( i-..-i       i-..-i ) \n|`|    |-------|    |'| \n\\ `-..-'  ,=.  `-..-'/ \n `--------|=|-------' \n          | | \n          \\ \\ \n           ) ) \n          / / \n         ( ( \n ")
#define CONNECT_FOUR        printf("                                 _      __ \n  ___ ___  _ __  _ __   ___  ___| |_   / _| ___  _   _ _ __ \n / __/ _ \\| '_ \\| '_ \\ / _ \\/ __| __| | |_ / _ \\| | | | '__|\n| (_| (_) | | | | | | |  __/ (__| |_  |  _| (_) | |_| | |   \n \\___\\___/|_| |_|_| |_|\\___|\\___|\\__| |_|  \\___/ \\__,_|_|   \n\n\n")
#define MAIN_SCREEN         printf("\n/-------Game Mode---------------/\n|                               |\n|   [P]layer vs Player          |\n|   [C]omputer vs Player        |\n|   [Q]uit the game             |\n|                               |\n/-------------------------------/\n")
#define PVP_SCREEN          printf("\n/ ------Player vs Player--------/\n|                               |\n|   [N]ew game                  |\n|   [L]oad game                 |\n|   [M]ain screen               |\n|                               |\n/-------------------------------/\n")
#define PVB_SCREEN          printf("\n/ ------Player vs Bot-----------/\n|                               |\n|   [N]ew game                  |\n|   [L]oad game                 |\n|   [M]ain screen               |\n|                               |\n/-------------------------------/\n")
#define SAVE_SCREEN         printf("\n/ ---------Save game------------/\n|                               |\n|   [S]ave the game             |\n|   [Q]uit without saving       |\n|                               |\n/-------------------------------/\n\n")
#define FAILED_SCREEN       printf("\n/-------------------------------/\n|                               |\n|     No DATA FOUND             |\n|                               |\n/-------------------------------/\n")
#define RANDOM_SEED         srand((unsigned) time(NULL))

#define ROW             (6)
#define COLUMN          (7)
#define MAX_TURN        (42)
#define PLAYER1         (0)
#define PLAYER2         (1)
#define PLAYER1_TOKEN   ('X')
#define PLAYER2_TOKEN   ('O')
#define COMPUTER_NAME   ("Computer") // It should not contain more than 20 character (19 +'\0')

#define NEW_GAME        ('n')
#define LOAD_GAME       ('l')
#define MAIN_MENU       ('m')
#define QUIT            ('q')
#define PVP             ('p')
#define PVB             ('c')
#define SAVE_GAME       ('s')
#define PVP_SAVE_PATH   "PVP.yml"
#define PVB_SAVE_PATH   "PVB.yml"
#define ERROR           (-1)
#define QUIT_SIGNAL     (-1)


struct position {

    unsigned x;                         //Line coordinate
    unsigned y;                         //Column coordinate
};

struct player {

    char            name[20];           // Player name
    char            token;              // 'X' or 'O' can be changed from the macros
    struct position last_played_position;/* Used by some functions to check various things*/

};

struct game {

    char            board[ROW][COLUMN]; // Store fields values
    unsigned short  turn_number;        // The game ends when it's equal to 42
    unsigned short  player_turn;        // Store the player index so i know who plays next
    struct player   player[2];          // Two players, the Bot is a player which have the name COMPUTER_NAME
};

enum save_state {
    
    EMPTY_SAVE = 1,
    CORRUPTED_SAVE = -1,
    NOT_EMPTY_SAVE = 0
};

//Global Variable
extern struct game game;


//functions
extern void print_board();

extern void prepare_new_game();
extern int f_empty_save(char *PATH);
extern _Bool load_game(char *PATH);
extern void save_game(char *PATH);
extern void clear_save(char *PATH);

extern void set_player_name(unsigned player_index);
extern void set_computer_name(unsigned player_index);

extern int get_player_entry(unsigned player_index);
extern int make_auto_decision(unsigned Player_index);

extern unsigned get_max_successive(char token, struct position pos);
extern int update_game(unsigned player_entry, unsigned player_index);
extern _Bool f_winner(unsigned player_index);
extern _Bool f_drawn_game();

extern char getchar_in(char a, char b, char c);
extern void delay(int number_of_seconds);

#endif