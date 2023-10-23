#ifndef _MATH_TOOLS_H
#define _MATH_TOOLS_H

#include <Eigen/Dense>
#include <pcl/point_cloud.h>

template<typename T>
float pointDepth(T p)
{
    float depth = std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    return depth;
}

// 输入：一个四元数（实部和虚部）
// 输出：一个3x3的旋转矩阵
Eigen::Matrix3d quaternionToRotationMatrix(double w, double x, double y, double z)
{
    Eigen::Matrix3d rotationMatrix;

    double sqw = w * w;
    double sqx = x * x;
    double sqy = y * y;
    double sqz = z * z;

    // 旋转矩阵的第一列
    rotationMatrix(0, 0) = sqx - sqy - sqz + sqw;
    rotationMatrix(1, 0) = 2.0 * (x * y + z * w);
    rotationMatrix(2, 0) = 2.0 * (x * z - y * w);

    // 旋转矩阵的第二列
    rotationMatrix(0, 1) = 2.0 * (x * y - z * w);
    rotationMatrix(1, 1) = -sqx + sqy - sqz + sqw;
    rotationMatrix(2, 1) = 2.0 * (y * z + x * w);

    // 旋转矩阵的第三列
    rotationMatrix(0, 2) = 2.0 * (x * z + y * w);
    rotationMatrix(1, 2) = 2.0 * (y * z - x * w);
    rotationMatrix(2, 2) = -sqx - sqy + sqz + sqw;

    return rotationMatrix;
}


#endif