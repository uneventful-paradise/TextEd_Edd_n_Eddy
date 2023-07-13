#include <iostream>
using namespace std;
#include "DocumentManager.h"


int main(int argc, char*argv[])
{
    string path;
    char ch;
    printf("Enter the document's path:\n");
    getline(cin, path);
    if (path == "") {
        printf("nu ai introdus un path\n");
        path = "./testDoc.txt";
    }
    printf("%s\n", argv[0]);
    DocumentManager document;
    document.open(path);
    return 0;
}
