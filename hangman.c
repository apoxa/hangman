#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>
/* some functions for the title and the gibbet */
#include "functions.h"
#define CORD getyx(stdscr, cury, curx)
#define CLOSE endwin(); return 0
/* TODO should be checked somewhere else, dynamically */
#define BACKSPACE 263

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW * local_win);

int main(void)
{
    char txt_3[] = "Please enter a text with at most 80 characters: ";
    char repeat;
    int i = 0;
    /* initialize the curses-screen */
    initscr();
    keypad(stdscr, TRUE);
    /* turn on colors (should be checked first) */
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    char word[80];              /* word to guess */
    char wrongletters[26];      /* letters already guessed wrong */
    char input;                 /* input-letter from the user */
    int letter;                 /* Read single letters for the word to guess */
    int length, tries, errors;  /* some buffers */
    int cury, curx;             /* cursor coordinates */

    do {
        word[0] = '\0';
        wrongletters[0] = '\0';
        letter = -1;
        length = 0;
        tries = 0;
        errors = 0;
        cury = 0;
        curx = 0;
        move(cury, curx);
        clear();
        print_title();
        CORD;
        mvprintw(cury + 2, (COLS - 49) / 2, txt_3);
        CORD;
        move(cury + 2, (COLS - 49) / 2);
        cbreak();               /* get input, don't wait for \n */
        noecho();               /* don't echo user-input */

        do {
            /* read user-input */
            letter = getch();
            if ((letter != BACKSPACE) && ((letter < 65) || (letter > 122))
                && (letter != 32)) {
            } else {
                switch (letter) {
                case BACKSPACE:
                    if (length > 0)
                        length--;
                    CORD;
                    if (curx > (COLS - 49) / 2)
                        mvdelch(cury, curx - 1);
                    break;
                case '\n':
                    word[length] = '\0';
                    break;
                default:
                    word[length] = letter;
                    length++;
                    CORD;
                    mvprintw(cury, curx, "%c", (((letter >= 65)
                                                 && (letter <=
                                                     122)) ? '*' :
                                                letter));
                }
            }
        } while (letter != '\n' && strlen(word) < 81);

        if (strlen(word) == 80) {
            mvprintw(cury + 2, (COLS - 40) / 2,
                     "You've entered the maximum number of letters.");
            getch();
        }
        /* count different letters in the word */
        char letters[length];
        int letter_index = -1, x = 0, y;
        bool found;
        /* go through the whole array */
        for (; word[x] != 0; x++) {
            found = false;
            /* go through every single letter */
            for (y = 0; y <= letter_index; y++) {
                if (letters[letter_index] == word[x])
                    found = true;       /* if letter already found set  
                                           found to true */
            }
            if (!found)
                letters[++letter_index] == word[x];     /* if letter appears 
                                                           for first time, count it */
        }

        char guessings[length];
        for (i = 0; i < length; i++) {
            guessings[i] = (word[i] >= 65
                            && word[i] <= 122) ? '_' : word[i];
        }

        clear();
        echo();                 /* echo the user-input */

        do {
            bool right = false;
            bool twice = false;
            bool already_false = false;
            mvprintw(0, 0, "Word to guess: ");
            for (i = 0; i < length; i++) {
                CORD;
                mvprintw(cury, curx + 1, "%c", guessings[i]);
            }
            mvprintw(cury + 1, 0, "Which letter do you want to guess? ");
            /* read input */
            input = getch();
            CORD;
            mvdelch(cury, curx - 1);

            if (input == '*') {
                CORD;
                print_answer(length, word);
                CORD;
                mvprintw(cury + 2, 0, "The answer is: %s", word);
                goto END;
            }
            for (i = 0; i < length; i++) {
                if (toupper(input) == toupper(guessings[i])) {
                    twice = true;
                }
                if (toupper(input) == toupper(word[i])) {
                    guessings[i] = word[i];
                    right = true;
                }
            }

            if (right != true) {
                for (i = 0; i < 26; i++) {
                    if (toupper(input) == wrongletters[i]) {
                        already_false = true;
                    }
                }
                if (already_false != true) {
                    int wrong_len = strlen(wrongletters);
                    wrongletters[wrong_len] = toupper(input);
                    errors++;
                    mvprintw(4, 0, "Number of errors: %i", errors);
                    mvprintw(5, 0, "Already wrong letters: ");
                    for (i = 0; i <= errors; i++) {
                        CORD;
                        mvprintw(cury, curx + 1, "%c", wrongletters[i]);
                    }
                    draw(errors);
                    move(cury, curx);
                    if (errors >= 8) {
                        move(cury + 1, 0);
                        clrtoeol();
                        attron(COLOR_PAIR(1));
                        mvprintw(cury + 1, 0,
                                 "You lost this round. The word to guess was %s",
                                 word);
                        attroff(COLOR_PAIR(1));
                        goto END;
                    }
                }
            }
            if (twice != true) {
                tries++;
            }
            refresh();
        } while (strncmp(word, guessings, length));

        /* Print all letters instead of dots */
        print_answer(length, word);

        /* clear to bottom, to remove the gibbet */
        clrtobot();
        attron(COLOR_PAIR(2));
        mvprintw(cury + 2, 0, "The word to guess was: ");
        attron(A_BOLD);
        mvprintw(cury + 2, 27, "%s", word);
        attroff(A_BOLD);
        mvprintw(cury + 3, 0, "You needed %i tries.", tries);
        attroff(COLOR_PAIR(2));
      END:
        refresh();
        /* Window with the question to repeat the game */
        WINDOW *end_win;
        int height = 3;
        int width = 48;
        int starty = (LINES - height) / 2;      /* place the box in the middle of the screen */
        int startx = (COLS - width) / 2;
        end_win = create_newwin(height, width, starty, startx);
        mvwprintw(end_win, 1, 3,
                  "Do you want to play another round? (Y/N) ");
        wrefresh(end_win);
        nocbreak();
        repeat = wgetch(end_win);
        /* Clear input-buffers of curses, to resolve problems with a newline */
        flushinp();
    } while (toupper(repeat) == 'Y');

    CLOSE;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);       /* 0, 0 are standard-chars 
                                 * for the borders */
    wrefresh(local_win);        /* refresh screen to show the box */

    return local_win;
}


void destroy_win(WINDOW * local_win)
{
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* Parameters are
     * 1. win: Window to print on
     * 2. ls: Character for the left side
     * 3. rs: Character for the right side
     * 4. ts: Character for the top side
     * 5. bs: Character for the bottom side
     * 6. tl: Character for the top left corner
     * 7. tr: Character for the top right border
     * 8. bl: Character for the bottom left corner
     * 9. br: Character for the bottom right corner
     */
    wrefresh(local_win);
    delwin(local_win);
}
