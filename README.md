Project Description


# Dataset Preparation Steps
1. scraped images from bing for different kinds of road signs
2. filtered out all the incorrect ones
3. resized to 16x16 with color and 32x32 in grayscale
4. normalized the pixel values to be in range of 0  to 1.0
5. extracted the value for each pixel and wrote it to a .bin file in order to read it as input to our neural network

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

![Formulas](Screen Shot 2016-09-06 at 1.03.51 PM.png)

![Formulas](Screen Shot 2016-09-06 at 1.05.24 PM.png)

![Formulas](Screen Shot 2016-09-06 at 1.12.50 PM.png)

