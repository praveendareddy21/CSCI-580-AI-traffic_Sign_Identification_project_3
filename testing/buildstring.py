with open('newfile.txt', 'a') as out:
    for i in range(1024):
        out.write(str(i % 255))
        out.write(' ')
