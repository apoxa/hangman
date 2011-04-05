ICLPATH = /usr/include

hangman: hangman.o
	cc -lncurses -o hangman hangman.o  

hangman.o: hangman.c $(ICLPATH)/stdio.h $(ICLPATH)/string.h $(ICLPATH)/ctype.h $(ICLPATH)/ncurses.h functions.h
	cc -c hangman.c
clean:
	rm -f hangman hangman.o
