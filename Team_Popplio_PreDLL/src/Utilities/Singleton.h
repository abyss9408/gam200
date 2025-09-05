/******************************************************************************/
/*!
\file   Singleton.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for singleton pattern implementation

        Used to create and maintain a single instance of an object (class / struct / type)

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <memory>
#include <stdexcept>

namespace Popplio
{
    /*
    *   singleton pattern
    */
    template <typename T>
    class Singleton
    {
        private:
        static std::unique_ptr<T> instance; 

        protected:
        //explicit Singleton<T>() { instance = nullptr; }
        //virtual ~Singleton<T>() {}

        public:

        /*
        *   creates a single instance
        */
        static T& CreateInstance()
        {
            if (instance == nullptr)
            {
                instance = std::make_unique<T>(); 
                return *instance;
            }
            else throw std::runtime_error("Instance already exists");
        }

        /*
        *   get the singleton
        */
        static T& GetInstance()
        {
            if (instance == nullptr) throw std::runtime_error("Instance does not exist");

            return *instance;
        }

        /*
        *   resets the singleton
        */
        virtual void Destroy()
        {
            if (instance != nullptr) instance.reset();
        }

        /*
        *   check if singleton is assigned
        */
        static bool IsNull()
        {
            return instance == nullptr;
        }
    };

    // Template Function Definitions //

    template <typename T>
    std::unique_ptr<T> Singleton<T>::instance = nullptr;
}