/*
 * =====================================================================================
 *
 *       Filename:  extractUrl.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年09月17日 20时29分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yu daoming (), ydm2011@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>
#include <ctime>
#include "extractUrl.h"
#include <memory>
using namespace std;
// get the start position of the given tag;
int  ExtractBySunday::extractTag(const char* src,
                                 size_t src_len,
                                 const char* tag,
                                 size_t tag_len,
                                 size_t& match_position)
{
    list<size_t> match_locations;
    getMatch(src,src_len,tag,tag_len,match_locations,ExtractBySunday::ONE);
    if(match_locations.empty())
    {
        return -1;
    }else match_position = match_locations.front();
    return 0;
}
//extract the content from the attribute of the given tag 
//now this function just extract between the "***" content parrern; 
void ExtractBySunday::extract(const char* src,
                              size_t src_len,
                              const char* attribute, 
                              size_t att_len,
                              string& content)
{
    int status=0;
    size_t src_iter=0;
    size_t start=0;
    while (src_iter<src_len&&status!=2)
    {
        if(src[src_iter]=='"')
         {
             ++status;
             if(status ==1)
                 start = src_iter;
         }
        ++src_iter;
    }
    content = string(src+start+1,src_iter-start-2);
}
//this function return the position of the match substring,
//using  sunday algorithm 
int ExtractBySunday::getMatch(const char* src, size_t src_len, 
                              const char* pattern, size_t pat_len,
                              list<size_t>& match_position,
                              int mode)
 
{
    if(src==NULL||pattern==NULL)
    {
        std::cout<<"error parameter in ExtractBySunday::getMatch()"<<endl;
        return -1;
    }
    
    int* character_position;
    character_position = preProcessPattern(pattern,pat_len);

    size_t src_iter = 0;
    size_t pattern_iter = 0;
    
    size_t current_start;

    int next_char;
    while(src_iter<src_len - pat_len)
    {   
        current_start = src_iter;
        while(pattern_iter<pat_len&&src[current_start]==pattern[pattern_iter])
        {
            ++current_start;
            ++pattern_iter;
        }
        if(pattern_iter==pat_len)
        {
            match_position.push_back(src_iter);
            if(mode == ONE)
               return 0;
        }
        next_char = (int)src[src_iter+pat_len];
        if(character_position[next_char]==-1)
            src_iter += pat_len+1;
        else src_iter += pat_len - character_position[next_char];
        pattern_iter = 0;
    }
    if(match_position.empty())
        return -1;
    return 0;
}
//this function compute the poistion of each character in the pattern;
//and then it will be simplify the match the string; 
int* ExtractBySunday::preProcessPattern(const char* pattern,size_t pat_len)
{
    int* shift_position;
    for(int i=0; i<256; i++)
    {
        pattern_position[i]=-1;
    }
    shift_position = pattern_position+128;
    //shift the start to the middle of the allocated memory;
    for(int i=0; i< pat_len;i++)
    {
        shift_position[pattern[i]] = i; 
    }

    //shift the start to the middle of the allocated memory;
    return shift_position;
}
//this function is to get the attribute of the given tag,the tag must be a 
//closed tag ,or not it'll be unexpected error!
//by the way: this function will updated in  next version :
int ExtractBySunday::getAttributeOfTag(const char* src,size_t src_len, 
                                       const string& attribute_name,
                                       size_t& tag_location)
{
    if(!src||attribute_name.empty())
    {
        cout<<"wrong parameters!"<<endl;
    }
    tag_container.clear();
    size_t src_iter=0;

    //int att_start=0;
    //int att_end=0;
    bool flag=true;
    char current_char;
    while(src_iter<src_len)
    {
        current_char = src[src_iter];
        //check whether in the value of the attriute
        if(current_char=='"')
        {
            flag = !flag;
            ++src_iter;
            continue;
        }
        //the seperate flag of the words
        if(flag&&(current_char==' '||current_char=='='))
        { 
            if(!tag_container.empty()&&!tag_container.compare(attribute_name))
            {
                tag_location = src_iter-attribute_name.size();
                return 0;
            }           
            ++src_iter;
            tag_container.clear();
            continue;
        }
        //check whether is the attibute name;
        if(flag&&((current_char<='z'&&current_char>='a')||(current_char<='Z'
           &&current_char>='A')))
        {
            tag_container.push_back(current_char);
            ++src_iter;
            continue;
        } 
        if(src[src_iter]=='>')
        {
            return -1;
        }
        ++src_iter;
    }
    return -1;
}




//this just extract the url from the so.com search result
//
ExtractUrlFromSo::ExtractUrlFromSo(ExtractContentInterface* extract_obj)
{
    this->extract_obj = extract_obj;
}

void ExtractUrlFromSo::init(const string &layers)
{
    ;
}

//get the container that contain the search result;
int ExtractUrlFromSo::getSearchResultContainer(const char* src,
                                               size_t src_len,
                                               const pair<string,string>&id,
                                               size_t& result_loca,
                                               const string& result_tag="<ul ")
{
    size_t src_iter=0;
    size_t content_position=0;
    size_t attribute_position=0;
    size_t curr_len=src_len;
    int flag=0;

    string attribute_value;
    while(src_iter<src_len)
    {

        flag = extract_obj->extractTag(src,curr_len,
                                       result_tag.c_str(),
                                       result_tag.size(),
                                       content_position);
        if(flag==-1)
        {
            return -1;
        }
        src += content_position;
        src_iter += content_position;
        curr_len -= content_position;
        //after got the need tag,check the attribute of the tag;
        flag = extract_obj->getAttributeOfTag(src,curr_len,id.first,attribute_position);
        src += attribute_position;
        src_iter += attribute_position;
        curr_len -= attribute_position;
        if(flag ==-1)
        {
            flag =0;
            ++src;
            ++src_iter;
            continue;
        }
        extract_obj->extract(src,curr_len,result_tag.c_str(),result_tag.size(),attribute_value);
        if(!attribute_value.compare(id.second))
            break;
    }
    result_loca = src_iter;
    return 0;
}
//
int ExtractUrlFromSo::getUrls(const char* src,size_t src_len,
                              const string& url_tag,list<string>& urls)
{
    size_t curr_iter=0;
    size_t next_iter=0;

    int flag=0;
    //get the search result div
    getSearchResultContainer(src,src_len,make_pair<string,string>("id","m-result"),next_iter,"<ul ");
    if(next_iter==curr_iter)
        return -1;
    src += next_iter;
    src_len -= next_iter;
    curr_iter += next_iter;
    //get the position of the result list;
    bool is_last = false;
    string temp_url;
    while(!is_last)
    {
        if(extractUrl(src,src_len,is_last,temp_url,next_iter)==-1)
            return -1;
        urls.push_back(temp_url);
        src += next_iter;
        src_len -= next_iter;
    }
    return 0;
}

int ExtractUrlFromSo:: extractUrl(const char* src, size_t src_len,bool& is_last,string& url,size_t& curr_iter)
{
    curr_iter=0;
    size_t next_iter=0;
    int flag=-1;
    //loca the next list of the search result:<li class="res-list"
    getSearchResultContainer(src,src_len,make_pair<string,string>("class","res-list"),next_iter,"<li ");
    if (next_iter==curr_iter)
        return -1;


    //check the current tag was the last search result:<li id="last"
    src += next_iter;
    src_len -= next_iter;
    curr_iter += next_iter;
    next_iter =0;
    flag = extract_obj->getAttributeOfTag(src,src_len,"id",next_iter);

    src += next_iter;
    src_len -= next_iter;
    curr_iter += next_iter;
    if(flag==0)
    {
        string id_value;
        extract_obj->extract(src,src_len,"id",2,id_value);
        if(id_value.compare("last"))
            is_last = true;
        else is_last = false;
    }
    //get the loca of the "<h3"
    next_iter =0;
    extract_obj->extractTag(src,src_len,"<h3",3,next_iter);
    //get the url location
    src += next_iter;
    src_len -= next_iter;
    curr_iter += next_iter;
    next_iter =0;
    extract_obj->extractTag(src,src_len,"<a ",3,next_iter);

    //get the url
    src += next_iter;
    src_len -= next_iter;
    curr_iter += next_iter;
    //extract the url
    next_iter =0;
    extract_obj->extract(src,src_len,"href",4,url);
    return 0;
}














