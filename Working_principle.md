The program loads the lotus image supplied to it in the code.

The program resizes the image to a desired size and converts it to grayscale. It then loops through all the image pixels and seperate them into Red, Green and Blue channels. 

Then the gray weighted value is calculated using the formular:

```
 0.299*Red channel + 0.587*Green channel + 0.114*Blue channel. 
```

The gray weighted value calculated for each pixel is assigned at the same coordinate position as the source image to an image variable.

*Gaussian blur algorithm* is performed on the grayscale image to reduce any noise it may have. Then the resulting image is *binarized*. The algorithm for the binarization of the image employs the idea of thresholding but an adaptive one. 

Again, the program loops through each pixel, finds the mean of its immediate 5x5 neigbours (i. e. the neighbouring pixels with the main one in the middle that forms a 5x5 matrix). Then the program compares each pixel within the 5x5 matrix with the calculated mean. If the pixel is greater than the mean, it sets the pixel value in the output image for that xy location to 1. If it's less, the pixel value for that location is set to 0. The process repeats for all pixels of the grayscale image.

The program is quite interactive to alter the value of the threshold used to binarize the image.

For the image segmentation algorithm, the program calculates the gradient of the grayscale image, inverts the pixels of the resulting outcome and either superimposes the results on the original grayscale or subtracts from it depending the segmentation option chosen.