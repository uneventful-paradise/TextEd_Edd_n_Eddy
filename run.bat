@echo off
echo -----Starting----
echo.


g++ -g -c ./src/*.cpp
g++ -g *.o -o ./bin/TextEdd.exe
del *.o
cd ./bin
TextEdd.exe
cd ..


echo.
echo -------End-------
pause