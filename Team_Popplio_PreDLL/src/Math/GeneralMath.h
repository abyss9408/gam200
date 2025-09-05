/******************************************************************************/
/*!
\file   GeneralMath.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Header file for general math operations

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace PopplioMath
{
    /*!*****************************************************************************
    \brief
    Normalizes value

    \param[in] value
    [float const&] float to be normalized

    \return
    [int const&] Normalized value (1 / -1)
    *******************************************************************************/
    int const Normalize(float const& value);

    unsigned int const GetSrand(unsigned int const& seed);
    float const NormRand();
}