# 打印当前网速，适用于linux和windows
import os
import time
import psutil


def get_network_speed():
    # 获取当前网络传输数据量（字节）
    bytes_sent = psutil.net_io_counters().bytes_sent
    bytes_recv = psutil.net_io_counters().bytes_recv

    # 等待1秒钟
    time.sleep(1)

    # 再次获取网络传输数据量（字节）
    new_bytes_sent = psutil.net_io_counters().bytes_sent
    new_bytes_recv = psutil.net_io_counters().bytes_recv

    # 计算每秒发送/接收的字节数
    sent_per_sec = (new_bytes_sent - bytes_sent) / 1024
    recv_per_sec = (new_bytes_recv - bytes_recv) / 1024

    os.system('cls' if os.name == 'nt' else 'clear')
    if sent_per_sec < 1024:
        sent_per_sec_str = "\033[93m\u2191\033[0m: {:.2f} \033[93mKB/s\033[0m".format(sent_per_sec)
    else:
        sent_per_sec_str = "\033[93m\u2191\033[0m: {:.2f} \033[92mMB/s\033[0m".format(sent_per_sec/1024)
    if recv_per_sec < 1024:
        recv_per_sec_str = "\033[92m\u2193\033[0m: {:.2f} \033[93mKB/s\033[0m".format(recv_per_sec)
    else:
        recv_per_sec_str = "\033[92m\u2193\033[0m: {:.2f} \033[92mMB/s\033[0m".format(recv_per_sec/1024)
    print(sent_per_sec_str, recv_per_sec_str)
    print()


if __name__ == "__main__":
    while True:
        # 调用函数开始显示实时网速
        get_network_speed()
