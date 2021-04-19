#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include <elevator_system.h>
#include <avsystem/commons/avs_vector.h>

#define GREEN_ON_BLACK 1
#define YELLOW_ON_BLACK 2

#define WITH_ATTR(attr, fun, ...) attron(attr); fun(__VA_ARGS__); attroff(attr)
#define BEG_X 40

void rectangle_fill(int y, int x, int h, int w, char chr) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            mvaddch(y + i, x + j, chr);
        }
    }
}

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
    keypad(stdscr, true);

    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "your terminal doesn't support colors :-(\n");
        return EXIT_FAILURE;
    }

    use_default_colors();
    start_color();
    init_pair(GREEN_ON_BLACK, COLOR_GREEN, -1);
    init_pair(YELLOW_ON_BLACK, COLOR_YELLOW, -1);

    WITH_ATTR(COLOR_PAIR(GREEN_ON_BLACK), mvprintw, 0, 0, "ELEVATOR SYSTEM");
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
    mvprintw(13, 0, "step: ");

    elevator_system el_system;
    elevator_system_init(&el_system, elevator_count, floor_count);

    size_t curr_elevator = 0;
    size_t curr_floor = 0;
    size_t curr_step = 0;

    while (true) {
        //draw elevator numbers
        for (size_t i = 0; i < elevator_count; i++) {
            if (i == curr_elevator) {
                attron(COLOR_PAIR(YELLOW_ON_BLACK));
                attron(A_BOLD);
            }
            mvprintw(0, BEG_X + 4 * i, "%3d", i);
            if (i == curr_elevator) {
                attroff(COLOR_PAIR(YELLOW_ON_BLACK));
                attroff(A_BOLD);
            }
        }

        //draw floor numbers
        for (size_t i = 0; i < floor_count; i++) {
            if (i == curr_floor) {
                attron(COLOR_PAIR(YELLOW_ON_BLACK));
                attron(A_BOLD);
            }
            mvprintw(2 * (floor_count - i), BEG_X - 3, "%3d", i);
            if (i == curr_floor) {
                attroff(COLOR_PAIR(YELLOW_ON_BLACK));
                attroff(A_BOLD);
            }
        }

        //clear elevator drawing
        rectangle_fill(2, BEG_X + 2, 2 * floor_count - 1, 4 * elevator_count - 3, ' ');
        
        //write step
        mvprintw(13, 6, "                      ");
        mvprintw(13, 6, "%zu", curr_step);

        for (size_t i = 0; i < elevator_count; i++) {
            elevator *el = AVS_VECTOR_AT(el_system.elevators, i);
            int x = BEG_X + 2 + 4 * i;
            int y = 2 * (floor_count - el->last_floor);
            if (el->state == EL_BETWEEN_FLOORS) {
                y += el->direction == DIR_UP ? -1 : 1;
            }
            if (el->state == EL_STATIONARY) {
                attron(COLOR_PAIR(GREEN_ON_BLACK));
            }
            mvaddch(y, x, '#');
            if (el->state == EL_STATIONARY) {
                attroff(COLOR_PAIR(GREEN_ON_BLACK));
            }
        }

        refresh();
        
        //handle input
        int chr = getch();
        switch (chr) {
            case KEY_UP: {
                if (curr_floor < floor_count - 1) {
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
                if (curr_elevator < elevator_count - 1) {
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
                curr_step++;
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
                    curr_step++;
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
    elevator_system_free(&el_system);
    endwin();
}
