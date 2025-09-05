/******************************************************************************/
/*!
\file   MonoUtilities.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for Mono-related utility functions

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// MonoUtilities.h

namespace PopplioScriptAPI
{
    template <typename T>
    MonoArray* ArrayToMono2D(T** arr, unsigned int rows, unsigned int cols, MonoDomain* appDomain)
    {
        Popplio::Logger::Warning("MonoUtilities.h | ArrayToMono2D<T> : Type not supported");

        return nullptr;

        //try
        //{
        //    MonoClass* typeClass = mono_get_single_class();
        //    MonoClass* arrayClass = mono_array_class_get(typeClass, 2); // 2D

        //    MonoArray* monoArray = mono_array_new(appDomain, arrayClass, rows);

        //    for (unsigned int i = 0; i < rows; ++i)
        //    {
        //        for (unsigned int j = 0; j < cols; ++j)
        //        {
        //            mono_array_set(monoArray, MonoArray*, i, mono_array_new(appDomain, typeClass, cols));
        //            MonoArray* r = mono_array_get(monoArray, MonoArray*, i);
        //            mono_array_set(r, T, j, arr[i][j]);
        //        }
        //    }

        //    return monoArray;
        //}
        //catch (std::exception const& e)
        //{
        //    std::stringstream ss{ "" };
        //    ss << "MonoUtilities.h | ArrayToMono2D : " << e.what();
        //    Popplio::Logger::Error(ss.str());

        //    return nullptr;
        //}
    }

    // ================================================================================ //

    //template <typename T>
    //MonoArray* ArrayToMono1D(T* arr, unsigned int size, MonoDomain* appDomain)
    //{
    //    Popplio::Logger::Warning("MonoUtilities.h | ArrayToMono1D<T> : Type not supported");
    //}

    template <typename T>
    MonoArray* ArrayToMono1D(std::vector<T> arr, MonoDomain* appDomain)
    {
        //Popplio::Logger::Warning("MonoUtilities.h | ArrayToMono1D<T> : Type not supported");

        MonoClass* dataType = nullptr;

        if constexpr (std::is_same_v<T, int>)               dataType = mono_get_int32_class();
        else if constexpr (std::is_same_v<T, float>)        dataType = mono_get_single_class();
        else if constexpr (std::is_same_v<T, double>)       dataType = mono_get_double_class();
        else if constexpr (std::is_same_v<T, char>)         dataType = mono_get_char_class();
        else if constexpr (std::is_same_v<T, bool>)         dataType = mono_get_boolean_class();
        else if constexpr (std::is_same_v<T, std::string>)  dataType = mono_get_string_class();
        else if constexpr (std::is_same_v<T, MonoObject*>)  dataType = mono_get_object_class();
        else if constexpr (std::is_same_v<T, intptr_t>)     dataType = mono_get_intptr_class();
        else
        {
            Popplio::Logger::Warning("MonoUtilities.h | ArrayToMono1D<T> : Type not supported");
            return nullptr;
        }

        try
        {
            MonoArray* monoArray = mono_array_new(appDomain, dataType, arr.size());
            for (size_t i = 0; i < arr.size(); i++)
            {
                if constexpr (std::is_same_v<T, int>)                 mono_array_set(monoArray, int, i, arr[i]);
                else if constexpr (std::is_same_v<T, float>)          mono_array_set(monoArray, float, i, arr[i]);
                else if constexpr (std::is_same_v<T, double>)         mono_array_set(monoArray, double, i, arr[i]);
                else if constexpr (std::is_same_v<T, char>)           mono_array_set(monoArray, uint16_t, i, CharToUInt16(arr[i]));
                else if constexpr (std::is_same_v<T, bool>)           mono_array_set(monoArray, bool, i, arr[i]);
                else if constexpr (std::is_same_v<T, std::string>)    mono_array_set(monoArray, MonoString*, i, StringToMono(arr[i]));
                else if constexpr (std::is_same_v<T, MonoObject*>)    mono_array_set(monoArray, MonoObject*, i, arr[i]);
                else if constexpr (std::is_same_v<T, intptr_t>)       mono_array_set(monoArray, intptr_t, i, arr[i]);
            }
            return monoArray;
        }
        catch (std::exception const& e)
        {
            std::stringstream ss{ "" };
            ss << "MonoUtilities.h | ArrayToMono1D : " << e.what();
            Popplio::Logger::Error(ss.str());

            return nullptr;
        }
    }

    //template <typename T>
    //T* MonoToArray1D(MonoArray* arr, MonoDomain* appDomain)
    //{
    //    Popplio::Logger::Warning("MonoUtilities.h | MonoToArray1D<T> : Type not supported");
    //}

    template <typename T>
    std::vector<T> MonoToArray1D(MonoArray* arr, MonoDomain* appDomain)
    {
        //Popplio::Logger::Warning("MonoUtilities.h | MonoToArray1D<T> : Type not supported");
        static_cast<void>(appDomain);
        try
        {
            uintptr_t length = mono_array_length(arr);
            std::vector<T> val(length);
            for (uintptr_t i = 0; i < length; i++)
            {
                if constexpr (std::is_same_v<T, int>)              val[i] = mono_array_get(arr, int, i);
                else if constexpr (std::is_same_v<T, float>)       val[i] = mono_array_get(arr, float, i);
                else if constexpr (std::is_same_v<T, double>)      val[i] = mono_array_get(arr, double, i);
                else if constexpr (std::is_same_v<T, char>)        val[i] = UInt16ToChar(mono_array_get(arr, uint16_t, i));
                else if constexpr (std::is_same_v<T, bool>)        val[i] = mono_array_get(arr, bool, i);
                else if constexpr (std::is_same_v<T, std::string>) val[i] = MonoToString(mono_array_get(arr, MonoString*, i));
                else if constexpr (std::is_same_v<T, MonoObject*>) val[i] = mono_array_get(arr, MonoObject*, i);
                else if constexpr (std::is_same_v<T, intptr_t>)    val[i] = mono_array_get(arr, intptr_t, i);
                else
                {
                    Popplio::Logger::Warning("MonoUtilities.h | MonoToArray1D<T> : Type not supported");
                    return std::vector<T>();
                }
            }
            return val;
        }
        catch (std::exception const& e)
        {
            std::stringstream ss{ "" };
            ss << "MonoUtilities.h | MonoToArray1D | int : " << e.what();
            Popplio::Logger::Error(ss.str());

            return std::vector<T>();
        }
    }

    // ================================================================================ //

    template <typename T>
    MonoArray* ArrayToMono(T* arr, std::initializer_list<unsigned int> dimensions, MonoDomain* appDomain)
    {
        Popplio::Logger::Warning("MonoUtilities.h | ArrayToMono<T> : Not implemented");
    }

    template <typename T>
    T* MonoToArray(MonoArray* arr, MonoDomain* appDomain)
    {
        Popplio::Logger::Warning("MonoUtilities.h | MonoToArray<T> : Not implemented");
    }
}