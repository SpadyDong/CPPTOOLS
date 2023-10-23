import numpy as np


# 视差转深度
def disparity2depth(disparity, baselinefx):
    # KITTI: baselinefx = abs(calib['P2'][0][3]-calib['P3'][0][3])
    # 通常情况下双目相机的摆放是齐平的，baselinefx的值为fx*(t0x-t1x)
    depth = np.full(disparity.shape, 0.0)
    for i in range(disparity.shape[0]):
        for j in range(disparity.shape[1]):
            depth[i, j] = baselinefx / disparity[i, j]
    return depth


# 深度转点云
def depth2pc(fx, fy, cx, cy, depth):
    h, w = depth.shape
    x = np.linspace(0, w - 1, w)
    y = np.linspace(0, h - 1, h)
    x, y = np.meshgrid(x, y)

    x = -(x - cx) * depth / fx
    y = -(y - cy) * depth / fy

    x = x.flatten()[:, np.newaxis]
    y = y.flatten()[:, np.newaxis]
    z = depth.flatten()[:, np.newaxis]

    points = np.concatenate((z, x, y), axis=1)

    return points


# 点云转range image
def point2RI(points, proj_H, proj_W, fov_up, fov_down, max_range, view_angle):
    fov_up = fov_up / 180.0 * np.pi
    fov_down = fov_down / 180.0 * np.pi
    fov = abs(fov_down) + abs(fov_up)

    points = points[:, 0:3]
    depth = np.linalg.norm(points, 2, axis=1)
    points = points[(depth > 0) & (depth < max_range)]
    depth = depth[(depth > 0) & (depth < max_range)]

    yaw = -np.arctan2(points[:, 1], points[:, 0]) * (180 / view_angle)
    pitch = np.arcsin(points[:, 2] / depth)

    proj_x = 0.5 * (yaw / np.pi + 1.0) * proj_W
    proj_y = (1.0 - (pitch + abs(fov_down)) / fov) * proj_H
    proj_x, proj_y = np.floor(proj_x), np.floor(proj_y)
    proj_x, proj_y = np.minimum(proj_W - 1, proj_x), np.minimum(proj_H - 1, proj_y)
    proj_x, proj_y = np.maximum(0, proj_x).astype(np.int32), np.maximum(0, proj_y).astype(np.int32)
    order = np.argsort(depth)[::-1]
    depth = depth[order]
    proj_x = proj_x[order]
    proj_y = proj_y[order]

    proj_range = np.full((proj_H, proj_W), 0, dtype=np.float32)
    proj_range[proj_y, proj_x] = depth

    return proj_range


# 点云转polar BEV
def point2polarBEV(points, proj_H, proj_W, max_range, view_angle):
    points = points[:, 0:3]
    depth = np.linalg.norm(points, 2, axis=1)
    points = points[(depth > 0) & (depth < max_range)]
    depth = depth[(depth > 0) & (depth < max_range)]

    yaw = -np.arctan2(points[:, 1], points[:, 0]) * (180 / view_angle)
    pitch = np.arcsin(points[:, 2] / depth)

    scan_r = depth * np.cos(pitch)
    proj_x = 0.5 * (yaw / np.pi + 1.0)
    proj_y = scan_r / max_range
    proj_x = proj_x * proj_W
    proj_y = proj_y * proj_H

    proj_x = np.floor(proj_x)
    proj_x = np.minimum(proj_W - 1, proj_x)
    proj_x = np.maximum(0, proj_x).astype(np.int32)

    proj_y = np.floor(proj_y)
    proj_y = np.minimum(proj_H - 1, proj_y)
    proj_y = np.maximum(0, proj_y).astype(np.int32)

    order = np.argsort(points[:, 2])
    points[:, 2] = points[:, 2][order]
    proj_x = proj_x[order]
    proj_y = proj_y[order]

    kitti_lidar_height = 3
    proj_bev = np.full((proj_H, proj_W), 0, dtype=np.float32)
    proj_bev[proj_y, proj_x] = points[:, 2] + abs(kitti_lidar_height)
    return proj_bev


def point2BEV(pc, width, scale):
    cloud2d = pc[:, 0:2] * scale + width // 2
    cloud2d = cloud2d[(cloud2d[:, 0] >= 0) & (cloud2d[:, 0] < width) & (cloud2d[:, 1] >= 0) & (cloud2d[:, 1] < width)]
    cloud2d = np.floor(cloud2d).astype(np.int32)
    image = np.full((width, width), 0, dtype=np.float32)
    image[cloud2d[:, 0], cloud2d[:, 1]] = 255
