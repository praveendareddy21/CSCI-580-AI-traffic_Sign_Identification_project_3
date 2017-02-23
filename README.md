# Project Description
Artificial Neural Network to classify US roadsigns from 32X32 size images.
We have used sigmoid activation function for back-propagation algorithm and used euclidean distance for output layer classification.
We used BLAS (Basic Linear Algebra Subprograms) routines for faster training of dataset.

# Dataset Preparation Steps
1. scraped images from bing for different kinds of road signs
2. filtered out all the incorrect ones
3. resized to 16x16 with color and 32x32 in grayscale
4. normalized the pixel values to be in range of 0.0  to 1.0
5. extracted the value for each pixel and wrote it to a .bin file in order to read it as input to our neural network

# Data Augmentation
 We performed data augmentation on dataset to counter overfitting and to increase dataset size
 We used python libraries ImageMagick and PIL for flipping over X-axis ,rotating images by 15 degrees , cropping out parts etc


# Traffic Signs Encoding
We used the following encoding to store traffic sign data.

Crosswalk - 0<br />
No Parking -1<br />
Railroad - 2<br />
Speed Limit -3<br />
Stop sign - 4<br />
Yield -5<br />

# CSCI_580
A place for resources for AI.

## Formulas:



