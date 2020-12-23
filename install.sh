#!/bin/bash

#Copy program files
mkdir $HOME/.recall
cp recall $HOME/.recall/recall
chmod +x $HOME/.recall/recall
cp recall.png $HOME/.recall/recall.png

#Decide where to put the RecallList directory
if [ -d ~/Documents ]
then
	touch ~/.recall/recallrc
	cat > ~/.recall/recallrc << EOF
#The next line tells Recall where the Recall lists are stored.
PATH ~/Documents/RecallLists/
EOF
	mkdir ~/Documents/RecallLists 
	if [ -d ./RecallLists ]
	then
		cp -r ./RecallLists ~/Documents
	fi
else
	touch ~/.recall/recallrc
	cat > ~/.recall/recallrc << EOF
#The next line tells Recall where the Recall lists are stored.
PATH~/.recall/RecallLists/
EOF
	mkdir ~/.recall/RecallLists
	if [ -d ./RecallLists ]
	then
		cp -r ./RecallLists ~/.recall
	fi

fi

#Create desktop launcher on desktop
if [ -d ~/Desktop ]
then
touch $HOME/Desktop/Recall.desktop
cat > $HOME/Desktop/Recall.desktop << EOF
[Desktop Entry]

Type=Application
Name=Recall
Comment=A virtual flashcard tool.
Exec=$HOME/.recall/recall
Terminal=true
Icon=$HOME/.recall/recall.png
Categories=Education
EOF
chmod +x $HOME/Desktop/Recall.desktop

#Add copy of desktop launcher to applications menu
sudo cp $HOME/Desktop/Recall.desktop /usr/share/applications/Recall.desktop
fi

#Add recall to $PATH
echo 'export PATH=$PATH:~/.recall' >> $HOME/.bashrc
source $HOME/.bashrc
