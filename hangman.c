#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <ncurses.h>
#include "functions.h"       // eigene Header-Datei mit der Funktion fuer den Galgen
#define CORD getyx(stdscr, cury, curx)
#define CLOSE endwin(); return 0
#define BACKSPACE 127

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(void) {
    char txt_3[] = "Bitte gib einen Text mit maximal 80 Zeichen ein: ";
    char wiederholen;
    int i = 0;
    initscr();      // curses initialisieren
    keypad(stdscr,TRUE);
    start_color();  // farben einschalten
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
    do {
        char ratewort[80];      // Zu erratendes Wort
        char falsche[26] = { '\0' };       // Bereits falsch geratene Buchstaben
        char eingabe;           // Rate-Eingabe des Spielers
        int worteingabe = -1;   // Einge des zu erratenden Wortes
        int laenge = 0, versuche = 0, fehler = 0;       // verschiedene Zwischenspeicher
        int cury = 0, curx = 0;         // cursor-koordinaten
        move(cury,curx); clear();
        mvprintw(0,0,"*** HANGMAN ***");
        mvprintw(1,0,"***************");
        mvprintw(2,0,"%s", txt_3);
        cbreak();       // input sofort nehmen, nicht auf \n warten
        noecho();       // Eingaben nicht anzeigen
    
        do {
            worteingabe = getch();          // eingabe lesen
            if ( (worteingabe != BACKSPACE) && ((worteingabe < 65) || (worteingabe > 122)) && (worteingabe != 32) ) {
            }
            else {
                switch(worteingabe) {
                    case BACKSPACE:
                        if ( laenge > 0 ) laenge--;
                        CORD;
                        if ( curx > strlen(txt_3) ) mvdelch(2,curx-1);
                        break;
                    case '\n':
                        ratewort[laenge] = '\0';
                        break;
                    default:
                        ratewort[laenge] = worteingabe;
                        laenge++;
                        CORD;
                        mvprintw(2,curx,  "%c", ( ((worteingabe >= 65) && (worteingabe <= 122)) ? '*' : worteingabe ));
                }
            }
        } while ( worteingabe != '\n' && strlen(ratewort) < 81);

        if ( strlen(ratewort) == 80 ) {
                mvprintw(3,0, "Sie haben die maximale Anzahl von Zeichen eingebgeben.");
                getch();
        }
    
        // verschiedene Buchstaben zaehlen um eine Bewertung nach Versuchen zu machen
        char buchstaben[laenge];
        int buchstaben_index = -1, x = 0, y;
        bool gefunden;
        for (; ratewort[x] != 0; x++) {         // jeden Buchstaben durchgehen
            gefunden = false;
            for ( y = 0; y <= buchstaben_index; y++) {  // alle gefundenen buchstaben durchgehen
                if ( buchstaben[buchstaben_index] == ratewort[x] ) gefunden = true;     // wenn er schon vorkommt
            }                                                                           // gefunden auf true setzen
            if ( !gefunden ) buchstaben[++buchstaben_index] == ratewort[x];             // wenn er nicht gefunden wurde
        }                                                                               // Zaehler hochsetzen
    
        char geraten[laenge];
        for ( i = 0; i < laenge; i++ ) {
            geraten[i] = (ratewort[i] >= 65 && ratewort[i] <= 122) ? '_' : ratewort[i];
        }
    
        clear();
        echo();         // Tastatureingaben wieder sichtbar
    
        do {
            bool richtig = false;
            bool doppelt = false;
            bool bereitsfalsch = false;
            mvprintw(0,0,"Zu ratender Text: ");
            for( i = 0; i < laenge; i++) {
                CORD;
                mvprintw(cury, curx+1,"%c", geraten[i]);
            }
            mvprintw(cury+1, 0,"Welchen Buchstaben wollen Sie raten? ");
            eingabe = getch();              // Eingabe lesen
            CORD;
            mvdelch(cury,curx-1);
        
            if ( eingabe == '*' ) {
                CORD;
                print_answer(laenge, ratewort);
                CORD;
                mvprintw(cury+2, 0, "Die Lösung ist: %s", ratewort);
                goto ENDE;
            }
            for( i = 0; i < laenge; i++) {
                if( toupper(eingabe) == toupper(geraten[i]) ) {
                    doppelt = true;
                }
                if( toupper(eingabe) == toupper(ratewort[i]) ) {
                    geraten[i] = ratewort[i];
                    richtig = true;
                }
            }

            if ( richtig != true ) {
                for( i = 0; i < 26; i++) {
                    if( toupper(eingabe) == falsche[i] ) {
                        bereitsfalsch = true;
                    }
                }
                if ( bereitsfalsch != true ) {
                    int falschlen = strlen(falsche);
                    falsche[falschlen] = toupper(eingabe);
                    fehler++;
                    mvprintw(4,0, "Anzahl der Fehlversuche: %i", fehler);
                    mvprintw(5,0, "Bereits gewaehlte Buchstaben: ");
                    for( i = 0; i <= fehler; i++ ) {
                        CORD;
                        mvprintw(cury, curx+1, "%c", falsche[i]);
                    }
                    draw(fehler);
                    move(cury,curx);
                    if ( fehler >= 8 ) {
                        move(cury+1,0);
                        clrtoeol();
                        attron(COLOR_PAIR(1));
                        mvprintw(cury+1,0, "Sie haben dieses Spiel verloren! Das zu erratende Wort war %s", ratewort);
                        attroff(COLOR_PAIR(1));
                        goto ENDE;
                    }
                }
            }
            if( doppelt != true ) { 
                versuche++; 
            }
            refresh();
        } while ( strncmp( ratewort, geraten, laenge) );
    
        // Alle . durch den Buchstaben ersetzen
        cury = 0; curx = 18;
        move(cury,curx);
        clrtoeol();
        for( i = 0; i < laenge; i++) {
            mvprintw(0, curx+1,"%c", ratewort[i]);
            CORD;
        }
    
        clrtobot();     // alles bis zum Bildschirmende entfernen (Galgen)
        attron(COLOR_PAIR(2));
        mvprintw(cury+2, 0, "Das zu erratende Wort war: ");
        attron(A_BOLD);
        mvprintw(cury+2, 27, "%s", ratewort);
        attroff(A_BOLD);
        //mvprintw(cury+3, 0,"Sie haben %i Versuche gebraucht, Sie %s.\n", versuche, (versuche > ( buchstaben_index + 2 ) ? "Trottel" : "Genie"));
        mvprintw(cury+3, 0, "Du hast %i Versuche gebraucht.", versuche);
        attroff(COLOR_PAIR(2));
        ENDE:
        refresh();
        WINDOW *end_win;        // Fenster mit Frage nach neuer Runde
        int height = 3;
        int width = 48;
        int starty = ( LINES - height ) / 2;        // Box in der Mitte des Terminals anzeigen
        int startx = ( COLS - width ) / 2;
        end_win = create_newwin(height, width, starty, startx);
        mvwprintw(end_win,1,3, "Wollen Sie noch eine Runde spielen? (J/N) ");
        wrefresh(end_win);
        nocbreak();
        wiederholen = wgetch(end_win);
        flushinp();     // Eingabepuffer von curses loeschen, sonst probleme wegen NewLine
    } while ( toupper(wiederholen) == 'J' );
    
    CLOSE;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *local_win;
    
	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 sind die Standardzeichen 
                                 * fuer die Linien am Rand */
	wrefresh(local_win);		/* Box auch wirklich anzeigen */
    
	return local_win;
}


void destroy_win(WINDOW *local_win) {	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* Die uebergeben Parameter sind 
	 * 1. win: Das Fenster auf das es angewendet wird
	 * 2. ls: Character fuer die linke Seite des Fensters
	 * 3. rs: Character fuer die rechte Seite des Fensters 
	 * 4. ts: Character fuer die Oberseite des Fensters 
	 * 5. bs: Character fuer die Unterseite des Fensters 
	 * 6. tl: Character fuer die linke obere Ecke
	 * 7. tr: Character fuer die rechte obere Ecke 
	 * 8. bl: Character fuer die linke untere Ecke 
	 * 9. br: Character fuer die rechte untere Ecke
	 */
	wrefresh(local_win);
	delwin(local_win);
}
