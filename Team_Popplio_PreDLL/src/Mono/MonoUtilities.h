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

#pragma once

#include "MonoAPI.h"

#include <mono/metadata/class.h>
#include <mono/metadata/object.h>
#include <mono/metadata/metadata.h>

namespace PopplioScriptAPI
{
    void MonoUtilInit(void* api);

    std::string MonoToString(MonoString* monoString);

    std::string& MonoToString(MonoString* monoString, std::string& str);

    std::string MonoToString(MonoObject* monoString);

    std::string& MonoToString(MonoObject* monoString, std::string& str);

    MonoString* StringToMono(std::string const& str);

    MonoString* StringToMono(std::string& str, MonoString* monoString);

    char UInt16ToChar(uint16_t val);

    uint16_t CharToUInt16(char val);

    // ================================================================================ //

    template <typename T>
    MonoArray* ArrayToMono2D(T** arr, unsigned int rows, unsigned int cols, MonoDomain* appDomain);

    // Specializations //

    template<>
    MonoArray* ArrayToMono2D<float>(float** arr, unsigned int rows, unsigned int cols, MonoDomain* appDomain);

    // ================================================================================ //

    //template <typename T>
    //MonoArray* ArrayToMono1D(T* arr, unsigned int size, MonoDomain* appDomain);

    template <typename T>
    MonoArray* ArrayToMono1D(std::vector<T> arr, MonoDomain* appDomain);

    //template <typename T>
    //T* MonoToArray1D(MonoArray* arr, MonoDomain* appDomain);

    template <typename T>
    std::vector<T> MonoToArray1D(MonoArray* arr, MonoDomain* appDomain);

    // Specializations //

    template<>
    MonoArray* ArrayToMono1D<int>(std::vector<int> arr, MonoDomain* appDomain);

    template<>
    std::vector<int> MonoToArray1D<int>(MonoArray* arr, MonoDomain* appDomain);

    // ================================================================================ //

    template <typename T>
    MonoArray* ArrayToMono(T* arr, std::initializer_list<unsigned int> dimensions, MonoDomain* appDomain);

    template <typename T>
    T* MonoToArray(MonoArray* arr, MonoDomain* appDomain);
}

#include "MonoUtilitiesTemplates.h"