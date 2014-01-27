#!/bin/bash
for FILE1 in "$@"
do
	file_name=$(basename ${FILE1%.*})
	sox --norm sounds_src/$file_name.wav sounds_src/_$file_name.wav 
	sox sounds_src/_$file_name.wav -n spectrogram -o images_src/$file_name.png -l -m -r -x 640 -y 480
	convert images_src/$file_name.png images/$file_name.pgm
done
