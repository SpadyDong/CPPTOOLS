#ifndef _FILE_TOOLS_H
#define _FILE_TOOLS_H

#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>
#include <boost/filesystem.hpp>

static bool cmp(std::string a, std::string b)
{
	if (a < b)
	{
		return true;
	}
	return false;
}

// 检查文件名后缀
bool checkPostfix(std::string fileName, std::string postfix)
{
    if(fileName.size()>4 && fileName.substr(fileName.size()-3, 3) == postfix && fileName[fileName.size()-4]=='.')
    {
        return true;
    }
    return false;
}

// linux读取文件夹下的文件
std::vector<std::string> pathList(std::string path, bool sort=false)
{
    std::vector<std::string> files;
    if(!boost::filesystem::exists(path))
    {
        std::cout<<"path don't exist"<<std::endl;
        return files;
    }
    if(!boost::filesystem::is_directory(path))
    {
        std::cout<<"not a dir"<<std::endl;
        return files;
    }
    struct dirent* ptr;
    DIR* dir;
    dir = opendir(path.c_str());

    while((ptr=readdir(dir)) != NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        files.push_back(ptr->d_name);
    }

    if(sort)
    {
        std::sort(files.begin(), files.end(), cmp);
    }
    
    return files;
}

// linux读取文件夹下的文件
std::vector<std::string> pathList(std::string path, std::string postfix, bool sort=false)
{
    std::vector<std::string> files;
    struct dirent* ptr;
    DIR* dir;
    dir = opendir(path.c_str());

    while((ptr=readdir(dir)) != NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        if(checkPostfix(ptr->d_name, postfix))
            files.push_back(ptr->d_name);
    }

    if(sort)
    {
        std::sort(files.begin(), files.end(), cmp);
    }
    
    return files;
}

#endif