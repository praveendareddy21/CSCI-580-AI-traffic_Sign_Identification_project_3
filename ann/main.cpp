/**
 * main.cpp
 * Created by Scott Gauthreaux <me@scottgauthreaux.com>
 * 2016/10/24
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <ostream>
#include <stdlib.h>     /* atoi */
#include <algorithm>
#include "ann.h"

#define MIN_ARGS 9

#define PRINT_ACCU true

using namespace std;

bool print_cmp = false;

vector<vector<prob> > matrixFromInput(istream *input, int rows, int cols) {
    vector<vector<prob> >matrix;
    prob buff;
    for (int i = 0; i < rows; i++) {
        vector<prob>row;
        for (int j = 0; j < cols; j++) {
            *input >> buff;
            row.push_back(buff/256.0);
        }
        matrix.push_back(row);
    }
    return matrix;
}

prob accuracy(ANN* ann, const char* input_file_name, int input_layer_size, const char* output_file_name) {
    // Read test output
    ifstream test_output_file;
    test_output_file.open(output_file_name);
    vector<int>testOutput;
    int intbuff;
    while(test_output_file >> intbuff) {
        testOutput.push_back(intbuff);
    }
    int output_layer_size = (int)testOutput.size();
    // Read test input
    ifstream test_input_file;
    test_input_file.open(input_file_name);
    vector<vector<prob> >testInput = matrixFromInput(&test_input_file, output_layer_size, input_layer_size);


    // Calculate accuracy
    vector<int>output_errors = vector<int>(output_layer_size, 0);
    vector<int>output_correct = vector<int>(output_layer_size, 0);
    vector<int>classifications;
    prob corrects = 0;
    if (print_cmp) {
        cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

    }
    for (unsigned long i = 0; i < testInput.size(); i++) {
        int digit = ann->classify(testInput[i]);
        if (print_cmp) {
            cout << testOutput[i] << " ";
            classifications.push_back(digit);
        }

        int correct = digit == testOutput[i];
        if (correct) {
            output_correct[testOutput[i]]++;
        } else {
            output_errors[testOutput[i]]++;
        }
        corrects += correct;
    }
    if (print_cmp) {
        cout << endl;
            for (unsigned long i = 0; i < classifications.size(); i++) {
            cout << classifications[i] << " ";
        }
        cout << endl;
    }



/*
    cout << "Classes\t";
    for (unsigned long i; i < output_layer_size; i++) {
        cout << i << "\t";
    }
    cout << endl;

    cout << "Correct\t";
    for (unsigned long i; i < output_layer_size; i++) {
        cout << output_correct[i] << "\t";
    }
    cout << endl;

    cout << "Errors\t";
    for (unsigned long i; i < output_layer_size; i++) {
        cout << output_errors[i] << "\t";
    }
    cout << endl;
*/
    return (prob)corrects/(prob)testOutput.size();
}

int main(int argc, char const *argv[]) {
    // User input verification
    if (argc < MIN_ARGS) {
        printf("Usage: ann <train_input> <train_output> <test_input> <test_output> <structure> <iterations> <alpha> <output_file> [-comp]\n");
        return 1;
    }

    if (argc > MIN_ARGS) {
        if (!strcmp("-comp", argv[MIN_ARGS])) {
            print_cmp = true;
        }
    }

    ifstream structure;
    structure.open(argv[5]);

    ifstream weight_file;
    weight_file.open(argv[6]);
    vector<int>layerSizes;
    int layerSize;
    // Read the structure in order to be able to read the weights
    while(structure >> layerSize) {
        vector<prob> layer;
        layerSizes.push_back(layerSize);
    }

    // Read the weights
    vector<vector<vector<prob> > >weights;
    for (unsigned long i = 0; i < layerSizes.size() - 1; i++) {
        //weights.push_back(matrixFromInput(&weight_file, layerSizes[i], layerSizes[i+1]));
        weights.push_back(vector<vector<prob> >(layerSizes[i], vector<prob>(layerSizes[i+1], .5/layerSizes[i+1])));
    }

    // Created the ANN with the given weights (automatically knows structure based on weights)
    ANN *ann = new ANN(weights, atof(argv[7]));

    // Read train output
    ifstream train_output_file;
    train_output_file.open(argv[2]);
    vector<prob>trainOutput;
    prob buff;
    while(train_output_file >> buff) {
        trainOutput.push_back(buff);
    }

    // Read train input
    ifstream train_input_file;
    train_input_file.open(argv[1]);
    vector<vector<prob> >trainInput = matrixFromInput(&train_input_file, (int)trainOutput.size(), layerSizes[0]);


    // Train ANN with training data k times
    vector<int>lookup = vector<int>(trainInput.size(), 0);
    for (unsigned long i = 0; i < trainInput.size(); i++) {
        lookup[i] = i;
    }

    srand ( unsigned ( time(0) ) );

    int max = atoi(argv[6]);
    int i;
    for (i = 0; i < max; i++) {
        random_shuffle(lookup.begin(), lookup.end());

        if (PRINT_ACCU && !print_cmp) {
            cout << "                                   \r" << i << "\t" << accuracy(ann, argv[3], layerSizes[0], argv[4]) << endl;
        } else {
            accuracy(ann, argv[3], layerSizes[0], argv[4]);
        }

        fflush(stdout);

        for (unsigned long j = 0; j < trainInput.size(); j++) {
            ann->train(trainInput[lookup[j]], ann->encodings[trainOutput[lookup[j]]]);
        }
    }

    // Test
    if(PRINT_ACCU) {
        cout << i << "   \t" << accuracy(ann, argv[3], layerSizes[0], argv[4]) << endl;
    }


    ofstream outputfile;
    outputfile.open(argv[8]);
    ann->printAll(outputfile);

    return 0;
}
