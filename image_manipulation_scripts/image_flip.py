from __future__ import print_function
import os, sys
from PIL import Image


path = '/home/red/input_directory'


for filename in os.listdir(path):
    
    try:
    	im = Image.open(filename)
    	# flip and save
    	im.transpose(Image.FLIP_LEFT_RIGHT).save("flip_"+filename)
    except IOError:
    	print("cannot open or save file")

