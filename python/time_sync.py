'''
Author: xieweidong
Date: 2023-12-21
Description: 时间戳同步
已有n个激光雷达关键帧的时间戳，以及m个图像帧，m>n，为每个激光雷达帧找到时间戳最近的图像帧
要求图像的时间戳范围能够覆盖关键帧的时间戳范围
根据需要修改 np.loadtxt 中的 delimiter
需要修改时间戳所在的列，如 t,x,y,z,qx,qy,qz,qw 的情况下，t_index = 0
'''

import os
import numpy as np

def time_sync(image_files_path, timestamp_file_path):
    image_file_names = os.listdir(image_files_path)
    image_file_names.sort()
    # image_file_names.reverse()
    print("total image num: ", len(image_file_names))
    
    timestamps = np.loadtxt(timestamp_file_path, delimiter=' ', dtype=np.float64)
    print("total timestamp num: ", len(timestamps))
    
    index_image = 0
    index_stamp = 0
    t_index = 1  # timestamp 在pose文件每一行中的位置
    keyframes = []
    while index_image < len(image_file_names)-1 and index_stamp < len(timestamps):
        t1 = float(image_file_names[index_image].split('.')[0])/1000
        t2 = float(image_file_names[index_image+1].split('.')[0])/1000
        
        if t1 <= timestamps[index_stamp][t_index] and t2 > timestamps[index_stamp][t_index]:
            if abs(t1 - timestamps[index_stamp][t_index]) <= abs(t2 - timestamps[index_stamp][t_index]):
                keyframes.append(image_file_names[index_image])
            else:
                keyframes.append(image_file_names[index_image+1])
            print(image_file_names[index_image], "=>", timestamps[index_stamp][t_index]) 
            index_stamp += 1
        
        if index_stamp >= len(timestamps):
            break
        if t1 >= timestamps[index_stamp][t_index]:
            index_image -= 1
        else:
            index_image += 1
    print("total keyframe num: ", len(keyframes))
    
time_sync("image_path", "pose_path/pose.txt")
