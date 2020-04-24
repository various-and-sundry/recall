recall: recall.c
	gcc recall.c -o recall -lncurses

clean:
	rm recall

install:
	bash install.sh

uninstall:
	bash uninstall.sh
