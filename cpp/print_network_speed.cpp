# 实时打印当前网速，适用于linux
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

void clearScreen() {
    // 使用系统命令清屏
    system(CLEAR_COMMAND);
}

class NetworkSpeedMonitor {
public:
    NetworkSpeedMonitor(const std::string& interfaceName) : interfaceName_(interfaceName) {}

    void printCurrentSpeed() {
        std::cout << std::fixed << std::setprecision(2);

        unsigned long long rxBytesPrev = getRxBytes();
        unsigned long long txBytesPrev = getTxBytes();

        std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待一秒

        unsigned long long rxBytesCurr = getRxBytes();
        unsigned long long txBytesCurr = getTxBytes();

        unsigned long long rxSpeed = rxBytesCurr - rxBytesPrev;
        unsigned long long txSpeed = txBytesCurr - txBytesPrev;

        clearScreen();

        std::cout << "Interface: " << "\033[34m" << interfaceName_ << "\033[0m" << "\t";

        // download
        if(rxSpeed < 1024)
        {
            std::cout << u8"\033[32m\u2193\033[0m " << rxSpeed << " \033[33mbytes/s\033[0m\t";
        }
        else
        {
            float rxSpeed_f = rxSpeed /1024.0;
            if(rxSpeed_f < 1024)
            {
                std::cout << u8"\033[32m\u2193\033[0m " << rxSpeed_f << " \033[32mKB/s\033[0m\t";
            }
            else
            {
                std::cout << u8"\033[32m\u2193\033[0m " << rxSpeed_f / 1024 << " \033[31mMB/s\033[0m\t";
            }
        }
        
        // upload
        if(txSpeed < 1024)
        {
            std::cout << u8"\033[33m\u2191\033[0m " << txSpeed << " \033[33mbytes/s\033[0m" << std::endl;
        }
        else
        {
            float txSpeed_f = txSpeed /1024.0;
            if(txSpeed_f < 1024)
            {
                std::cout << u8"\033[33m\u2191\033[0m " << txSpeed_f << " \033[32mKB/s\033[0m" << std::endl;
            }
            else
            {
                std::cout << u8"\033[33m\u2191\033[0m " << txSpeed_f / 1024 << " \033[32mMB/s\033[0m" << std::endl;
            }
        }
        
    }

private:
    unsigned long long getRxBytes() const {
        return getInterfaceBytes("rx_bytes");
    }

    unsigned long long getTxBytes() const {
        return getInterfaceBytes("tx_bytes");
    }

    unsigned long long getInterfaceBytes(const std::string& type) const {
        std::ifstream file("/sys/class/net/" + interfaceName_ + "/statistics/" + type);
        if (!file.is_open()) {
            std::cerr << "\033[31mError opening file for interface: " << interfaceName_ << "\033[0m" << ".\nCheck NIC name with \"ifconfig\"." << std::endl;
            return 0;
        }

        unsigned long long bytes;
        file >> bytes;
        file.close();

        return bytes;
    }

private:
    std::string interfaceName_;
};

int main() {
    NetworkSpeedMonitor monitor("ens33");

    // 打印当前网速
    while(true)
    {
        monitor.printCurrentSpeed();
    }
    return 0;
}
