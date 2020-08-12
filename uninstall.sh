#!/bin/bash


#Remove ~/.recall from PATH
sed -i -r '/^export /s/.*recall$//g' $HOME/.bashrc

#Remove Program File
rm -r $HOME/.recall

#Remove desktop icon
rm $HOME/Desktop/Recall.desktop

#Remove launcher from /usr/share/applications/Recall.desktop
sudo rm /usr/share/applications/Recall.desktop
