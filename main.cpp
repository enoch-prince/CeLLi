/* 
Project Name: CeLLi
Description: A CImg based image processing software for Edge detection 
and Image segregation using C++
Author: Enoch Prince
Date: July 30, 2019
 */

#include <iostream>
#include <string>
#include "include/CImg.h"
#ifndef imagepath
#define imagepath "imgs/"
#endif

using namespace std;
using namespace cimg_library;

/* Global Variables */
// const short FRAMEWIDTH = 320;
// const short FRAMEHEIGHT = 240;
const short DISPLAYWIDTH = 500;
const short DISPLAYHEIGHT = 425;
unsigned int keypress;
int saveCounter = 0;
bool vis;
string file_o;
bool showImage = false;
float timeElapsed, ThresholdVal, kConst = 0.25f;

enum TASK {
	binarizeImage, segmentImage, segmentImage_2,
	adaptiveThresholding, saveImage
};

CImg<float> gradientImage, buffer, finalImage, tmp;
CImgDisplay mainDisplay, bufferDisplay, finalDisplay, instrDisplay;
//CImgDisplay mainDisplay(DISPLAYWIDTH, DISPLAYHEIGHT,"Main Display", 0), bufferDisplay(DISPLAYWIDTH, DISPLAYHEIGHT,"Buffer Display", 0);
TASK task = binarizeImage;


/* Function Prototypes */
CImg<float> convertToGrayScale(CImg<float>& img){
    return img.RGBtoYCbCr().channel(0);
}

CImg<float> gradient_cimg(CImg<float> image, int type=2)
{
  CImgList<> gradList = image.get_gradient("xy",type);
  CImg<> result = (gradList[0].pow(2) + gradList[1].pow(2)).sqrt().normalize(0,255);
  
  return result;
}

void convertToGrayScale(CImg<float>& srcImg, CImg<float>& destImg){
    int R, G, B;
    int grayWeightedValue;
    cimg_forXY(srcImg, x, y){
        /* seprate the channels */
        R = srcImg(x, y, 0, 0);
        G = srcImg(x, y, 0, 1);
        B = srcImg(x, y, 0, 2);

        /* Calculate the weighted grayscale value */
        grayWeightedValue = (int)(0.299*R + 0.587*G + 0.114*B);
        destImg(x, y, 0, 0) = grayWeightedValue;
    }
}

float adaptiveThresholdValue(CImg<float>& img, int kSize = 5){
    float nNsum; //Nearest Neighbour Sum
    float thresholdValue = 0.0f;
    int wx = img.width(), hy = img.height();
    const unsigned long wh = wx * hy;
    const unsigned long whd = wh * img.depth();
    int i, j, itr = 0;
    //CImg<> binarizedImage(wx, hy, 1, 1);
    for(int x = 0; x < wx; x+=5){
        for(int y = 0; y < hy; y+=5){
            nNsum = 0.0f;
            
    		for(i = x; i <= (kSize-1)+x; i++)
    			for(j = y; j <= (kSize-1)+y; j++)
    				nNsum += img(i, j, 0, 0, wh, whd);
			
	        nNsum /= (kSize*kSize);
	        //cout << "nNsum: " << nNsum << endl;
			
	    	thresholdValue += nNsum;	
			itr++;	 
	    } 
	}
	//cout << "Thr sum: "<< thresholdValue << "\tItr: "<< itr << endl;
    return thresholdValue /= itr;
}

CImg<float> binarize(CImg<float>& img, float k, int kSize = 5){
    float nNsum; //Nearest Neighbour Sum
    int wx = img.width(), hy = img.height();
    const unsigned long wh = wx * hy;
    const unsigned long whd = wh * img.depth();
    int i, j, itr;
    CImg<> binarizedImage(wx, hy, 1, 1);
    for(int x = 0; x < wx; x+=5){
        for(int y = 0; y < hy; y+=5){
            nNsum = 0.0f;
            
    		for(i = x; i <= (kSize-1)+x; i++)
    			for(j = y; j <= (kSize-1)+y; j++)
    				nNsum += img(i, j, 0, 0, wh, whd);
			
	        nNsum /= (kSize*kSize);
	        
	        for(i = x; i <= (kSize-1)+x; i++)
    			for(j = y; j <= (kSize-1)+y; j++)
    				binarizedImage(i, j, 0, 0, wh, whd) = (img(i,j,0,0,wh,whd) > (nNsum*k) ) ? 1.0f:0.0f;
    		
			itr++;
	    	ThresholdVal += nNsum;		 
	    } 
	}
    ThresholdVal /= itr;
    return binarizedImage;
}

CImg<float> binarize(CImg<float>& img, int kSize = 5){
    int wx = img.width(), hy = img.height();
    const unsigned long wh = wx * hy;
    const unsigned long whd = wh * img.depth();
    int i, j;
    float onesCount, zerosCount;
    CImg<> binarizedImage(wx, hy, 1, 1, 0);
    for(int x = 0; x < wx; x+=5){
        for(int y = 0; y < hy; y+=5){
           onesCount = 0; zerosCount = 0;
            
    		for(i = x; i <= (kSize-1)+x; i++)
    			for(j = y; j <= (kSize-1)+y; j++){
    				 if(img(i, j, 0, 0, wh, whd) == 1.0f) onesCount++;	 	
					 else  zerosCount++;
				}
			//cout << "Ones Count: " << onesCount << "\tZeros Count: " << zerosCount << endl;
	        float ratio = (onesCount >= zerosCount) ? ((zerosCount != 0)?(onesCount/zerosCount):onesCount) : ((onesCount != 0)?(zerosCount/onesCount):zerosCount);	        
	        //cout << "Ratio: " << ratio << endl;
	        	for(i = x; i <= (kSize-1)+x; i++)
	    			for(j = y; j <= (kSize-1)+y; j++){
	    				if(ratio >= 1.0f)
		    				binarizedImage(i, j, 0, 0, wh, whd) = (onesCount > zerosCount) ? 1.0f:0.0f;
		    			else binarizedImage(i, j, 0, 0, wh, whd) = img(i,j,0,0,wh,whd);
					}	 
	    } 
	}
    return binarizedImage;
}
/*
CImg<float> binarize(CImg<float>& img, int kSize = 5){
    float nNsum; //Nearest Neighbour Sum
    int wx = img.width(), hy = img.height();
    int i, j, itr;
    CImg<> binarizedImage(wx, hy, 1, 1);
    for(int y = 0; y < hy; y++){
        for(int x = 0; x < wx; y++){
            nNsum = 0.0f;
            
    		for(i = -2; i < (kSize-2); i++){
    			nNsum += img(std::max(std::min(x-i, wx-1), 0), std::max(std::min(y-i, hy-1), 0), 0, 0);
			}
	        nNsum /= (kSize*kSize);
	        
	        for(i = -2; i < (kSize-2); i++){
    			binarizedImage(std::max(std::min(x-i, wx-1), 0), std::max(std::min(y-i, hy-1), 0), 0, 0) = (img(std::max(std::min(x-i, wx-1), 0), std::max(std::min(y-i, hy-1), 0), 0, 0) >= nNsum ) ? 1.0f:0.0f; 
			}
			itr++;
	        ThresholdVal += nNsum;
        }  
    }
    ThresholdVal /= itr;
    return binarizedImage;
}
*/

CImg<> dilateImage(CImg<>& img){
	CImg<> s = img;
		//s = s.dilate(5) & img;
		s.dilate(2);
		cout << "Image Dilated" << endl;
		return s;
}

void setUp(int argc, const char** argv){ 
	// Read and check command line parameters.
	cimg_usage("Image Processing Algorithms for 2D images");
	const char *file_i = cimg_option("-i",imagepath "5.jpg","Input Image");
	//const char *file_o  = cimg_option("-o",(char*)0,"Output Image");
	vis     = cimg_option("-visu",true,"Visualization mode");
	system("mkdir out");

	CImg<float> originalImage(file_i);
	originalImage.resize(DISPLAYWIDTH, DISPLAYHEIGHT);
	CImg<float> grayScaleImage(originalImage.width(), originalImage.height());

	mainDisplay = CImgDisplay( cimg_fitscreen(originalImage.width(), originalImage.height(), 3), "Main Display");
	bufferDisplay = CImgDisplay( cimg_fitscreen(originalImage.width(), originalImage.height(), 3),"Original Image");
	finalDisplay = CImgDisplay( cimg_fitscreen(originalImage.width(), originalImage.height(), 1),"Final Image Display");
	instrDisplay = CImgDisplay(500, 500, "Instruction Display", 0);
	CImg<unsigned char> instrText(instrDisplay.width(), instrDisplay.height(), 1, 3);
	const unsigned char dark_green[] = {1, 115, 1};
	instrText.draw_text(5, 5, "\tInstructions for Interactivity of CeLLi \n"
								"\t===========================\n\n"
								"The program starts with the Global \nThresholding Algorithm but implements\n"
								"the following algorithms. Choose \nthe option by hitting the corresponding key:\n\n"
								"1. Global Thresholding ===> B \n"
								"\tOnce B is selected use Z and X to \n\tdecrement and increment the threshold value\n\n"
								"2. Adaptive Thresholding ===> A \n\n"
								"3. Image Segmentation - Option 1 ===> D \n\n"
								"4. Image Segmentation - Option 2 ===> S \n"
								"\n\tAlways click on the Main Display to \n\tactivate Interactivity\n\t\t\t\tENJOY!!!", dark_green, 0, 1, 23);
	instrDisplay.display(instrText);
	//buffer = convertToGrayScale(originalImage);
	convertToGrayScale(originalImage, grayScaleImage);
	CImg<float> gray_blur = grayScaleImage.get_blur(2);
	//gray_blur.move_to(gradientImage);
	//gray_blur.display(bufferDisplay);
	gradientImage = gradient_cimg(gray_blur);
	//gray_blur = -gradientImage;
	tmp = gray_blur;
	gray_blur.display(bufferDisplay);
	//gradientImage.display(bufferDisplay);
	//gray_blur = buffer.watershed(gradientImage);
	//buffer = gray_blur;
}


void process(){
 	//convertToGrayScale(originalImage, buffer);
	timeElapsed = 1.0f / mainDisplay.frames_per_second();
	keypress = mainDisplay.key();
	if(keypress == cimg::keyB) task = binarizeImage;
	if(keypress == cimg::keyD) task = segmentImage;
	if(keypress == cimg::keyS) task = segmentImage_2;
	if(keypress == cimg::keyA) task = adaptiveThresholding;
	if(keypress == cimg::keyC) task = saveImage;
	
	switch(task){
		
		case binarizeImage:
			if(mainDisplay.key() == cimg::keyX) kConst += 0.01f * timeElapsed;
			if(mainDisplay.key() == cimg::keyZ) kConst -= 0.01f * timeElapsed;
			kConst = (kConst > 1.0f) ? 1.0f:(kConst < 0.0f) ? 0.0f:kConst;
			
			//buffer = binarize(gradientImage, kConst);
			buffer = gradientImage.get_threshold(kConst*255);
			
			break;
		case segmentImage:
			buffer = -gradientImage;
			//finalImage = buffer.get_sqr().normalize(0,255) - tmp;
			finalImage = tmp - buffer.get_sqr().normalize(0,255);
//			buffer = gradientImage.get_sqr().normalize(0, 255);
//			finalImage = buffer - tmp;
			break;
		case segmentImage_2:
			buffer = gradientImage.get_sqr().normalize(0, 255);
			finalImage = buffer - tmp;
			finalImage.normalize(0, 255);
			break;
		case adaptiveThresholding:
			ThresholdVal = adaptiveThresholdValue(gradientImage);
			cout << "Adaptive Threshold: " << ThresholdVal << endl;
			finalImage = gradientImage.get_threshold(ThresholdVal);
			finalImage = -finalImage;
			finalImage.normalize(0, 255);
			break;
		case saveImage:
			++saveCounter;
			file_o = "out/" + to_string(saveCounter) + ".jpg";
			finalImage.save(file_o.c_str());
			cout << "Successfully saved " << file_o << endl;
		default:
			break;				
	}
		
}  

void displayResults(){
	if(!buffer.is_empty()){
		buffer.display(mainDisplay);
		// mainDisplay.display(buffer);
		if(!finalImage.is_empty()){
			finalImage.display(finalDisplay);
		}
	}
}

/**************************************************************
************************  MAIN PROGRAM  ***********************
***************************************************************/
int main(int argc, const char** argv) {

    setUp(argc, argv);

    while(!mainDisplay.is_closed()){

        process();
        
        if(vis) displayResults();

        mainDisplay.wait();
    }
    
	system("pause");
    return 0;
}
