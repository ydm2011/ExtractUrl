#include <iostream>
#include <fstream>
#include "extractUrl.h"
#include <ctime>
using namespace std;

int main()
{
    ifstream in;

    in.open("/home/daoming/Desktop/index.html",ios::in|ios::binary);
    if(!in)
    {
        std::cout<<"index.html"<<" Not Found!"<<endl;
        return -1;
    }
    string contents;
    in.seekg(0,ios::end);
    contents.resize(in.tellg());
    in.seekg(0,ios::beg);
    in.read(&contents[0],contents.size());
    in.close();



    ExtractBySunday *extract = new ExtractBySunday;
    ExtractUrlFromSo extract_so(extract);

    list<string> urls;
    extract_so.getUrls(contents.c_str(),contents.size(),"<h3",urls);

    return 0;
}

