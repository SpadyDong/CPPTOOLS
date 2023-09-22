#ifndef _MATH_TOOLS_H
#define _MATH_TOOLS_H

#include <dirent.h>
#include <vector>
#include <string>

// from "1.23 1.45 456" to [1.23, 1.45, 456]
std::vector<float> segment(std::string words)
{
    std::vector<float> nums;
    std::string word;
    for(int i=0; i<words.length(); i++)
    {
        if(words[i]==' ')
        {
            nums.push_back(std::stof(word));
            word="";
        }
        else
        {
            word = word + words[i];
        }
    }
    nums.push_back(std::stof(word));
    return nums;
}

#endif