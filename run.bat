@echo off
echo -----Starting----
echo.


g++ -g -c ./src/*.cpp -std=c++17 
g++ -g *.o -o ./bin/TextEdd.exe -std=c++17 
del *.o
cd ./bin
TextEdd.exe
cd ..


echo.
echo -------End-------
pause