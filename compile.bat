REM ----------------------------------------------------------------
REM
REM Script to compile Project CeLLi, using Microsoft Visual C++.
REM
REM Copy this script into 'CeLLi/' and run it to compile
REM the main.cpp file.
REM
REM ----------------------------------------------------------------

cl /W4 /wd"4127" /wd"4311" /wd"4312" /wd"4512" /wd"4571" /wd"4640" /wd"4706" /wd"4710" /wd"4800" /wd"4804" /wd"4820" /wd"4996" /Ox /Ob2 /Oi /Ot /c /EHsc /D "_CRT_SECURE_NO_WARNINGS" /I"%SDKPATH%\Include" /I"..\\" main.cpp
link /LIBPATH:"%SDKPATH%\Lib" main.obj user32.lib gdi32.lib shell32.lib /out:CeLLi.exe