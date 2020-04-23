recall: recall.c
	gcc recall.c -o recall -lncurses

clean:
	rm recall

install:
	./install.sh

uninstall:
	./uninstall.sh
