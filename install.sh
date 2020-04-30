#!/bin/bash

#Copy program files
mkdir $HOME/.recall
cp recall $HOME/.recall/recall
chmod +x $HOME/.recall/recall
cp recall.png $HOME/.recall/recall.png

#Create RecallList folder in Documents
cp -r RecallLists  $HOME/Documents

#Create desktop launcher on desktop
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

#Add recall to $PATH
echo 'export PATH=$PATH:~/.recall' >> $HOME/.bashrc
source $HOME/.bashrc
