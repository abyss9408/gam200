/******************************************************************************/
/*!
\file   Constraints.h
\author Team Popplio
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
        This is the header file for engine constraints for templates
        Uses C++20 concepts to constrain template types

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace PopplioUtil
{
    template <typename Container>
    concept ContainerConcept = requires(Container c)
    {
        typename Container::value_type; // ensure value type
    };
}