from PIL import Image
from os import scandir

def is_png(file):
    return ".png" in file

def get_pngs(dir):
    pngs = []
    for file in scandir(dir):
        if is_png(file.name):
            pngs.append(file)
    return pngs

directories = ['crosswalk','noparking','railroad','speed', 'stop','yield']
# directory = 'crosswalk-32-bw'#input()
# directories = ['railroad-32-bw']

for directory in directories:
    pngs = get_pngs(directory)
    outfile = open(directory + '/all.bin', 'w')
    for i in range(len(pngs)):
        im = Image.open(directory + '/' + pngs[i].name)
        pixels = list(im.getdata())
        width, height = im.size
        pixels = [pixels[i * width:(i + 1) * width] for i in range(height)]
        for row in pixels:
            for col in row:
                print(col, sep=' ', end=' ', file=outfile)
        if i < len(pngs) - 1:
            print('', file=outfile)

