/**
 * ann.h
 * Created by Scott Gauthreaux <me@scottgauthreaux.com>
 * 2016/10/24
 */

#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

typedef long double prob;

class ANN {
public:
    ANN(vector<vector<vector<prob> > >weights, prob alpha);
    ~ANN();
    void train(const vector<prob> &input, const vector<prob> &output);
    void feedForward(const vector<prob> &input); // Feeds the neural net (calculates)
    void backPropagate(const vector<prob> &expectedOutput); // Back propagates the errors
    vector<prob> output(); // Returns the current values of the output layer (excluding possible bias node)
    int classify(const vector<prob>& input); // Classifies the output using euclidean distance
    void commit(); // Adjusts weights and clears errors
    void print(const vector<vector<prob> > &matrix);
    void printAllWeights(ostream& stream);
    void print(const vector<prob> &vector);
    void printFirstWeights();
    vector<vector<prob> >encodings;
private:
    // Member variables
    prob alpha; // The learning constant
    vector<vector<vector<prob> > >weights; // Array of matrices with weight values
    vector<vector<prob> >layers; // Array of arrays with node values
    vector<vector<prob> >errors; // The current calculated errors associated with the nodes
    
    // Utility Functions
    vector<prob> omitDummy(const vector<prob> &input);
    prob sigmoid(prob); // The sigmoid activation function (a.k.a. logistic)
    void sigmoid(vector<prob> &input);
    prob sigmoidPrime(prob); // The derivative of the sigmoid function
    vector<prob> sigmoidPrime(const vector<prob> &input);

    //Linear algebra
    //1D
    prob euclideanDist(const vector<prob> &left, const vector<prob> &right); // Computes uclidean distance between 2 vectors
    void add(vector<vector<prob> > &left, const vector<vector<prob> > &right); // Adds vector to other vector (in-place, mutating)
    vector<prob> multiply(const vector<prob> &left, const vector<prob> &right); // Mulitplies vectors (immutable)
    void multiply(vector<prob> &left, prob factor); // Multiplies vector by factor (in-place, mutating)
    vector<prob> error(const vector<prob> &output, const vector<prob> &expectedOutput); // Computes error between layer and output
    
    //2D
    vector<prob>dot(const vector<vector<prob> >&matrix, const vector<prob>&vector); // Computes the dot product between a matrix and vector
    vector<prob>dotTranspose(const vector<vector<prob> > &matrix, const vector<prob> &vector); // Computes the dot product between a matrix\s transpose and a vector
    void outerProductAddMultiply(vector<vector<prob> >&output, const vector<prob>&left, const vector<prob> &right, prob alpha); // Adds the outer product of 2 vectors to a matrix and multiplies by factor (in-place, mutating)
};

/*

Let I be a vector of input layer values
Let H be a vector of hidden layer input values
Let T be a matrix representing the transition function between the input layer and the hidden layer

I = [0,1,2]

T = [03,04]
    [13,14]
    [23,24]

Ttranspose = [03,13,23]
             [04,14,24]

H = Ttranspose * I = [0*03 + 1*13 + 2*23]
                     [0*04 + 1*14 + 2*24]

While this won't improve runtime complexity with a naive and sequential linear-algebra based implementation of an Artificial Neural Network, it lays the ground work for hardware acceleration via GPU or leveraging advanced instruction sets such as SIMD (Single Instruction, Multiple Data) or the BLAS (Basic Linear Algebra Subprograms) library

I have drastically improved my ANN's performance by switching from a value-type functional programming style to a procedural style.
I have also switched to heavy reference-type usage and added mutating, in-place operations on variables that could be altered.
The execution time on test 2 went from 12s to 1s after all optimizations.

Next step is to leverage a linear algebra library that takes advantage of the architecture and parallellism. It will make all matrix operations O(2) instead of O(n^2) 

 */
