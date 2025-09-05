/******************************************************************************/
/*!
\file   FFT.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		This is the source file for the Fast Fourier Transform implementation (Audio)
		Enables features such as audio visualisation & beat detection (TODO)

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include "FFT.h"

#include <pch.h>

#include "../Math/MathLib.h"

namespace Popplio
{
	using cf = std::complex<float>;

	void FFT(PopplioMath::Polyc256& num, bool const& reverse)
	{
		short deg = num.GetDegB();
		if (deg == 1) return;

		// recursive operation

		PopplioMath::Polyc256 a0(0, deg), a1(0, deg);

		for (short i = 0; 2 * i <= deg; ++i) 
		{
			a0[2 * i] = num[2 * i];
			a1[2 * i + 1] = num[2 * i + 1];
		}

		FFT(a0, reverse);
		FFT(a1, reverse);

		float angle = 2.f * PopplioMath::M_PI / static_cast<float>(deg) * (reverse ? -1.f : 1.f);

		cf z(1.f), zn(cos(angle), sin(angle));

		for (short i = 0; 2 * i <= deg; ++i)
		{
			num[i] = a0[2 * i] + z * a1[2 * i + 1];
			num[i + deg / 2] = a0[2 * i] - z * a1[2 * i + 1];

			if (reverse)
			{
				num[i] /= 2.f;
				num[i + deg / 2] /= 2.f;
			}

			z *= zn;
		}
	}

	void FFT(PopplioMath::Polyf256& num, bool const& reverse)
	{
		static_cast<void>(num);
		static_cast<void>(reverse);
		//short deg = num.GetDegB();
		//if (deg == 1) return;

		//// recursive operation

		//PopplioMath::Polyf256 a0(0, deg), a1(0, deg);

		//for (short i = 0; 2 * i <= deg; ++i)
		//{
		//	a0[2 * i] = num[2 * i];
		//	a1[2 * i + 1] = num[2 * i + 1];
		//}

		//FFT(a0, reverse);
		//FFT(a1, reverse);

		//float angle = 2.f * PopplioMath::M_PI / static_cast<float>(deg) * (reverse ? -1.f : 1.f);

		//float z(1.f), zn(cos(angle), sin(angle));

		//for (short i = 0; 2 * i <= deg; ++i)
		//{
		//	num[i] = a0[2 * i] + z * a1[2 * i + 1];
		//	num[i + deg / 2] = a0[2 * i] - z * a1[2 * i + 1];

		//	if (reverse)
		//	{
		//		num[i] /= 2.f;
		//		num[i + deg / 2] /= 2.f;
		//	}

		//	z *= zn;
		//}
	}
}