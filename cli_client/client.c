#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ncurses.h>

#include <elevator_system.h>
#include <avsystem/commons/avs_vector.h>

#define GREEN_ON_BLACK 1

#define WITH_ATTR(attr, fun, ...) attron(attr); fun(__VA_ARGS__); attroff(attr)

int main() {
    printf("enter elevator count: ");
    fflush(stdout);

    size_t elevator_count;
    if (scanf(" %zu", &elevator_count) != 1) {
        fprintf(stderr, "malformed input\n");
        return EXIT_FAILURE;
    }

    if (elevator_count == 0) {
        fprintf(stderr, "elevator count must be positive\n");
        return EXIT_FAILURE;
    }

    printf("enter floor count: ");
    fflush(stdout);

    size_t floor_count;
    if (scanf(" %zu", &floor_count) != 1) {
        fprintf(stderr, "malformed input\n");
        return EXIT_FAILURE;
    }

    if (floor_count == 0) {
        fprintf(stderr, "floor count must be positive\n");
        return EXIT_FAILURE;
    }

    //skip remaining data in stdin
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

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
    mvprintw( 1, 0, "1. use "); WITH_ATTR(A_BOLD, printw, "ARROWS"); printw(" to select");
    mvprintw( 2, 0, "   floors/elevators");
    mvprintw( 3, 0, "2. use "); WITH_ATTR(A_BOLD, printw, "U"); printw(" to request a travel up");
    mvprintw( 4, 0, "   for the selected floor");
    mvprintw( 5, 0, "3. use "); WITH_ATTR(A_BOLD, printw, "D"); printw(" to request a travel down");
    mvprintw( 6, 0, "   for the selected floor");
    mvprintw( 7, 0, "4. use "); WITH_ATTR(A_BOLD, printw, "O"); printw(" to request a drop-off");
    mvprintw( 8, 0, "   for the selected floor/elevator");
    mvprintw( 9, 0, "5. to advance, press "); WITH_ATTR(A_BOLD, printw, "SPACE"); printw(" (1 step)");
    mvprintw(10, 0, "   or keys "); WITH_ATTR(A_BOLD, printw, "1-9"); printw(" for 2^n steps");
    mvprintw(11, 0, "6. quit with "); WITH_ATTR(A_BOLD, printw, "Q");

    elevator_system el_system;
    elevator_system_init(&el_system, elevator_count, floor_count);

    size_t curr_elevator = 0;
    size_t curr_floor = 0;

    while (true) {
        //draw...
        refresh();
        
        //handle input...
        int chr = getch();
        switch (chr) {
            case KEY_UP: {
                if (curr_floor < AVS_VECTOR_SIZE(el_system.floors) - 1) {
                    curr_floor++;
                }
            } break;
            case KEY_DOWN: {
                if (curr_floor > 0) {
                    curr_floor--;
                }
            } break;
            case KEY_LEFT: {
                if (curr_elevator > 0) {
                    curr_elevator--;
                }
            } break;
            case KEY_RIGHT: {
                if (curr_elevator < AVS_VECTOR_SIZE(el_system.elevators) - 1) {
                    curr_elevator++;
                }
            } break;
            case 'u': {
                elevator_system_request_pickup(&el_system, curr_floor, DIR_UP);
            } break;
            case 'd': {
                elevator_system_request_pickup(&el_system, curr_floor, DIR_DOWN);
            } break;
            case 'o': {
                elevator_system_request_dropoff(&el_system, curr_elevator, curr_floor);
            } break;
            case ' ': {
                elevator_system_step(&el_system);
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
                int power = chr - '0';
                size_t steps = 1;
                while (power-- > 0) {
                    steps *= 2;
                }
                while (steps-- > 0) {
                    elevator_system_step(&el_system);
                }
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
}
