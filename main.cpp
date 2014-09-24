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

    ExtractBySunday test_extract;
    list<size_t> match_locations;
    size_t match_postion;
    time_t start;
    test_extract.getMatch(contents.c_str(),
                          contents.size(),
                          "<ul ",4,match_locations,
                          ExtractContentInterface::ALL);
    test_extract.extractTag(contents.c_str(),contents.size(),
                            "<ul ",4,
                            match_postion);
    cout<<string(contents.c_str(),match_locations.front(),4)<<endl;
    size_t attribute_loca;
    test_extract.getAttributeOfTag(contents.c_str()+match_postion,contents.size()-match_postion,"id",attribute_loca);
    string attribute_value;
    test_extract.extract(contents.c_str()+match_postion+attribute_loca,contents.size()-match_postion,"id",2,attribute_value);
    cout<<"id:"<<attribute_value<<endl;

    ExtractBySunday *extract = new ExtractBySunday;
    ExtractUrlFromSo extract_so(extract);
    pair<string,string> id("id","m-result");
    cout<<"content size:"<<contents.size()<<endl;
    size_t result_loca;

    extract_so.getSearchResultContainer(contents.c_str(),contents.size(),id,result_loca,"<ul ");

    list<string> urls;
    extract_so.getUrls(contents.c_str(),contents.size(),"<h3",urls);
    string temp_test(contents,result_loca,10);
    cout<<temp_test<<endl;
    cout << "Hello World!" << endl;
    return 0;
}

