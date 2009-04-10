int draw( int fehler ) {
    int x, y;
    getyx(stdscr, y, x);
    y += 2;
    attron(A_BOLD);
    switch(fehler) {
        case 8:
            attroff(A_BOLD);
            attron(COLOR_PAIR(1));
            mvprintw(y+4,6, "/ \\");
        case 7:
            attroff(A_BOLD);
            mvprintw(y+3,7, "l");
        case 6:
            attroff(A_BOLD);
            mvprintw(y+2,6, "\\o/");
        case 5:
            attron(A_BOLD);
            attroff(COLOR_PAIR(1));
            mvprintw(y,4, "____");
            mvprintw(y+1,7, "l");
        case 4:
            mvprintw(y,2, "__");
            mvprintw(y+1,2, "l/");
        case 3:
            mvprintw(y+2,2, "l");
        case 2:
            mvprintw(y+3,2, "l");
            mvprintw(y+4,2, "l");
        case 1: 
            mvprintw(y+5,2, "l__");
            mvprintw(y+6,1, "/  \\");
            attroff(A_BOLD);
            break;
    }
    
    return 0;
}

int print_answer( int laenge, char ratewort[]) {
    int cury = 0, curx = 18;
    int i;
    move(cury,curx);
    clrtoeol();
    for( i = 0; i < laenge; i++) {
        mvprintw(cury, curx+1,"%c", ratewort[i]);
        getyx(stdscr, cury, curx);
    }
    return 0;
}
