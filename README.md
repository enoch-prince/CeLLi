# Project CeLLi
A C++ image processing application based on the CImg library built for Windows Platform. Implemented only global thresholding, adaptive thresholding and segmentation. 

## How to Compile the Code

 -  Create a CeLLi.vcproj in Visual Studio and build it from there.<br>
    If this fails proceed to step 2.


 -  Search for "x64 Native Tools Command Prompt for VS 2019" from the Windows Start Menu. A command prompt instance will open. Navigate to the project CeLLi directory.

Run:
 
```bash
compile.bat
```

## How to Run the Executable

The program uses command line arguments to process different images other than the one specified directly inside the source code.

usage: 

```bash
.\CeLLi.exe -i fullPath\imageName.jpg -visu true
```

> The arguments:
>	**-i**  => specifies the full path to the image.
>	**-visu** => enables visualization (to display whatever is happening on the screen). 
> 		 Can be set to true or false.


**Note:** When in Visualization mode you can press "C" on the keyboard to save the final Image

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.
