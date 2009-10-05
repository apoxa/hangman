const char *title[] = {
    "HH   HH   AAA   NN   NN   GGGG",
    "HH   HH  AAAAA  NNN  NN  GG  GG",
    "HHHHHHH AA   AA NN N NN GG",
    "HH   HH AAAAAAA NN  NNN GG   GG",
    "HH   HH AA   AA NN   NN  GGGGGG",
    "",
    "    MM    MM   AAA   NN   NN", 
    "    MMM  MMM  AAAAA  NNN  NN", 
    "    MM MM MM AA   AA NN N NN", 
    "    MM    MM AAAAAAA NN  NNN", 
    "    MM    MM AA   AA NN   NN" 
};

/* Prints the title screen */
void print_title(void) {
  int title_lines = sizeof(title) / sizeof(const char *); 

  int top = (LINES-title_lines)/3.0 + 0.5;
  int i;

  if( top < 5 ) top = 5;
  for( i = 0; i < title_lines; ++i ) {
      mvaddstr(top+i, (COLS-40)/2, title[i]);
  }
}

int draw( int errors ) {
    int x, y;
    getyx(stdscr, y, x);
    y += 2;
    attron(A_BOLD);
    switch(errors) {
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

int print_answer( int length, char word[]) {
    int cury = 0, curx = 16;
    int i;
    move(cury,curx);
    clrtoeol();
    for( i = 0; i < length; i++) {
        mvprintw(cury, curx+1,"%c", word[i]);
        getyx(stdscr, cury, curx);
    }
    return 0;
}
