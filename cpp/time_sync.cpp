/*
Author: xieweidong
Date: 2023-12-21
Description: 已有n个激光雷达关键帧的时间戳，以及m个图像帧，m>n，为每个激光雷达帧找到时间戳最近的图像帧
要求图像的时间戳范围能够覆盖关键帧的时间戳范围
确认文件名和时间戳文件中时间的单位是一致的
需要在72行，timestamps.push_back(data[k]); 中将k指定为时间戳所在的列，如 t,x,y,z,qx,qy,qz,qw 的情况下，k=0
*/

#include <iostream>
#include <sstream>
#include <dirent.h>
#include <iomanip>
#include <vector>
#include <boost/filesystem.hpp>

// linux读取文件夹下的文件
static std::vector<std::string> path_list(std::string path, bool sort=false)
{
    std::vector<std::string> file_names;
    if(!boost::filesystem::exists(path))
    {
        std::cout<<"path don't exist"<<std::endl;
        return file_names;
    }
    if(!boost::filesystem::is_directory(path))
    {
        std::cout<<"not a dir"<<std::endl;
        return file_names;
    }
    struct dirent* ptr;
    DIR* dir;
    dir = opendir(path.c_str());

    while((ptr=readdir(dir)) != NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        file_names.push_back(ptr->d_name);
    }

    if(sort)
    {
        std::sort(file_names.begin(), file_names.end());
    }
    
    return file_names;
}

class Synchronizer
{
public:
    std::string image_files_path;
    std::string timestamp_file_path;
public:
    void run()
    {
        // 列出file_path下所有的文件名并从小到大排序
        std::vector<std::string> image_file_names = path_list(image_files_path, true);
        std::cout << "total image file num: " << image_file_names.size() << std::endl;

        // 读取时间戳文件
        std::ifstream in(timestamp_file_path);
        std::string line;
        int cnt = 0;
        std::vector<double> timestamps;
        while(getline(in, line)) {
            std::stringstream ss(line);
            std::vector<double>data;
            double x;
            while(ss >> x) {
                data.push_back(x);
            }
            timestamps.push_back(data[1]);  // 指定时间戳所在列
        }
        std::cout << "total timestamp num: " << timestamps.size() << std::endl;

        // 设置cout的输出精度
        std::cout << std::setprecision(18);
        
        // 同步
        size_t index_image = 0;
        size_t index_stamp = 0;
        std::vector<std::string> keyframes;
        while(index_image < image_file_names.size()-1 && index_stamp < timestamps.size())
        {
            double t1 = std::stod(image_file_names[index_image].substr(0, image_file_names[index_image].length()-4))/1000;
            double t2 = std::stod(image_file_names[index_image+1].substr(0, image_file_names[index_image].length()-4))/1000;
            if(t1 <= timestamps[index_stamp] && t2 > timestamps[index_stamp])
            {
                if(std::abs(t1 - timestamps[index_stamp]) <= std::abs(t2 - timestamps[index_stamp]))
                {
                    keyframes.push_back(image_file_names[index_image]);
                }
                else
                {
                    keyframes.push_back(image_file_names[index_image+1]);
                }
                std::cout << image_file_names[index_image] << " => " << timestamps[index_stamp] << std::endl;
                index_stamp++;
            }
            if(index_stamp >= timestamps.size())
            {
                break;
            }
            if(t1 >= timestamps[index_stamp])
            {
                index_image--;
            }
            else
            {
                index_image++;
            }
        }
        std::cout << keyframes.size() << " frames synchronized" << std::endl;
    }

};

int main() {
    Synchronizer sync;
    sync.image_files_path = "/media/lwl/mdisk2/beijing_p0_xwd/image0_all/";
    sync.timestamp_file_path = "/media/lwl/mdisk2/beijing_p0_xwd/pose0.txt";
    sync.run();

    return 0;
}
