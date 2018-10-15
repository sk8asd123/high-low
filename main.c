#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_PLAYERS 2 /* Number of Players in the game */
#define PASS "PASS\n" /* A player may decline to guess */
#define ANSWER_MAX 20 /* Max number that the answer could be */
#define ALLOWED_PASSES 3 /* Max allowed passes that a user can use during the game */

typedef enum
{
    initial,
    guessed,
    passed
} move_t;



struct player_data{
    int num_passes; /* The number of passes a user has used in the game so far. */
    move_t last_move; /* Indicates if the player used a pass during the last turn. */
};


void initialize_player_data(struct player_data* ptr, int size);
int get_player_turn(int round, int offset);
int generate_random(int num);
char get_command(void);
void get_player_number(int num);
void evaluate_guess(long num1, int num2);
long get_player_action(struct player_data* ptr, int num);
void enum_to_string(move_t last_move, char* last_move_name, int name_length);

int main(int argc, char* argv[]) {
    char command;
    int round = 0;
    int i, player_number_in_play, offset, answer;
    struct player_data p_data[NUM_PLAYERS]; /* array of structures */
    char move_name[24];

    answer = generate_random(ANSWER_MAX);

    printf ("Welcome to high-low!\n");
    printf ("The objective is to guess the mystery number that is an integer between 1 and %d.\n", ANSWER_MAX);
    printf ("Rules: 1) You are allowed only %d passes per game. 2) You cannot pass twice in a row.\n", ALLOWED_PASSES);

    player_number_in_play = generate_random(NUM_PLAYERS);
    offset = player_number_in_play;

    initialize_player_data(p_data, NUM_PLAYERS);

    for (command = get_command();
    command != 'q';
    command = get_command()) {
        printf("It is player's turn %d to guess.\n", player_number_in_play);
        get_player_number(player_number_in_play);
        round++;
        player_number_in_play =  get_player_turn(round, offset);

        printf("Round %d\n", round);

        if (get_player_action(&p_data[player_number_in_play - 1], answer) == answer) {
            break;
        }
    }

    printf("Player Statistics:\n");
    for (i = 0; i < NUM_PLAYERS; i++) {
        memset(move_name, 0, sizeof (move_name));
        enum_to_string (p_data[i].last_move, move_name, sizeof(move_name));
        printf("Player: %d | Number of passes in the game: %d | The last move made: %s\n", i+1,
                p_data[i].num_passes, move_name);
    }

    return 0;
}


void initialize_player_data(struct player_data* ptr, int size) {
    int player;
    for (player = 0; player < size; player++) {
        ptr[player].num_passes = 0;
        ptr[player].last_move = initial;
    }
}


/* Random number generator that takes a max number. It is always greater than 1. */
int  generate_random(int num) {
    srand(time(NULL)); /* seed the random number generator */
    return rand() % num + 1;
}


/* Once the first player is randomly picked, the remaining players take turns in order after the first player. */
int  get_player_turn(int round, int offset) {
    int player_turn_num = round%NUM_PLAYERS + offset;
    if (player_turn_num > NUM_PLAYERS) {
        player_turn_num = player_turn_num - NUM_PLAYERS;
    }
    return player_turn_num;
}


/* If a player wants to quit, they can type 'q' */
char get_command(void) {
    char command, ignore;

    printf("Enter q to quit. Enter any other letter to continue.\n");
    scanf(" %c", &command);

    do
        ignore = getchar();
    while (ignore != '\n' && ignore != EOF);

    return command;
}


/* Check to make sure the correct player is playing. */
void get_player_number(int num) {
    char response[24];
    char *response_compare;

    puts("Enter your player number.");
    fgets(response, sizeof(response), stdin);

    while (strtol(response, &response_compare, 10) !=  num) {
        puts("Please wait your turn.");
        fgets(response, sizeof(response), stdin);
    }
}


/* Judge the guess entered by the player. */
void evaluate_guess(long num1, int num2) {
    if (num1  < num2) {
        printf("The guess is too low.\n");
    }
    else if (num1  > num2) {
        printf("The guess is too high.\n");
    }
    else {
        printf("The guess is correct; congratulations!\n");
    }
}

/* Evaluate player action. */
long get_player_action(struct player_data* ptr, int answer) {
    char response[24];
    char *number_response;
    long return_guess = -1;

    printf("Enter your guess or type %s", PASS);
    fgets(response, sizeof(response), stdin);

    if (strcmp (response, PASS) == 0) {
        if (ptr->num_passes == 3) {
            printf("You have exceeded the number of allowed passes, %d", ALLOWED_PASSES);
            puts("Please enter an integer guess.");
            return_guess = strtol(fgets(response, sizeof(response), stdin), &number_response, 10);
            printf("your guess %ld\n", return_guess);
            evaluate_guess (return_guess, answer);
            ptr->last_move = guessed;
        }
        else if (ptr->last_move == passed) {
            printf("You cannot pass twice in a row. Please enter an integer guess.\n");
            return_guess = strtol(fgets(response, sizeof(response), stdin), &number_response, 10);
            evaluate_guess (return_guess, answer);
            ptr->last_move = guessed;
        }
        else {
            ptr->num_passes++;
            printf("You have passed %d number of times, you have %d more times left\n", ptr->num_passes,
                   ALLOWED_PASSES - ptr->num_passes);
            ptr->last_move = passed;
        }
    }
    else {
        return_guess = strtol(response, &number_response, 10);
        evaluate_guess (return_guess, answer);
        ptr->last_move = guessed;
    }

    return return_guess;
}


/* Allows the printing of enum values */
void enum_to_string(move_t last_move, char* last_move_name, int name_length){
    switch (last_move){
        case initial:
            strncpy (last_move_name, "initial", name_length);
            break;

        case guessed:
            strncpy (last_move_name, "guessed", name_length);
            break;

        case passed:
            strncpy (last_move_name, "passed", name_length);
            break;

        default:
            strncpy (last_move_name, "INVALID", name_length);
    }
}