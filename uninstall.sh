#!/bin/bash


#Remove ~/.recall from PATH
#sed -i -r 's#export PATH=#PATH:~/.recall##g' '/home/pi/.bashrc'
sed -i -r '/^export /s/.*recall$//g' $HOME/.bashrc

#Remove Recalists from ~/Docments
rm -r $HOME/Documents/RecallLists

#Remove Program File
rm -r $HOME/.recall

#Remove desktop icon
rm $HOME/Desktop/Recall.desktop

#Remove launcher from /usr/share/applications/Recall.desktop
sudo rm /usr/share/applications/Recall.desktop
