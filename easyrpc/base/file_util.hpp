#ifndef _FILE_UTIL_H
#define _FILE_UTIL_H

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#ifndef PATH_MAX
#define PATH_MAX 1024      
#endif

namespace easyrpc
{

class file_util
{
public:
    static std::string current_work_path()
    {
#ifdef _WIN32
        char buf[MAX_PATH];
        GetCurrentDirectoryA(sizeof(buf), buf);  
        return buf;
#else
        char buf[PATH_MAX] = {'\0'};
        if (getcwd(buf, sizeof (buf) - 1) != nullptr)
        {
            return buf;
        }
        return "";
#endif
    }

    static bool set_current_work_path(const std::string& path)
    {
#ifdef _WIN32
        BOOL ret =  SetCurrentDirectoryA(path.c_str());  
        return ret == TRUE ? true : false;
#else
        int ret = chdir(path.c_str());
        return ret == -1 ? false : true;
#endif
    }

    static std::string current_exe_path()
    {
#ifdef _WIN32
        char path[MAX_PATH + 1] = { '\0' };
        ::GetModuleFileNameA(NULL, path, MAX_PATH);
        (strrchr(path, '\\'))[1] = 0;
        return path;
#else
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

        return path.substr(0, pos);
#endif
    }

    static std::string current_exe_name()
    {
#ifdef _WIN32
        char buf[MAX_PATH + 1] = {'\0'};
        ::GetModuleFileNameA(NULL, buf, MAX_PATH);
        std::string path(buf);
        int pos = path.find_last_of("\\");
        if (pos == -1)
        {
            return "";
        }

        path = path.substr(pos + 1, path.size() - 1);
        pos = path.find_first_of(".");
        if (pos == -1)
        {
            return "";
        }
        return path.substr(0, pos);
#else
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

        return path.substr(pos + 1, path.size() - 1);
#endif
    }

    static bool is_exists(const std::string& path)
    {
#ifdef _WIN32
        int ret = _access(path.c_str(), 0);
#else
        int ret = access(path.c_str(), F_OK);
#endif
        return ret == 0 ? true : false;
    }

    static bool mkdir(const std::string& path)
    {
        if (path.empty())
        {
            return false;
        }

        if (is_exists(path))
        {
            return true;
        }

        std::string dir_path = path;
        if (dir_path[dir_path.size() - 1] != '/' || dir_path[dir_path.size() - 1] != '\\')
        {
            dir_path += '/';
        }

        for (std::size_t i = 0; i < dir_path.size(); ++i)
        {
            if (dir_path[i] == '/' || dir_path[i] == '\\')
            {
                if (i == 0)
                {
                    continue;
                }

                std::string temp_path = dir_path.substr(0, i);
                if (!is_exists(temp_path))
                {
#ifdef _WIN32
                    if (_mkdir(temp_path.c_str()) != 0)
#else
                    if (::mkdir(temp_path.c_str(), 0755) != 0)
#endif
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    static bool remove(const std::string& path)
    {
        if (path.empty())
        {
            return false;
        }

        if (!is_exists(path))
        {
            return false;
        }

        int ret = ::remove(path.c_str());
        return ret == 0 ? true : false;
    }
};

}

#endif
