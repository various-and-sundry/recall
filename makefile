recall: recall.c
	gcc recall.c -o recall -lncurses

installer: recall manual
	mkdir recall_installer
	cp -r recall install.sh uninstall.sh graphics/recall.png docs/manual.pdf recall_installer
	zip -r recall_installer.zip recall_installer

manual: docs/manual.tex
	cd docs && make

updatelists: RecallLists
	cp -r RecallLists ~/Documents

clean:
	rm -r recall recall_installer recall_installer.zip

install:
	cp graphics/recall.png recall.png
	bash install.sh
	rm recall.png

uninstall:
	bash uninstall.sh
