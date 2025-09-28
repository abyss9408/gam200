/******************************************************************************/
/*!
\file   Directory.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for filesystem utilities

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

#include "Directory.h"

#include "../Engine/Engine.h"

#include <filesystem>

namespace PopplioUtil
{
    void ResetCurrentDirectory()
    {
        std::filesystem::current_path(std::filesystem::path(Popplio::Engine::programPath));
    }

    std::string GetAppDataPath()
    {
        char* appdata = nullptr;
        size_t len = 0;
        std::string str = "";

        // get environment variable
        if (_dupenv_s(&appdata, &len, "APPDATA") == 0 && appdata != nullptr) 
        {
            str = std::string(appdata);
            Popplio::Logger::Info("PopplioUtil::GetAppDataPath() | APPDATA = " + str);
            
            free(appdata); // must free when done
        }
        else 
        {
            Popplio::Logger::Error("PopplioUtil::GetAppDataPath() | APPDATA not found!");
        }

        return str;
    }

    std::string GetTeamPopplioUserPath()
    {
        return GetAppDataPath() + "\\TeamPopplio";
    }

    bool CreateFile(std::string const& path)
    {
        std::string dir = path;
        dir.replace(path.find_last_of('\\'), path.length() - path.find_last_of('\\') + 1, "");

        //std::cout << dir << std::endl;

        try 
        {
            if (std::filesystem::create_directories(dir))
                Popplio::Logger::Info("PopplioUtil::CreateFile() | Directory created: " + dir);
            else Popplio::Logger::Info("PopplioUtil::CreateFile() | Directory already exists: " + dir);
        }
        catch (const std::filesystem::filesystem_error& e) 
        {
            Popplio::Logger::Error("PopplioUtil::CreateFile() | Error: " + std::string(e.what()));
        }

        std::ofstream file(path);

        if (!file)
        {
            Popplio::Logger::Error("PopplioUtil::CreateFile() | Error creating file " + path);
            return false;
        }

        file.close();

        Popplio::Logger::Info("PopplioUtil::CreateFile() | Created file " + path);

        return true;
    }

    bool FileExists(std::string const& path)
    {
        return std::filesystem::exists(path);
    }
}