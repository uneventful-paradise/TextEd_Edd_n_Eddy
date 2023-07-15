#include <iostream>
using namespace std;
#include "../include/DocumentManager.h"


int main(int argc, char*argv[])
{
    string path;
    char ch;
    path = "../assets/testDoc.txt";
    DocumentManager document;
    document.open(path);
    return 0;
}
