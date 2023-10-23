#ifndef _POINT_CLOUD_H
#define _POINT_CLOUD_H

#include <opencv2/opencv.hpp>
#include <pcl/point_cloud.h>

template<typename T>
cv::Mat point2BEV(T pc, int width, int scale)
{
    cv::Mat board = cv::Mat::zeros(width, width, CV_32FC1);
    int n = pc->size();
    #pragma omp parallel for num_threads(4)
    for(int i=0; i<n; i++)
    {
        int x = std::min(std::max(0, int(pc->points[i].x*scale+width/2)), width-1);
        int y = std::min(std::max(0, int(pc->points[i].y*scale+width/2)), width-1);
        board.at<float>(x,y) = 1;
    }
    return board;
}

#endif