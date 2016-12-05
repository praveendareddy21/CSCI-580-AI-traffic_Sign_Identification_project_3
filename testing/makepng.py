from PIL import Image
import numpy as np
import sys

if len(sys.argv) != 2:
    print('usage: <file to parse>')
    exit()


#open the user specified file
with open(sys.argv[1]) as f:
    fnum = 0;
    #for each line in the file
    for line in f:
        values = line.split()
        dim = int(len(values) ** .5)
        print(len(values))
        #print(dim)
        arr = np.zeros((dim,dim), dtype=np.uint8)
        i = 0
        j = 0

        counter = 0
        #for each word in the line
        for value in values:
            arr[j, i] = value
            counter+=1
            #print(i,j, '=', value, counter)
            i+= 1
            if(i % 32 == 0):
                j+= 1
                i = 0

        fname = 'results/result' + str(fnum) + '.png'
        fnum+= 1
        print('saving to', fname)
        img = Image.fromarray(arr)
        img.save(fname)
