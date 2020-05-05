recall: recall.c
	gcc recall.c -o recall -lncurses

installer: recall install.sh uninstall.sh RecallLists graphics/recall.png
	mkdir recall_installer
	cp -r recall install.sh uninstall.sh graphics/recall.png RecallLists manual/manual.pdf recall_installer 	
	zip -r recall_installer.zip recall_installer

updatelists: RecallLists
	cp -r RecallLists  ~/Documents

clean:
	rm recall
	rm -r recall_installer recall_installer.zip

install:
	cp graphics/recall.png recall.png
	bash install.sh
	rm recall.png

uninstall:
	bash uninstall.sh
