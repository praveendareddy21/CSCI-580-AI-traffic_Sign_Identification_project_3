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
#include "ann.h"

#define MIN_ARGS 7
#define ALPHA 0.01

using namespace std;

vector<vector<prob> > matrixFromInput(istream *input, int rows, int cols) {
    vector<vector<prob> >matrix;
    prob buff;
    for (int i = 0; i < rows; i++) {
        vector<prob>row;
        for (int j = 0; j < cols; j++) {
            *input >> buff;
            row.push_back(buff);
        }
        matrix.push_back(row);
    }
    return matrix;
}

int main(int argc, char const *argv[]) {
    // User input verification
    if (argc < MIN_ARGS) {
        printf("Usage: ann <train_input> <train_output> <test_input> <test_output> <structure> <k>\n");
        return 1;
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
        weights.push_back(vector<vector<prob> >(layerSizes[i], vector<prob>(layerSizes[i+1], 0.5)));
    }

    // Created the ANN with the given weights (automatically knows structure based on weights)
    ANN *ann = new ANN(weights, ALPHA);
    
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
    for (int i = 0; i < atoi(argv[7]); i++) {
        for (unsigned long j = 0; j < trainInput.size(); j++) {
            ann->train(trainInput[j], ann->encodings[trainOutput[j]]);
        }
    }

    // Test
    
    // Read test output
    ifstream test_output_file;
    test_output_file.open(argv[4]);
    vector<int>testOutput;
    int intbuff;
    while(test_output_file >> intbuff) {
        testOutput.push_back(intbuff);
    }
    
    // Read test input
    ifstream test_input_file;
    test_input_file.open(argv[3]);
    vector<vector<prob> >testInput = matrixFromInput(&test_input_file, (int)testOutput.size(), layerSizes[0]);
    
    ann->printFirstWeights();
    
    prob corrects = 0;
    for (unsigned long i = 0; i < testInput.size(); i++) {
        int digit = ann->classify(testInput[i]);
        cout << digit << endl;
        corrects += digit == testOutput[i];
    }
    cout << (prob)corrects/(prob)testOutput.size() << endl;
    
    return 0;
}
