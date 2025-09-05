/******************************************************************************/
/*!
\file   FFT.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/13 08:55:22 PM (Last Edit)
\brief
		This is the header file for the Fast Fourier Transform implementation (Audio)
		Enables features such as audio visualisation & beat detection (TODO)

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "../Math/MathLib.h"

#include <complex>
#include <vector>

namespace Popplio
{
	// using cd = std::complex<double>;

	// pass in polynomial as cf array (Polyc256)
	/*
	*   TODO
	*/
	void FFT(PopplioMath::Polyc256& num, bool const& reverse = false);

	// pass in polynomial as float array (Polyf256)
	/*
	*   TODO
	*/
	void FFT(PopplioMath::Polyf256& num, bool const& reverse = false);
}