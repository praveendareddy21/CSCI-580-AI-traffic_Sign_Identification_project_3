catIm.rotate(6).save('rotated6.png')

from __future__ import print_function
import os, sys
from PIL import Image


path = '/home/red/input_directory'


for filename in os.listdir(path):
    
    try:
    	im = Image.open(filename)
    	#rotate by 6 degrees
    	im.rotate(6).save("rotate6_"+filename)
    except IOError:
    	print("cannot open or save file")
