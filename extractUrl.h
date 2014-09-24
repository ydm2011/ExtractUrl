/*
 * =====================================================================================
 *
 *       Filename:  extractUrl.h
 *
 *    Description:  this file define the interface of the extract algorthims
 *
 *        Version:  1.0
 *        Created:  2014年09月17日 16时07分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yu daoming (), ydm2011@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <string>
#include <map>
#include <list>
#define _DEBUG_
//this is the interface of the 
class ExtractContentInterface{
public:
    enum MatchMode{ONE,ALL};

    //extract the position of tag from the src html file; 
    virtual int  extractTag(const char* src,size_t src_len,
                            const char* tag, size_t tag_len,
                            size_t& match_position)=0;
    //this function extract the pattern tag content that pattern 
    //from the given str;
    virtual void extract(const char* src,
                         size_t src_len,
                         const char* attribute,
                         size_t att_len,
                         std::string& content)=0;
    
    //this function will return  the match strings' position;
    virtual int getMatch(const char* src, size_t src_len,
                         const char* pattern, size_t pat_len,
                         std::list<size_t>& match_position,
                         int mode=ONE)=0;
    //release the data
    virtual ~ExtractContentInterface(){}
    //get the required data;
    virtual int getAttributeOfTag(const char* src,
                                  size_t src_len,
                                  const std::string& attribute_name,
                                  size_t& tag_location)=0;
};

//this use sunday algorithm to match the given pattern
class ExtractBySunday:public ExtractContentInterface{
public:
       
    //this will extract the tag content of the html file;
    int extractTag(const char* src, size_t src_len,
                   const char* tag, size_t tag_len,
                   size_t& match_position);


    //use sunday algorithm  to match the given pattern
    virtual int getMatch(const char* src, size_t src_len, 
                         const char* pattern, size_t pat_len,
                         std::list<size_t>& match_position,
                         int mode=ONE);
    //use sunday algorithm to match the given pattern and extract
    //the given attribute content;
    void extract(const char* src, size_t src_len,
                 const char* attribute,size_t attr_len,
                 std::string& content);

    //get the location of the given attribute of the tag
    int getAttributeOfTag(const char* src, 
                          size_t src_len,
                          const std::string& attribute_name,
                          size_t& tag_location);
private:
    //locate the patten character's loaction in the pattern
    int* preProcessPattern(const char*pattern,size_t pat_len);
    
    //just use the temp store continer
    std::string tag_container;
    int pattern_position[256];
};
//extract the url from the so.com
class ExtractUrlFromSo{
public:
    ExtractUrlFromSo(ExtractContentInterface* extractAlgorithm);
    //get the
    void init(const std::string& layers);

    int getSearchResultContainer(const char* src,size_t src_len,
                                 const std::pair<std::string,std::string>& id,
                                 size_t& result_loca,
                                 const std::string& result_tag);
    int getUrls(const char* src,
                size_t src_len,
                const std::string& url_tag,
                std::list<std::string>& urls);
protected:
    int extractUrl(const char* src, size_t src_len,bool& is_last,std::string& url,size_t& curr_iter);
private:
    ExtractContentInterface* extract_obj;
    std::string pattern;
};

