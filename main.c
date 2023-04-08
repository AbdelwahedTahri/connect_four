#include "connect_four.h"

int main()
{
    LOGO;
    CONNECT_FOUR;

    while(1){

        char game_type = 0;
        char user_entry = 0;
        
        MAIN_SCREEN;
        game_type = getchar_in(QUIT, PVP, PVB);
        if(game_type == QUIT){

            printf("\n");
            exit(EXIT_SUCCESS);
        }
        (game_type == PVP) ? PVP_SCREEN : PVB_SCREEN;



        user_entry = getchar_in(MAIN_MENU, NEW_GAME, LOAD_GAME);

        if(user_entry == MAIN_MENU)
            continue;


        if(user_entry == NEW_GAME){

            set_player_name(PLAYER1);
            (game_type == PVP) ? set_player_name(PLAYER2) : set_computer_name(PLAYER2);

            prepare_new_game();

            print_board();
        }


        if(user_entry == LOAD_GAME){

            char *PATH = (game_type == PVP) ? PVP_SAVE_PATH : PVB_SAVE_PATH ;

            if(f_empty_save(PATH) || load_game(PATH) == 0){

                FAILED_SCREEN;
                clear_save(PATH);
                printf("\n  Redirecting to Main Menu... \n");
                delay(2);
                continue;
            }

            clear_save(PATH);
            print_board();

        }


        printf("\nQuit whenever you want by pressing Q \n");
        while(1){// The game start (continue)

            _Bool player_turn = game.player_turn;

            if(game_type == PVB && strcmp(game.player[player_turn].name, COMPUTER_NAME) == 0){

                printf("\n %s: Processing ...\n",COMPUTER_NAME);
                user_entry = make_auto_decision(player_turn);
                delay(1);
            }
            else
                user_entry = get_player_entry(player_turn);
            

            if(user_entry == QUIT_SIGNAL){
                
                SAVE_SCREEN;
                char save_it = getchar_in(SAVE_GAME,QUIT,QUIT);

                if(save_it == SAVE_GAME){

                    if(game_type == PVP)
                        save_game(PVP_SAVE_PATH);
                    else
                        save_game(PVB_SAVE_PATH);

                }

                break;
            }

            if(update_game(user_entry, player_turn) == -1){

                printf("\n  This field is full ! pick another one\n");
                continue;
            }

            print_board();

            if(f_winner(player_turn)){

                printf("\n/-------------------------------/\n");
                printf("\n  %s has won the game \n", game.player[player_turn].name);
                printf("\n/-------------------------------/\n");
                delay(2);
                break;
            }

            if(f_drawn_game()){

                printf("\n/-------------------------------/\n");
                printf("\n  This game ended in a draw \n");
                printf("\n/-------------------------------/\n");
                delay(2);
                break;
            }

        }

    }
}
