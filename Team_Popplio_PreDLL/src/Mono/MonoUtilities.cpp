/******************************************************************************/
/*!
\file   MonoUtilities.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the source file for Mono-related utility functions

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h>

//#include <mono/eglib/glib.h>

#include "MonoUtilities.h"

namespace PopplioScriptAPI
{
    // temp buffers
    namespace
    {
        static char* charPtrMS = nullptr;
        static std::string strMS = "";
        static MonoAPI* monoAPI = nullptr;
    }

    void MonoUtilInit(void* api)
    {
        monoAPI = static_cast<MonoAPI*>(api);
    }

    std::string MonoToString(MonoString* monoString)
    {
        charPtrMS = mono_string_to_utf8(monoString);
        strMS = charPtrMS;
        MonoAPI::MonoFree(charPtrMS);
        //mono_free(charPtrMS);
        return strMS;
    }

    std::string& MonoToString(MonoString* monoString, std::string& str)
    {
        charPtrMS = mono_string_to_utf8(monoString);
        str = charPtrMS;
        MonoAPI::MonoFree(charPtrMS);
        //mono_free(charPtrMS);
        return str;
    }

    std::string MonoToString(MonoObject* monoString)
    {
        charPtrMS = mono_string_to_utf8((MonoString*)monoString);
        strMS = charPtrMS;
        MonoAPI::MonoFree(charPtrMS);
        //mono_free(charPtrMS);
        return strMS;
    }

    std::string& MonoToString(MonoObject* monoString, std::string& str)
    {
        charPtrMS = mono_string_to_utf8((MonoString*)monoString);
        str = charPtrMS;
        MonoAPI::MonoFree(charPtrMS);
        //mono_free(charPtrMS);
        return str;
    }

    MonoString* StringToMono(std::string const& str)
    {
        return monoAPI->CreateMonoString(mono_domain_get(), str.c_str());
    }

    MonoString* StringToMono(std::string& str, MonoString* monoString)
    {
        return monoString = monoAPI->CreateMonoString(mono_domain_get(), str.c_str());
    }

    char UInt16ToChar(uint16_t val)
    {
        return static_cast<char>(static_cast<wchar_t>(val));
    }

    uint16_t CharToUInt16(char val)
    {
        return static_cast<uint16_t>(static_cast<wchar_t>(val));
    }

    // ================================================================================ //

    // Specializations //

    template<>
    MonoArray* ArrayToMono2D<float>(float** arr, unsigned int rows, unsigned int cols, MonoDomain* appDomain)
    {
        try
        {
            // jagged array [][]
            
            MonoArray* arrRows = mono_array_new(appDomain, mono_get_array_class(), rows);

            for (unsigned int i = 0; i < rows; i++)
            {
                MonoArray* arrCols = mono_array_new(appDomain, mono_get_single_class(), cols);

                for (unsigned int j = 0; j < cols; j++)
                {
                    mono_array_set(arrCols, float, j, arr[i][j]);
                }

                mono_array_set(arrRows, MonoArray*, i, arrCols);
            }

            return arrRows;

            // true rectangle array [,]
            
            //MonoClass* arrayClass = mono_array_class_get(mono_get_int32_class(), 2);

            //// Create the array
            //MonoArray* monoArray = mono_array_new(appDomain, arrayClass, rows * cols);

            //// Fill array
            //for (int i = 0; i < rows; i++) 
            //{
            //    for (int j = 0; j < cols; j++) 
            //    {
            //        mono_array_set(monoArray, int, i * cols + j, arr[i][j]);
            //    }
            //}

            //return monoArray;
        }
        catch (std::exception const& e)
        {
            std::stringstream ss{ "" };
            ss << "MonoUtilities.h | ArrayToMono2D<T> | float : " << e.what();
            Popplio::Logger::Error(ss.str());

            return nullptr;
        }
    }

    // ================================================================================ //

    // Specializations //

    template<>
    MonoArray* ArrayToMono1D<int>(std::vector<int> arr, MonoDomain* appDomain)
    {
        try
        {
            MonoArray* monoArray = mono_array_new(appDomain, mono_get_int32_class(), arr.size());
            for (size_t i = 0; i < arr.size(); i++)
            {
                mono_array_set(monoArray, int, i, arr[i]);
            }
            return monoArray;
        }
        catch (std::exception const& e)
        {
            std::stringstream ss{ "" };
            ss << "MonoUtilities.h | ArrayToMono1D | int : " << e.what();
            Popplio::Logger::Error(ss.str());

            return nullptr;
        }
    }

    template<>
    std::vector<int> MonoToArray1D<int>(MonoArray* arr, MonoDomain* appDomain)
    {
        (void)appDomain;

        try
        {
            uintptr_t length = mono_array_length(arr);
            std::vector<int> val(length);
            for (uintptr_t i = 0; i < length; i++)
            {
                val[i] = mono_array_get(arr, int, i);
            }
            return val;
        }
        catch (std::exception const& e)
        {
            std::stringstream ss{ "" };
            ss << "MonoUtilities.h | MonoToArray1D | int : " << e.what();
            Popplio::Logger::Error(ss.str());

            return std::vector<int>();
        }
    }
}