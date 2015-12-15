/* Copyright(C)
 * For free
 * All right reserved
 * 
 */
/**
 * @file Utils.cpp
 * @brief 常用工具实现
 * @author highway-9, 787280310@qq.com
 * @version 1.1.0
 * @date 2015-11-28
 */

#include "Utils.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <uuid/uuid.h>
#include <openssl/crypto.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#ifndef PATH_MAX
#define PATH_MAX        1024        // 默认最大路径长度
#endif

/**********************************class String**********************************************/

std::string utils::String::int32ToString(int n)
{
    return tToString(n);
}

std::string utils::String::uint32ToString(unsigned int n)
{
    return tToString(n);
}

std::string utils::String::int64ToString(long long n)
{
    return tToString(n);
}

std::string utils::String::uint64ToString(unsigned long long n)
{
    return tToString(n);
}

std::string utils::String::floatToString(float n)
{
    return tToString(n);
}

std::string utils::String::doubleToString(double n)
{
    return tToString(n);
}

std::string utils::String::time_tToString(time_t n)
{
    struct tm* p;
    p = localtime(&n);

    p->tm_year += 1900;
    p->tm_mon++;

    char time[50] = {'\0'};
    sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d",
           p->tm_year,
           p->tm_mon,
           p->tm_mday,
           p->tm_hour,
           p->tm_min,
           p->tm_sec);

    return time;
}

bool utils::String::stringToInt32(const std::string& str, int& n)
{
    return stringToT(str, n);
}

bool utils::String::stringToUint32(const std::string& str, unsigned int& n)
{
    return stringToT(str, n);
}

bool utils::String::stringToInt64(const std::string& str, long long& n)
{
    return stringToT(str, n);
}

bool utils::String::stringToUint64(const std::string& str, unsigned long long& n)
{
    return stringToT(str, n);
}

bool utils::String::stringToFloat(const std::string& str, float& n)
{
    return stringToT(str, n);
}

bool utils::String::stringToDouble(const std::string& str, double& n)
{
    return stringToT(str, n);
}

time_t utils::String::stringToTime_t(const std::string& time)
{
    struct tm stTm;

    sscanf(time.c_str(), "%d-%d-%d %d:%d:%d",
            &(stTm.tm_year),
            &(stTm.tm_mon),
            &(stTm.tm_mday),
            &(stTm.tm_hour),
            &(stTm.tm_min),
            &(stTm.tm_sec));

    stTm.tm_year -= 1900;
    stTm.tm_mon--;
    stTm.tm_isdst = -1;

    return mktime(&stTm);
}

/**********************************class FileSystem**********************************************/

std::string utils::FileSystem::currentWorkPath()
{
    char buf[PATH_MAX] = {'\0'};
    if (getcwd(buf, sizeof (buf) - 1) != NULL)
    {
        return buf;
    }

    return "";
}

bool utils::FileSystem::setCurrentWorkPath(const std::string& path)
{
    int ret = chdir(path.c_str());
    return ret == -1 ? false : true;
}

std::string utils::FileSystem::currentExePath()
{
    char buf[PATH_MAX] = {'\0'};

    int ret = readlink("/proc/self/exe", buf, PATH_MAX);
    if (ret < 0 || ret >= PATH_MAX)
    {
        return "";
    }

    std::string path(buf);
    int pos = path.find_last_of("/");
    if (pos == -1)
    {
        return "";
    }

    path = path.substr(0, pos);

    return path;
}

std::string utils::FileSystem::currentExeName()
{
    char buf[PATH_MAX] = {'\0'};

    int ret = readlink("/proc/self/exe", buf, PATH_MAX);
    if (ret < 0 || ret >= PATH_MAX)
    {
        return "";
    }

    std::string path(buf);
    int pos = path.find_last_of("/");
    if (pos == -1)
    {
        return "";
    }

    path = path.substr(pos + 1, path.size() - 1);

    return path;
}

bool utils::FileSystem::isExists(const std::string& path)
{
    // F_OK 用于判断文件是否存在
    int ret = access(path.c_str(), F_OK);
    return ret == -1 ? false : true;
}

bool utils::FileSystem::mkdir(const std::string& path)
{
    if (path.empty())
    {
        return false;
    }

    if (utils::FileSystem::isExists(path))
    {
        return true;
    }

    std::string dirPath = path;
    if (dirPath[dirPath.size() - 1] != '/')
    {
        dirPath += '/';
    }

    for (unsigned int i = 0; i < dirPath.size(); ++i)
    {
        if (dirPath[i] == '/') 
        {
            if (i == 0)
            {
                continue;
            }

            std::string tempPath = dirPath.substr(0, i);
            if (!utils::FileSystem::isExists(tempPath))
            {
                if (::mkdir(tempPath.c_str(), 0755) == -1)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool utils::FileSystem::remove(const std::string& path)
{
    if (path.empty())
    {
        return false;
    }

    if (!utils::FileSystem::isExists(path))
    {
        return false;
    }

    int ret = ::remove(path.c_str());
    return ret == -1 ? false : true;
}

/**********************************class System**********************************************/

std::string utils::System::uuid()
{
    uuid_t uuid;
    uuid_generate(uuid);
    
    char buf[37] = {'\0'};
    uuid_unparse_upper(uuid, buf);

    return buf;
}

std::string utils::System::md5(const std::string& str)
{
    MD5_CTX hashCtx;
    unsigned char hashRet[MD5_DIGEST_LENGTH];

    int ret = MD5_Init(&hashCtx);
    if (ret == 0)
    {
        return "";
    }

    ret = MD5_Update(&hashCtx, str.c_str(), str.size());
    if (ret == 0)
    {
        return "";
    }

    ret = MD5_Final(hashRet, &hashCtx);
    if (ret == 0)
    {
        return "";
    }

    OPENSSL_cleanse(&hashCtx, sizeof (hashCtx));

    std::string md5Text;
    char buf[2] = {'\0'};

    for (int i = 0; i < MD5_DIGEST_LENGTH * 2; ++i)
    {
        memset(buf, '\0', sizeof (buf));
        if (i & 0x1)
        {
            sprintf(buf, "%x", (hashRet[i / 2]) & 0xf);
        }
        else
        {
            sprintf(buf, "%x", (hashRet[i / 2] >> 4) & 0xf);
        }
        md5Text += buf;
    }
    
    return md5Text;
}

std::string utils::System::sha1(const std::string& str)
{
    SHA_CTX hashCtx;
    unsigned char hashRet[SHA_DIGEST_LENGTH];

    int ret = SHA1_Init(&hashCtx);
    if (ret == 0)
    {
        return "";
    }

    ret = SHA1_Update(&hashCtx, str.c_str(), str.size());
    if (ret == 0)
    {
        return "";
    }

    ret = SHA1_Final(hashRet, &hashCtx);
    if (ret == 0)
    {
        return "";
    }

    OPENSSL_cleanse(&hashCtx, sizeof (hashCtx));

    std::string sha1Text;
    char buf[2] = {'\0'};

    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        memset(buf, '\0', sizeof (buf));
        sprintf(buf, "%02x", hashRet[i]);
        sha1Text += buf;
    }

    return sha1Text;
}

unsigned long utils::System::totalMemery()
{
    struct sysinfo info;
    int ret = sysinfo(&info);

    return ret == -1 ? 0 : info.totalram;
}

unsigned long utils::System::freeMemery()
{
    struct sysinfo info;
    int ret = sysinfo(&info);

    return ret == -1 ? 0 : info.freeram;
}
