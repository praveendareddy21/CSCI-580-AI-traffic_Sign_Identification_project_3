from __future__ import print_function
import os, sys
from PIL import Image


path = '/home/red/input_directory'


for filename in os.listdir(path):
    
    try:
    	im = Image.open(filename)
    	# Blur the image
    	blurred = im.filter(ImageFilter.BLUR)
    	blurred.save("blur_"+filename)
    except IOError:
    	print("cannot open or save file")
