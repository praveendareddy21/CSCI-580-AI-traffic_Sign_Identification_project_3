import sys

# This needs to round robin over a list of input files and it needs to track which is which

# administrative work here...
kinds = ['crosswalk','noparking','railroad','speed','stop','yield']
kinds_count = len(kinds)

data_files = []

train_input = open("train-input-5", 'w')
train_output = open("train-output-5", 'w')
test_input = open("test-input-5", 'w')
test_output = open("test-output-5", 'w')

for kind in kinds:
    data_files.append(open(kind + '/all.bin'))



i = 0
while True:
    next = False
    for j in range(kinds_count):
        line = data_files[j].readline()
        if line:
            if i % 10 < 8: 
                print(line, file=train_input, end='')
                print(str(j), file=train_output)
            else:
                print(line, file=test_input, end='')
                print(str(j), file=test_output)
            next = True
            i += 1
    if not next:
        break


"""
# a poor man's git

    train_files.append(open(kind + train_suffix))
    test_files.append(open(kind + test_suffix))


i = 0
while True:
    next_train = False
    next_test = False

    for j in range(kinds_count):
        train_line = train_files[j].readline()
        if train_line:
            print(train_line, file=train_input)
            print(str(j), file=train_output)
            next_train = True

    for j in range(kinds_count):
        test_line = test_files[j].readline()
        if test_line:
            print(test_line, file=test_input)
            print(str(j), file=test_output)
            next_test = True
    if not next_train and not next_test:
        break
"""
