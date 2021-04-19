#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ncurses.h>

#include <elevator_system.h>
#include <avsystem/commons/avs_vector.h>

#define GREEN_ON_BLACK 1

int main() {
    printf("enter elevator count: ");
    fflush(stdout);

    size_t el_count;
    if (scanf(" %zu", &el_count) != 1) {
        fprintf(stderr, "malformed input\n");
        return EXIT_FAILURE;
    }

    //skip remaining data in stdin
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    if (el_count == 0) {
        fprintf(stderr, "elevator count must be positive\n");
        return EXIT_FAILURE;
    }

    printf("enter floor labels, in increasing order, separated with spaces (e.g. -1 ground 0 1 2): ");
    fflush(stdout);

    AVS_VECTOR(char*) floor_labels = AVS_VECTOR_NEW(char*);

    char *line = NULL;
    size_t line_len = 0;
    if (getline(&line, &line_len, stdin) > 0) {
        char *line_curr = line;
        
        while (*line_curr) {
            char *label = NULL;
            int n = 0;

            if (sscanf(line_curr, " %ms %n", &label, &n) != 1) {
                fprintf(stderr, "malformed input\n");
                return EXIT_FAILURE;
            }
            else {
                AVS_VECTOR_PUSH(&floor_labels, &label);
            }

            line_curr += n;
        }
    }
    else {
        fprintf(stderr, "malformed input\n");
        return EXIT_FAILURE;
    }

    free(line);

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "your terminal doesn't support colors :-(\n");
        return EXIT_FAILURE;
    }

    use_default_colors();
    start_color();
    init_pair(GREEN_ON_BLACK, COLOR_GREEN, -1);

    attron(COLOR_PAIR(GREEN_ON_BLACK));
    mvprintw(0, 0, "ELEVATOR SYSTEM");
    attroff(COLOR_PAIR(GREEN_ON_BLACK));
    mvprintw(1, 0, "1. use "); attron(A_BOLD); printw("ARROWS"); attroff(A_BOLD); printw(" to select");
    mvprintw(2, 0, "   floors/elevators");
    mvprintw(3, 0, "2. use "); attron(A_BOLD); printw("P"); attroff(A_BOLD); printw(" to request a pickup");
    mvprintw(4, 0, "   for the selected floor");
    mvprintw(5, 0, "3. use "); attron(A_BOLD); printw("D"); attroff(A_BOLD); printw(" to request a dropoff");
    mvprintw(6, 0, "   for the selected floor/elevator");
    mvprintw(7, 0, "4. to advance, press "); attron(A_BOLD); printw("SPACE"); attroff(A_BOLD); printw(" (1 step)");
    mvprintw(8, 0, "   or keys "); attron(A_BOLD); printw("1-9"); attroff(A_BOLD); printw(" for 2^n steps");
    mvprintw(9, 0, "5. quit with "); attron(A_BOLD); printw("Q"); attroff(A_BOLD);

    while (true) {
        //draw...
        refresh();
        
        //handle input...
        int chr = getch();
        switch (chr) {
            case KEY_UP: {

            } break;
            case KEY_DOWN: {

            } break;
            case KEY_LEFT: {

            } break;
            case KEY_RIGHT: {

            } break;
            case 'p': {

            } break;
            case 'd': {

            } break;
            case ' ': {

            } break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {

            } break;
            case 'q': {
                goto quit;
            } break;
            default:
            break;
        };
    }

    quit:
    endwin();

    char **elem;
    AVS_VECTOR_CLEAR(&floor_labels, elem) {
        free(*elem);
    }

    AVS_VECTOR_DELETE(&floor_labels);
}
