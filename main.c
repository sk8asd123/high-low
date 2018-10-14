#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_PLAYERS 2 /* Number of Players in the game */
#define PASS "PASS" /* A player may decline to guess */
#define ANSWER_MAX 20 /* Max number that the answer could be */

int generate_random(int max_number) {
    srand(time(NULL)); /* seed the random number generator */
    return rand() % max_number + 1;
}

int get_player_turn(int round, int offset) {
    int player_turn_num = round%NUM_PLAYERS + offset;
    if (player_turn_num > NUM_PLAYERS) {
        player_turn_num = player_turn_num - NUM_PLAYERS;
    }
    return player_turn_num;
}

void
scan_char(char *c) {
    while ((*c = getchar()) != '\n' && *c != EOF);
    /*scanf ("%c", c);*/
}

char get_command(void);
void get_player_number(int player_number_in_play);

int
main(int argc, char* argv[]) {
    char command;
    int round = 0;
    int player_number_in_play, offset, answer;

    answer = generate_random(ANSWER_MAX);
    printf("Mystery numnber is: %d\n", answer);

    printf ("Welcome to high-low!\n");
    printf ("The objective is to guess the mystery number that is an integer between 1 and 20.\n");

    player_number_in_play = generate_random(NUM_PLAYERS);
    offset = player_number_in_play;

    for (command = get_command();
    command != 'q';
    command = get_command()) {
        printf("player's turn %d\n", player_number_in_play);
        get_player_number(player_number_in_play);
        round++;
        player_number_in_play =  get_player_turn(round, offset);
        printf("round %d\n", round);
        printf("next player is %d\n", player_number_in_play);
    }
    return 0;
}

char
get_command(void) {
    char command, ignore;

    printf("Enter q to quit. Enter any other letter to continue.");
    scanf(" %c", &command);

    do
        ignore = getchar();
    while (ignore != '\n');

    return command;
}


void
get_player_number(int player_number_in_play) {
    char response[24];
    char *response_compare;

    printf ("correct player number %d\n", player_number_in_play);
    puts("Enter your player number.");
    gets(response);

    while (strtol(response, &response_compare, 10) !=  player_number_in_play) {
        puts("Please wait your turn.");
        gets(response);
    }
    puts("That is the correct player");
}