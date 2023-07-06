
#include "connect_four.h"


struct game game;


/***********************Utilities*********************************/

void delay(int number_of_seconds)
{
    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + number_of_seconds * CLOCKS_PER_SEC);
}

int random_number(unsigned min, unsigned max)
{
    /*
     * Returns a random integer between min and max
     */
    return (rand() / (RAND_MAX + 1.)) * (max - min + 1) + min;
}

int char2int(char number)
{
    return (int)number - 48;
}

char getchar_in(char a, char b, char c)
{
    /**
     * Ask the user to put one of the 3 valid values
     * returns the user entry
     */
    char entry = 0;
    printf("\n  > Your choice  : ");

    while(scanf("%c", &entry) != 1 || (tolower(entry) != a && tolower(entry) != b && tolower(entry) != c)){

        printf("  Wrong value, Enter a valid value  : ");
        while(fgetc(stdin) != '\n');
    }
    while(fgetc(stdin) != '\n');
    
    return tolower(entry);
}

/******************** Game core functions************************/

void prepare_new_game()
{

    // Who play first 
    RANDOM_SEED;
    game.player_turn = random_number(PLAYER1, PLAYER2);

    // Set there tokens
    if(game.player_turn == PLAYER1){

        game.player[0].token = PLAYER1_TOKEN;
        game.player[1].token = PLAYER2_TOKEN;

    }else{

        game.player[0].token = PLAYER2_TOKEN;
        game.player[1].token = PLAYER1_TOKEN;
    }
    
    //reset turn_number
    game.turn_number = 0;

    //fill the board with spaces
    for(unsigned i = 0; i < ROW; i++)
        for(unsigned j = 0; j < COLUMN; j++)
            game.board[i][j] = ' ';
}

_Bool load_game(char *PATH)
{
    _Bool is_loaded = 1;

    // Open the save file
    FILE *file = fopen(PATH,"r");
    if(file == NULL){

        perror("Open file");
        return 0;
    }

    // Get The board fields
    for(int i = 0 ;i < ROW; i++)
        for(int j = 0 ; j < COLUMN; j++){
            if(i == 0 && j == 0){

                if(fscanf(file, "BOARD_STATE : ['%c'",&game.board[i][j]) != 1)
                    goto error;

                continue ;
                
            }
            if(fscanf(file, ", '%c'", &game.board[i][j]) != 1)
                goto error;
        }   
    while(fgetc(file) != '\n');

    // Get Turn number
    if(fscanf(file, "TURN_NUMBER : %u\n", &game.turn_number) != 1)
        goto error;

    // Get the index of the player who should play first
    if(fscanf(file, "PLAYER_TURN : %u\n", &game.player_turn) != 1)
        goto error;

    // Get Players DATA
    for(int i = 0; i < 2; i++){

        if(fscanf(file, "PLAYER%d_NAME : %s\n", &i, game.player[i].name) != 2)
        goto error;

        if(fscanf(file, "PLAYER%d_TOKEN : %c\n", &i, &game.player[i].token) != 2)
        goto error;

        if(fscanf(file, "PLAYER%d_LAST_PLAYED_POSITION_X : %u\n", &i, &game.player[i].last_played_position.x) !=2)
        goto error;

        if(fscanf(file, "PLAYER%d_LAST_PLAYED_POSITION_Y : %u\n", &i, &game.player[i].last_played_position.y) !=2)
        goto error;

    }

close:
    if(fclose(file) == EOF)
        perror("Closing file");

    return is_loaded;

error:
    fprintf(stderr, "\n  Can't Load data correctly");
    is_loaded = 0;
    goto close;
}

void set_player_name(unsigned player_index)
{
    /**
     * Asking the user to choose a name
     * The name must be different from the macro COMPUTER_NAME
    */
    printf("\n > Choose your name, Player[%u]  : ", player_index + 1);
    while(scanf("%20s", game.player[player_index].name) != 1 || strcmp(game.player[player_index].name, COMPUTER_NAME) == 0 ){

        printf("  Invalid name, please choose another name  : ");
        while(fgetc(stdin) != '\n');
    }
    while(fgetc(stdin) != '\n');

}

void set_computer_name(unsigned player_index)
{
    strcpy(game.player[player_index].name, COMPUTER_NAME);
}

int get_player_entry(unsigned player_index)
{
    /**
     * returns The column number where the player wants to play
     * or -1 if he want to quit the game by typing 'q' or 'Q'
    */
    char entry = 0;

ask:
    printf("\n %s  : ", game.player[player_index].name);

    if(scanf("%c", &entry) != 1){

        printf("\n  An Error has occurred, please reinsert a value\n");
        while(fgetc(stdin) != '\n');
        goto ask;       
    }
    while(fgetc(stdin) != '\n');

    if(tolower(entry) == QUIT)
        return QUIT_SIGNAL;
    
    if(!(char2int(entry) >= 1 && char2int(entry) <= 7)){

        printf("\n  Wrong value, the value must be between 1 and 7\n");
        goto ask; 
    }

    return char2int(entry) - 1;

}

int make_auto_decision(unsigned Player_index)
{
    /*
     * return the column number where the computer will play
     */

    int column[COLUMN];

    int decision = -1;
    int max = -1;
    int max_index = -1;
    int min_index = -1;

    for(int j = 0; j < COLUMN; j++)
        for(int i = ROW-1; i >= 0; i--)

            if(game.board[i][j] == ' '){

                int x = 0, o = 0;

                x = get_max_successive(PLAYER1_TOKEN, (struct position){i, j});
                if(game.player[Player_index].token == PLAYER1_TOKEN && x == 4)
                    return j;

                o = get_max_successive(PLAYER2_TOKEN, (struct position){i, j});
                if(game.player[Player_index].token == PLAYER2_TOKEN && o == 4)
                    return j;

                column[j] = (x >= o) ? x : o ;

                if(column[j] >= max){

                    if(column[j] == max ){

                        max_index = j;
                    }else{

                        min_index = j;
                        max = column[j];
                    }
                }

                break;
            }
    
    if(min_index < max_index){  // means that max is found multiple times in the array

        RANDOM_SEED;
        for(decision = random_number(min_index,max_index); column[decision] != max; decision = random_number(min_index,max_index));
        
        return decision;

    }else{

        return min_index;
    }
}

int update_game(unsigned player_entry, unsigned player_index)
{
    /**
     * If the player entry is correct, the game parameters will be updated and the function return a number >= 0
     * else the function return -1; 
    */

    int i = ROW - 1;
    while(i >= 0){

        if(game.board[i][player_entry] != ' '){

            i--;
            continue;
        }

        game.board[i][player_entry] = game.player[player_index].token;
        game.player[player_index].last_played_position = (struct position){i, player_entry};
        game.player_turn = !game.player_turn;
        game.turn_number++;

        return i;               
    }

    return ERROR;       

}

void print_board()
{
    /*
     * print "board[][]" values inside grids
     * works only for 6 * 7 Matrix
     */
    printf("\n  1   2   3   4   5   6   7  \n");
    
    for(unsigned i = 0; i < ROW; i++){

        printf("+---+---+---+---+---+---+---+\n");
        for(unsigned j = 0; j < COLUMN; j++)
        {
            printf("| %c ", game.board[i][j]);
        }
        printf("|\n");
    }
    printf("+---+---+---+---+---+---+---+\n");
    printf("  1   2   3   4   5   6   7  \n");
}

unsigned get_max_successive(char token, struct position pos)
{
    /*
     *  search for the number of successive tokens in each direction starting from position 'pos'
     *  then it returns the max found
     *  this function does not take account of token type in position 'pos'
     *  e.g : XXOX <=> XXXX with 'pos' is the 'O' position
     */

    int x = pos.x;
    int y = pos.y;
    
    unsigned horizontal_line = 0;
    unsigned vertical_line   = 0;
    unsigned d_left_to_right = 0;
    unsigned d_right_to_left = 0;
    unsigned aux = 0;

    unsigned max = 0;


    //check the horizontal line
    for(int j = 1; y+j < COLUMN && game.board[x][y+j] == token; aux = j++);

    for(int j = 1; y-j >= 0 && game.board[x][y-j] == token; horizontal_line = j++);
    horizontal_line += aux + 1;

    //check the vertical line
    for(int i = 1; x+i < ROW && game.board[x+i][y] == token; vertical_line = i++);
    vertical_line++;

    // check the diagonal '/'
    aux = 0;

    for(int i = 1, j = 1; (x-i >= 0 && y+j < COLUMN) && game.board[x-i][y+j] == token; aux = i++, j++);
 
    for(int i = 1, j = 1; (x+i < ROW && y-j >= 0) && game.board[x+i][y-j] == token; d_left_to_right = i++, j++);
    d_left_to_right += aux + 1;

    // check the diagonal '\'
    aux = 0;

    for(int i = 1, j = 1; (x-i >= 0 && y-j >= 0) && game.board[x-i][y-j] == token; aux = i++, j++);

    for(int i = 1, j = 1; (x+i < ROW && y+j < COLUMN) && game.board[x+i][y+j] == token; d_right_to_left = i++, j++);
    d_right_to_left += aux + 1;

    // Find and store max
    max = (vertical_line > horizontal_line)? vertical_line : horizontal_line ;
    max = (max > d_left_to_right)? max : d_left_to_right ;
    max = (max > d_right_to_left)? max : d_right_to_left ;

    return max;
}

_Bool f_winner(unsigned player_index)
{
    if(get_max_successive(game.player[player_index].token, game.player[player_index].last_played_position) >= 4)
        return 1;
    
    return 0; 
}

_Bool f_drawn_game()
{
    if(game.turn_number == MAX_TURN)
        return 1;
    
    return 0;
}

int f_empty_save(char *PATH)
{
    /**
     * 1  empty save
     * 0  not empty
     * -1 error while reading or can't open the stream
    */
    FILE* file=fopen(PATH,"a+");
    int return_value = NOT_EMPTY_SAVE;

    if(file == NULL){

        perror("Opening file");
        return CORRUPTED_SAVE;
    }

    fseek(file,0,SEEK_SET);
    if(fgetc(file)== EOF){

        if(feof(file) != 0){

            return_value = EMPTY_SAVE;
        }else{

            perror("Reading file");
            return_value = CORRUPTED_SAVE;
        }     
    }

    if(fclose(file) == EOF)
        perror("Closing file");

    return return_value;
}

void clear_save(char *PATH)
{

    FILE* file=fopen(PATH,"w");
    if(file == NULL){

        perror("Opening file");
        return;
    }

    if(fclose(file) == EOF)
        perror("Closing file");

}

void save_game(char *PATH)
{
    // Open the save file
    FILE *file = fopen(PATH, "w");
    if(file == NULL){

        perror("Open file");
        return;
    }

    // Store The board fields
    fprintf(file, "BOARD_STATE : [");
    for(int i = 0 ;i < ROW; i++)
        for(int j = 0 ; j < COLUMN; j++)
            fprintf(file, "'%c', ", game.board[i][j]);
    
    fseek(file, -2, SEEK_CUR);
    fprintf(file, "]\n");

    // Save the turn number
    fprintf(file, "TURN_NUMBER : %u\n", game.turn_number);

    // Save the index of the player who should play first next time
    fprintf(file, "PLAYER_TURN : %u\n", game.player_turn);

    // Save Players DATA
    for(int i = 0; i < 2; i++){

        fprintf(file, "PLAYER%d_NAME : %s\n", i, game.player[i].name);
        fprintf(file, "PLAYER%d_TOKEN : %c\n", i, game.player[i].token);
        fprintf(file, "PLAYER%d_LAST_PLAYED_POSITION_X : %u\n", i, game.player[i].last_played_position.x);
        fprintf(file, "PLAYER%d_LAST_PLAYED_POSITION_Y : %u\n", i, game.player[i].last_played_position.y);
    }

    // Close file
    if(fclose(file) == EOF)
        perror("Closing file");

    return;
}

