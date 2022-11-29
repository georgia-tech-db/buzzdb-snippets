#include<vector>
#include<string>
#include "utils/run_length_encoding.h"

namespace buzzdb 
{
    namespace utils 
    {

        std::string RunLengthEncoding::compress() {        
        std::string res="";
        uint64_t i=0;
        while(i<s.length()){
            char current_char = s[i];
            uint64_t j=i+1, char_count=1;
            while(j<s.length()){
                if(s[j]==current_char){
                    j++;
                    char_count++;
                }
                else
                    break;
            }
            res+=(std::to_string(char_count)+current_char);                                    
            i=j;
        }
        return res;
        }   
    }
}



