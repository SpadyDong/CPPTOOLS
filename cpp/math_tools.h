#ifndef _MATH_TOOLS_H
#define _MATH_TOOLS_H

/*
一些常用的数学操作
*/

#include <Eigen/Dense>
#include <pcl/point_cloud.h>

// 计算pcl点云中点的深度
template<typename T>
float pointDepth(T p)
{
    float depth = std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    return depth;
}

// 一个四元数（实部和虚部） => 一个3x3的旋转矩阵
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

// XYZ + 四元数 => 位姿矩阵
Eigen::Matrix4d poseToMatrix(double x, double y, double z, double w, double qx, double qy, double qz)
{
    Eigen::Matrix4d poseMatrix = Eigen::Matrix4d::Identity();

    Eigen::Matrix3d rotationMatrix = quaternionToRotationMatrix(w, qx, qy, qz);

    poseMatrix.block<3, 3>(0, 0) = rotationMatrix;
    poseMatrix(0, 3) = x;
    poseMatrix(1, 3) = y;
    poseMatrix(2, 3) = z;

    return poseMatrix;
}

// XYZ + 四元数 => 位姿矩阵
Eigen::Matrix4d poseToMatrix2(double x, double y, double z, double w, double qx, double qy, double qz)
{
    Eigen::Quaterniond qvl(w, qx, qy, qz);
    Eigen::Vector3d tvl(x, y, z);
    Eigen::Isometry3d Tvr(qvl);
    Tvr.pretranslate(tvl);

    return Tvr.matrix();
}


#endif
