/**
 * ann.cpp
 * Created by Scott Gauthreaux <me@scottgauthreaux.com>
 * 2016/10/24
 */

#include "ann.h"
#include <assert.h>     /* assert */

#define DEFAULT_BIAS 0.01

//TODO: rebuild using reference semantics and pointers because value types are too slow

/**
 * The ANN constructor
 *
 * This constructor only takes an array of weight matrices since the
 * dimensions of the weight matrices correspond to the size of the layers
 *
 * @param  weights an array of weight matrices between the layers
 * @param  alpha the learning constant
 * @return the newly created neural network
 */
ANN::ANN(vector<vector<vector<prob> > >weights, prob alpha) {
    // Create allocate space for our vectors for peformance
    this->weights.reserve(weights.size());
    this->layers.reserve(weights.size() + 1);
    this->errors.reserve(weights.size() + 1);
    // Copy over the weight matrices, adding bias weights
    for (unsigned long i = 0; i < weights.size(); i++) {
        unsigned long rows = weights[i].size();
        unsigned long cols = weights[i][0].size();
        // This means add a row of bias weights to each weight matrix
        vector<vector<prob> > layer;
        layer.reserve(rows + 1);
        // For each layer of weights, the bias row must be of the same width as the
        vector<prob>row;
        row.reserve(cols);
        for (unsigned long j = 0; j < cols; j++) {
            row.push_back(DEFAULT_BIAS);
        }
        layer.push_back(row);
        layer.insert(layer.begin()+1, weights[i].begin(), weights[i].end());
        this->weights.push_back(layer);
    }

    // Create layers
    // First layer
    vector<prob>layer = vector<prob>(weights[0].size() + 1);
    // Set bias node to 1
    layer[0] = 1;
    this->layers.push_back(layer);
    // Create error nodes
    errors.push_back(vector<prob>(weights[0].size()));

    for (unsigned long i = 0; i < weights.size(); i++) {
        // Need to add bias to each layer so set them to be 1 larger than input info
        vector<prob>layer(weights[i][0].size() + 1);
        // Set bias node to 1
        layer[0] = 1;
        this->layers.push_back(layer);
        // Create error nodes
        errors.push_back(vector<prob>(weights[i][0].size()));
    }

    // Let's set the encodings for the digits (the ANN is set to recognize as many digits as there are output nodes)
    unsigned long outputSize = this->layers[this->layers.size() - 1].size() - 1;
    encodings = vector<vector<prob> >(outputSize, vector<prob>(outputSize, 0.1));
    for (unsigned long i = 0; i < outputSize; i++) {
        encodings[i][i] = .9;
    }

    // Finally let's set the learning constant
    this->alpha = alpha;

}

/**
 * ANN destructor
 */
ANN::~ANN() {
}


/**
 * The sigmoid activation function
 *
 * This function is used to normalize input to a node inside
 * a neural network to values between 0-1
 * @param  input
 * @return sigmoid(input)
 */
prob ANN::sigmoid(prob input) {
    return (prob)1.0/((prob)1.0+expl(-input));
}

void ANN::sigmoid(vector<prob> &input) {
    for (unsigned long i = 0; i < input.size(); i++) {
        input[i] = sigmoid(input[i]);
    }
}

/**
 * The derivative of the sigmoid activation function
 *
 * This function is used during the back-propagation phase
 * of the training of a neural network to determine the reponsibility
 * of a given node in the error at output. This is done for regression
 * towards the optimal edge weights in the neural network
 *
 * @param  input (the sigmoid value, not the raw value)
 * @return d(sigmoid(input))/dinput
 */
prob ANN::sigmoidPrime(prob input) {
//    prob sig = sigmoid(input);
    prob sigPrime = input * ((prob)1.0-input);
    return (sigPrime ? sigPrime : .00000001);
}


/**
 * The vectorized version of the sigmoid prime function
 *
 * @param  input the input vector
 * @return the vector with sigmoid prime applied to each element
 */
vector<prob> ANN::sigmoidPrime(const vector<prob> &input) {
    vector<prob>result(input.size());
    for (unsigned long i = 0; i < input.size(); i++) {
        result[i] = sigmoidPrime(input[i]);
    }
    return result;
}


/**
 * Performs the feet forward operation on the network
 * This is the essential operation on a neural network, it calculates the output
 * for each layer and each node based on a given input and the current weights of the network
 *
 * @param input
 */
void ANN::feedForward(const vector<prob> &input) {
    // Set input layer values
    // Insert input vector
    //layers[0].insert(layers[0].begin() + 1, input.begin(), input.end());
    for (unsigned long i = 0; i < input.size(); i++) {
        layers[0][i+1] = input[i];
    }
    // Run feedforward
    for (unsigned long i = 0; i < weights.size(); i++) {
        // cblas_dgemv
        unsigned long m = weights[i].size();
        unsigned long n = weights[i][0].size();
        vector<prob>result = dotTranspose(weights[i], layers[i]);
        sigmoid(result);
        for (unsigned long j = 0; j < result.size(); j++) {
            layers[i+1][j+1] = result[j];
        }
        //layers[i+1].insert(layers[i+1].begin() + 1, result.begin(), result.end());
    }
}


/**
 * Extracts the output data from the output layer (omitting the first bias node)
 * @return the output data
 */
vector<prob> ANN::output() {
    return omitDummy(layers[layers.size() - 1]);
}


/**
 * Classifies the output of the neural net with a digit label based on previously defined encodings
 * @return the calculated label
 */
int ANN::classify(const vector<prob> &input) {
    feedForward(input);
    prob min = 100.0; // Should be enough for a vector of size 10,000
    unsigned long digit = -1;
    vector<prob>out = output();
    for (unsigned long i = 0; i < encodings.size(); i++) {
        prob dist = euclideanDist(encodings[i], out);
        if (dist < min) {
            min = dist;
            digit = i;
        }
    }
    return digit;
}

/**
 * Omits a dummy variable (bias node) from a vector
 *
 * This really translates to "omits the first value"
 *
 * @param  input the input vector
 * @return the filtered vector
 */
vector<prob> ANN::omitDummy(const vector<prob> &input) {
    return vector<prob>(input.begin()+1, input.end());

}

/**
 * Computes the error between the output layer and the expected output
 *
 * While not necessary, this function was created for performance optimizations to combine all error operations into one atomic operation
 * This reduces the number of intermediate copies of these vectors which eliminates malloc and free calls
 *
 * @param  output
 * @param  expectedOutput
 * @return the computed error
 */
vector<prob> ANN::error(const vector<prob> &output, const vector<prob> &expectedOutput) {
    // assert(output.size() == expectedOutput.size());
    vector<prob>error(output.size());
    for (unsigned long i = 0; i < output.size(); i++) {
        error[i] = sigmoidPrime(output[i]) * (expectedOutput[i] - output[i]);
    }
    return error;
}

/**
 * Performs the back-propagation algorithm on the neural net
 * @param expectedOutput the vector of expected output values
 */
void ANN::backPropagate(const vector<prob> &expectedOutput) {
    // Set output layer errors
    unsigned long lastLayer = layers.size()-1;
    vector<prob>out = output();
    errors[lastLayer] = error(out, expectedOutput);
    // Run backpropagation
    for(unsigned long i = lastLayer - 1; i > 0; i--) {
        // cblas_dgemv (dot)
        // cblas_dscal (multiply)
        errors[i] = omitDummy(multiply(sigmoidPrime(layers[i]), dot(weights[i], errors[i+1])));
    }
}

/**
 * Adjusts the new weights according the the backpropagated errors and commits them to the network
 */
void ANN::commit() {
    for (unsigned long i = 0; i < weights.size(); i++) {
        // cblas_dger
        outerProductAddMultiply(weights[i], layers[i], errors[i+1], alpha);
    }
}

/**
 * Trains the neural network with a given input and output
 *
 * @param input  the input values form the training data
 * @param output the expected output
 */
void ANN::train(const vector<prob> &input, const vector<prob> &output) {
    // assert(input.size() == layers[0].size() - 1);
    // assert(output.size() == output.size());
    feedForward(input);
    backPropagate(output);
    commit();
}


/**
 * Compute the euclidean distance between two vectors
 *
 * @param  left the left vector
 * @param  right the right vector
 * @return the euclidean distance
 */
prob ANN::euclideanDist(const vector<prob> &left, const vector<prob> &right) {
    // assert(left.size() == right.size());
    prob dist = 0;
    for (unsigned long i = 0; i < left.size(); i++) {
        dist += powl(left[i] - right[i], 2);
    }
    return powl(dist, 0.5);
}


/**
 * Multiplies two vectors elementwise
 *
 * @param  left the left vector
 * @param  right the right vector
 * @return the result of the elementwise multiplication
 */
vector<prob> ANN::multiply(const vector<prob> &left, const vector<prob> &right) {
    // assert(left.size() == right.size());
    vector<prob>result(left.size());
    for (unsigned long i = 0; i < left.size(); i++) {
        result[i] = left[i] * right[i];
    }
    return result;
}

/**
 * Performs scalar multiplication on all elements of a vector
 *
 * @see cblas_dscal
 *
 * @param vec the vector
 * @param factor the factor by which to multiply
 * @return the multiplied vector
 */
void ANN::multiply(vector<prob> &vec, prob factor) {
    for (unsigned long i = 0; i < vec.size(); i++) {
        vec[i] = vec[i] * factor;
    }
}

/**
 * Performs an outer product on the two vectors and adds the result in place into a matrix
 *
 * This is the same as multiplying two column vectors U and V like so UV^T
 *
 * This function was bulit off of an original outerProduct function
 * but a multiplication and in place summation were added to reduce malloc and free calls and improve performance
 *
 * @see cblas_dger
 *
 * @param output the output matrix to add to
 * @param left the left vector (u) to multiply
 * @param right the right vector (v) to multiply
 * @param alpha the factor to multiply by
 */
void ANN::outerProductAddMultiply(vector<vector<prob> >&output, const vector<prob>&left, const vector<prob> &right, prob alpha) {
    for (unsigned long i = 0; i < left.size(); i++) {
        for (unsigned long j = 0; j < right.size(); j++) {
            output[i][j] += left[i] * right[j] * alpha;
        }
    }
}


/**
 * Adds values from a vector to another vector
 *
 * This function happens in place and mutates the left object.
 * This is fast but dangerous!
 *
 * @param left the left and output vector
 * @param right the right vector
 */
void ANN::add(vector<vector<prob> > &left, const vector<vector<prob> > &right) {
    assert(left.size() == right.size());
    for (unsigned long i = 0; i < left.size(); i++) {
        for (unsigned long j = 0; j < left[0].size(); j++) {
            left[i][j] += right[i][j];
        }
    }
}


/**
 * Performs a matrix multiplication operation on a matrix and a vector
 *
 * @see cblas_dgemv
 *
 * @param matrix the matrix to multiply (left operand)
 * @param vec the vector to multiply (right operand)
 * @return a vector containing the result of the matrix multiplication
 */
vector<prob> ANN::dot(const vector<vector<prob> >&matrix, const vector<prob>&vec) {
    unsigned long rows = matrix.size();
    unsigned long cols = matrix[0].size();
    // assert(cols == vec.size());
    vector<prob>result(rows);
    for (unsigned long i = 0; i < rows; i++) {
        prob sum = 0;
        for (unsigned long j = 0; j < cols; j++) {
            sum += matrix[i][j] * vec[j];
        }
        result[i] = sum;
    }
    return result;
}

/**
 * Performs a matrix multiplication operation on a tranposed matrix and a vector
 *
 * This is the same as dot() but it iterates over colums->rows instead of rows->columns
 *
 * @see cblas_dgemv
 *
 * @param matrix the matrix to multiply (left operand)
 * @param vec the vector to multiply (right operand)
 * @return a vector containing the result of the matrix multiplication
 */
vector<prob> ANN::dotTranspose(const vector<vector<prob> >&matrix, const vector<prob>&vec) {
    unsigned long rows = matrix.size();
    unsigned long cols = matrix[0].size();
    // assert(rows == vec.size());
    vector<prob>result(cols);
    // returns a matrix the size of this->rows() * right->cols()
    for (unsigned long i = 0; i < cols; i++) {
        prob sum = 0;
        for (unsigned long j = 0; j < rows; j++) {
            sum += matrix[j][i] * vec[j];
        }
        result[i] = sum;
    }
    return result;
}

void ANN::visualizeEncodings() {
    // Trickery here....
}

/**
 * Prints a vector description to the console
 * @param vector
 */
void ANN::print(const vector<prob>&vector) {
    printf("\n");
    for (unsigned long i = 0; i < vector.size(); i++) {
        printf("%f\t", vector[i]);
        printf("\n");
    }
    printf("\n");
}

/**
 * Prints a matrix description to the console
 * @param matrix
 */
void ANN::print(const vector<vector<prob> >&matrix) {
    printf("\n");
    for (unsigned long i = 0; i < matrix.size(); i++) {
        for (unsigned long j = 0; j < matrix[0].size(); j++) {
            printf("%f\t", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


/**
 * Prints the weights from the first node of the input layer to the nodes in the hidden layer
 */
void ANN::printFirstWeights() {
    cout << showpoint << fixed << setprecision(12);
    for (unsigned long i = 0; i < weights[0][1].size(); i++) {
        cout << weights[0][1][i] << " ";
    }
    cout << endl;
}

/* weights[0][0][1] = weight from layer 1 node to layer 2 node 2
  **/

void ANN::printAll(ostream &outstream){
  outstream << setprecision(17);
  for(unsigned long i = 0; i < weights.size(); i++) {
    for(unsigned long j = 0; j < weights[i].size(); j++){
      for(unsigned long k = 0; k < weights[i][j].size(); k++){
          outstream << weights[i][j][k] << " ";
        }
      }
      outstream << endl;
    }
}
