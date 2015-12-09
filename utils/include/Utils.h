/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file Utils.h
* @brief 常用工具实现
* @author highway-9, 787280310@qq.com
* @version 0.1.0
* @date 2015-11-28
*/

#ifndef _UTILS_H
#define _UTILS_H

#include <time.h>
#include <string>
#include <sstream>

/**
* @brief 提供常用工具
*/
namespace utils
{
    /**
    * @brief 字符串与其他类型的相互转换
    */
    class String
    {
      public:
        /**
        * @brief int32ToString int类型转换成string类型
        *
        * @param n int类型
        *
        * @return string类型
        */
        static std::string int32ToString(int n);

        /**
        * @brief uint32ToString unsigned int类型转换成string类型
        *
        * @param n unsigned int类型
        *
        * @return string类型
        */
        static std::string uint32ToString(unsigned int n);

        /**
        * @brief int64ToString long long类型转换成string类型
        *
        * @param n long long类型
        *
        * @return string类型
        */
        static std::string int64ToString(long long n);

        /**
        * @brief uint64ToString unsigned long long类型转换成string类型
        *
        * @param n unsigned long long类型
        *
        * @return string类型
        */
        static std::string uint64ToString(unsigned long long n);

        /**
        * @brief floatToString float类型转换成string类型
        *
        * @param n float类型
        *
        * @return string类型
        */
        static std::string floatToString(float n);

        /**
        * @brief doubleToString double类型转换成string类型
        *
        * @param n double类型
        *
        * @return string类型
        */
        static std::string doubleToString(double n);

        /**
        * @brief time_tToString time_t类型转换成string类型
        *
        * @param n time_t类型
        *
        * @note 转换格式，默认为"%d-%d-%d %d:%d:%d"
        *
        * @return string类型
        */
        static std::string time_tToString(time_t n);

        /**
        * @brief stringToInt32 string类型转换成int类型
        *
        * @param str string类型
        * @param n int类型
        *
        * @return 成功返回true，否则返回false
        */
        static bool stringToInt32(const std::string& str, int& n);

        /**
        * @brief stringToUint32 string类型转换成unsigned int类型
        *
        * @param str string类型
        * @param n unsigned int类型
        *
        * @return 成功返回true，否则返回false
        */
        static bool stringToUint32(const std::string& str, unsigned int& n);
        
        /**
        * @brief stringToInt64 string类型转换成long long类型
        *
        * @param str string类型
        * @param n long long类型
        *
        * @return 成功返回true，否则返回false
        */
        static bool stringToInt64(const std::string& str, long long& n);

        /**
        * @brief stringToUint64 string类型转换成unsigned long long类型
        *
        * @param str string类型
        * @param n unsigned long long类型
        *
        * @return 成功返回true，否则返回false
        */
        static bool stringToUint64(const std::string& str, unsigned long long& n);

        /**
        * @brief stringToFloat string类型转换成float类型
        *
        * @param str string类型
        * @param n float类型
        *
        * @return 成功返回true，否则返回false
        */
        static bool stringToFloat(const std::string& str, float& n);

        /**
        * @brief stringToDouble string类型转换成double类型
        *
        * @param str string类型
        * @param n double类型
        *
        * @return 成功返回true，否则返回false
        */
        static bool stringToDouble(const std::string& str, double& n);

        /**
        * @brief stringToTime_t string类型转换成time_t类型
        *
        * @param time 字符串时间
        *
        * @note 时间格式:2015-11-30 20:21:34
        *
        * @return time_t表示的时间
        */
        static time_t stringToTime_t(const std::string& time);

      private:
          /**
          * @brief tToString 其他类型转换成string类型
          *
          * @tparam T 其他类型
          * @param t 其他类型
          *
          * @return string类型
          */
        template <typename T>
        static std::string tToString(T t)
        {
            std::stringstream ss;
            ss << t;
            return ss.str();
        }

        /**
        * @brief stringToT string类型转换成其他类型
        *
        * @tparam T 其他类型
        * @param str string类型
        * @param t 其他类型
        *
        * @return 成功返回true，否则返回false
        */
        template <typename T>
        static bool stringToT(const std::string& str, T& t)
        {
            std::stringstream ss;
            ss << str;
            ss >> t;
            return !ss.good();
        }
    };

    /**
    * @brief 文件、路径操作
    */
    class FileSystem
    {
      public:
        /**
        * @brief currentWorkPath 获取当前程序的工作目录
        *
        * @return 工作目录
        *
        * @note 失败将返回空字符串
        */
        static std::string currentWorkPath();

        /**
        * @brief setCurrentWorkPath 设置当前程序的工作目录
        *
        * @param path 工作目录
        *
        * @return 成功返回true，否则返回false
        */
        static bool setCurrentWorkPath(const std::string& path);

        /**
        * @brief currentExePath 获得当前可执行文件所在目录
        *
        * @return 可执行文件所在目录
        *
        * @note 失败将返回空字符串
        */
        static std::string currentExePath();

        /**
        * @brief currentExeName 获得当前可执行文件名
        *
        * @return 可执行文件名
        *
        * @note 失败将返回空字符串
        */
        static std::string currentExeName();

        /**
        * @brief isExists 判断一个文件或目录是否存在
        *
        * @param path 文件或目录路径
        *
        * @return 存在返回true，否则返回false
        */
        static bool isExists(const std::string& path);

        /**
        * @brief mkdir 创建多级目录
        *
        * @param path 路径
        *
        * @return 成功返回true，否则返回false
        */
        static bool mkdir(const std::string& path);

        /**
        * @brief remove 删除文件或目录
        *
        * @param path 路径
        *
        * @note 目录里面有内容，返回false，并且不能够递归删除目录
        *
        * @return 成功返回true，否则返回false
        */
        static bool remove(const std::string& path);
    };

    /**
    * @brief 系统相关操作
    */
    class System
    {
      public:
        /**
        * @brief uuid 获取uuid
        *
        * @return uuid字符串
        */
        static std::string uuid();

        /**
        * @brief md5 对字符串进行md5加密
        *
        * @param str 待加密的字符串
        *
        * @return 加密后的字符串
        *
        * @note 若加密失败，则返回空字符串
        */
        static std::string md5(const std::string& str);

        /**
        * @brief sha1 对字符串进行sha1加密
        *
        * @param str 待加密的字符串
        *
        * @return 加密后的字符串
        *
        * @note 若加密失败，则返回空字符串
        */
        static std::string sha1(const std::string& str);

        /**
        * @brief totalMemery 获取系统总内存大小
        *
        * @return 成功返回总内存大小，失败返回0
        */
        static unsigned long totalMemery();

        /**
        * @brief freeMemery 获取系统可用内存大小
        *
        * @return 成功返回可用内存大小，失败返回0
        */
        static unsigned long freeMemery();
    };
}

#endif
