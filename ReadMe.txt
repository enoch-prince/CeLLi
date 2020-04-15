How to Compile the Code
=======================
1. Open the CeLLi.vcproj in Visual Studio and build it from there.

If this fails proceed to step 2.

2. Run the "x64 Native Tools Command Prompt for VS 2019". A command prompt will open up. Then type:
	compile.bat

Press enter and the program will compile. The Executable CeLLi.exe will be created. Run it.

In case this doesn't work too..... proceed to step 3.

3. Search for "x64 Native Tools Command Prompt for VS 2019" from the Windows Start Menu. A command prompt instance will open. 
Navigate to the project CeLLi directory.
Run the compile.bat file

How to Run the Executable
=========================
The program uses command line arguments to process different images other than the one specified directly inside the source code.

usage: 
	.\CeLLi.exe -i fullPath/imageName.jpg -visu true


The arguments:
	-i  => specifies the full path to the image
	-visu => enables visualization (to display whatever is happening on the screen). Can be set to true or false

Note: When in Visualization mode you can press "C" on the keyboard to save the final Image