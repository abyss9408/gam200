/******************************************************************************/
/*!
\file   Any.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for std::any helper functions

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <any>
#include <stdexcept>
#include <string>

namespace PopplioUtil
{
    // Template Function Definitions //

    /*
    *   try cast type to desired type
    */
    template <typename T>
    T const & AnyTo(std::any const& operand) 
    {
        try 
        {
            return std::any_cast<T const&>(operand);
        }
        catch (std::bad_any_cast const& e) 
        {
            throw std::runtime_error("Failed to convert std::any to requested type: " + std::string(e.what()));
        }
    }

    /*
    *   try cast type to desired type
    */
    template <typename T>
    T& AnyTo(std::any& operand)
    {
        try
        {
            return std::any_cast<T&>(operand);
        }
        catch (std::bad_any_cast const& e) 
        {
            throw std::runtime_error("Failed to convert std::any to requested type: " + std::string(e.what()));
        }
    }

    /*
    *   try cast type to desired class
    */
    template <class T>
    T const& AnyToC(std::any const& operand)
    {
        try
        {
            return std::any_cast<T const&>(operand);
        }
        catch (std::bad_any_cast const& e) 
        {
            throw std::runtime_error("Failed to convert std::any to requested type: " + std::string(e.what()));
        }
    }

    /*
    *   try cast type to desired class
    */
    template <class T>
    T& AnyToC(std::any& operand)
    {
        try
        {
            return std::any_cast<T&>(operand);
        }
        catch (std::bad_any_cast const& e) 
        {
            throw std::runtime_error("Failed to convert std::any to requested type: " + std::string(e.what()));
        }
    }
}
