/******************************************************************************/
/*!
\file   Polynomial.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		This is the source file for the Polynomial classes that have mathematical polynomial functionality:
		Polynomial evaluation, arithmetic, and modification to polynomials

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h> // PCH
#include "Polynomial.h" // Header
#include <stdexcept>

namespace PopplioMath
{
	// Mathematical polynomial (float w/ deg 0 - 255)

	/********************************************************************************/
	/*!	
	*	\brief
	*	Default constructor for Polyf256 - Creates zero polynomial
	*/	
	/********************************************************************************/
	Polyf256::Polyf256() : expr(), degA(0), degB(0) {}

	/********************************************************************************/
	/*!
	*	\brief
	*	Argument constructor for Polyf256 to construct degree a to b polynomial
	*
	*	\param[in] a
	*	[short const &] Degree a
	*
	*	\param[in] b
	*	[short const &] Degree b
	*/
	/********************************************************************************/
	Polyf256::Polyf256(short const& a, short const& b) : expr(), degA(a), degB(b) {}

	/********************************************************************************/
	/*!	
	*	\brief
	*	Argument constructor for Polyf256 to construct from another same degree polynomial
	*
	*	\param[in] poly
	*	[Polyf256 const &] Other polynomial of same degree
	*/	
	/********************************************************************************/
	Polyf256::Polyf256(Polyf256 const& poly) : expr(), degA(poly.degA), degB(poly.degB) // arg constructor
	{
		for (size_t i = ToIndex(degA); i < ToIndex(degB); ++i) // copy coefficient values
		{
			expr[i] = poly[static_cast<short>(i) + degA];
		}
	}

	/********************************************************************************/
	/*!	
	*	\brief
	*	Copy Assignment Operator - Assigns another polynomial to this polynomial (Both with same degree)
	*
	*	\param[in] poly
	*	[Polyf256 const &] Other polynomial of same degree
	*
	*	\return 
	*	[Polyf256 const &] This polynomial after assignment
	*/	
	/********************************************************************************/
	Polyf256 const & Polyf256::operator=(Polyf256 const &poly) // copy assign
	{
		if (poly.degA != degA || poly.degB != degB)
			throw std::runtime_error("Polyf256: Copy assignment received different polynomial");

		for (size_t i = ToIndex(degA); i < ToIndex(degB); ++i) // copy coefficient values
		{
			expr[i] = poly[static_cast<short>(i) + degA];
		}

		return *this;
	}

	/********************************************************************************/
	/*!	
	*	\brief
	*	operator() - Evaluate polynomial with input
	*
	*	\param[in] value
	*	[T const &] Value to input polynomial function with
	*
	*	\return 
	*	[T const] Solution to polynomial with given value
	*/	
	/********************************************************************************/
	float const Polyf256::operator()(float const &value) const // Evaluate polynomial
	{
		float result{};

		for (size_t i = ToIndex(degA); i <= ToIndex(degB); ++i) // for each deg
		{
			if (i > ToIndex(0)) // more than 0 deg
			{
				float var{value};
				for (size_t j = ToIndex(0); j < i; ++j) var *= value; // power

				result += expr[i] * var; // add to result

				continue;
			}
			
			if (i < ToIndex(0)) // less than 0 deg
			{
				float var{ value };
				for (size_t j = ToIndex(degA); j < ToIndex(0); ++j) var *= 1.f / value; // power

				result += expr[i] * var; // add to result

				continue;
			}
			
			result += expr[i]; // add constant to result
		}

		return result;
	}

	/********************************************************************************/
	/*!	
	*	\brief
	*	operator[] - Get coefficient at given degree
	*
	*	\param[in] degree
	*	[float const &] Degree to get coefficient from
	*
	*	\return 
	*	[float const &] Coefficient value at given degree
	*/	
	/********************************************************************************/
	float const & Polyf256::operator[](short const &degree) const // Const coefficient
	{
		return expr[ToIndex(degree)];
	}
	/********************************************************************************/
	/*!
	*	\brief
	*	operator[] - Set coefficient at given degree
	*
	*	\param[in] degree
	*	[float const &] Degree to set coefficient at
	*
	*	\return
	*	[float const &] Coefficient value at given degree
	*/
	/********************************************************************************/
	float& Polyf256::operator[](short const &degree) // Modifiable coefficient
	{
		return expr[ToIndex(degree)];
	}

	/********************************************************************************/
	/*!	
	*	\brief
	*	operator* - Evaluates product of lhs polynomial and rhs polynomial
	*
	*	\param[in] rhs
	*	[Polyf256 const &] RHS polynomial
	*
	*	\return 
	*	[Polyf256 const] Product of LHS and RHS
	*/	
	/********************************************************************************/
	Polyf256 const Polyf256::operator*(Polyf256 const &rhs) // Multiply with another polynomial
	{
		Polyf256 result(degA + rhs.degA, degB + rhs.degB);

		for (short i = degA; i < 0 && i <= degB; ++i) // for each lhs deg
		{
			for (short j = rhs.degA; j < 0 && i <= rhs.degB; ++j) // for each rhs deg
			{
				result[i + j] += (*this)[i] * rhs[j]; // calculate coefficient per deg
			}
		}

		for (short i = (degA <= 0 ? 0 : degA); i <= degB; ++i) // for each lhs deg
		{
			for (short j = (rhs.degA <= 0 ? 0 : rhs.degA); j <= rhs.degB; ++j) // for each rhs deg
			{
				result[i + j] += (*this)[i] * rhs[j]; // calculate coefficient per deg
			}
		}

		return result;
	}

    /********************************************************************************/
    /*!	
    *	\brief
    *	operator<< - Outputs polynomial information to output stream
    *
    *	\param[in] out
    *	[std::ostream &] Reference to output stream
    *
    *	\param[in] pol
    *	[Polyf256 const &] Polyf256 to print info about
    *
    *	\return 
    *	[std::ostream &] Output stream
    */	
    /********************************************************************************/
	std::ostream& operator<<(std::ostream &out, Polyf256 const& pol) 
	{
		for (short i = pol.GetDegA(); i < 0 && i <= pol.GetDegB(); ++i)
		{
			if (pol[i] == 0) continue; // skip terms with zero coefficients

			if (pol[i] > 0) out << "+";
			else out << "-";

			if (pol[i] != 1) out << pol[i];

			out << "x^" << i;

			if (i < -1) out << " ";
		}

		if (pol.GetDegA() <= 0 && pol.GetDegB() >= 0) out << pol[0] << " ";

		for (short i = (pol.GetDegA() <= 1 ? 1 : pol.GetDegA()); i <= pol.GetDegB(); ++i)
		{
			if (pol[i] == 0) continue; // skip terms with zero coefficients

			if (i != 1) out << " ";

			if (pol[i] > 0) out << "+";
			else out << "-";

			if (pol[i] != 1) out << pol[i];

			out << "x^" << i;
		}

		return out;
	}

	// ****************************************************************************** //

	// Mathematical polynomial (complex<float> w/ deg 0 - 255)

	/********************************************************************************/
	/*!
	*	\brief
	*	Default constructor for Polyc256 - Creates zero polynomial
	*/
	/********************************************************************************/
	Polyc256::Polyc256() : expr(), degA(0), degB(0) {}

	/********************************************************************************/
	/*!
	*	\brief
	*	Argument constructor for Polyc256 to construct degree a to b polynomial
	*
	*	\param[in] a
	*	[short const &] Degree a
	*
	*	\param[in] b
	*	[short const &] Degree b
	*/
	/********************************************************************************/
	Polyc256::Polyc256(short const& a, short const& b) : expr(), degA(a), degB(b) {}

	/********************************************************************************/
	/*!
	*	\brief
	*	Argument constructor for Polyc256 to construct from another same degree polynomial
	*
	*	\param[in] poly
	*	[Polyc256 const &] Other polynomial of same degree
	*/
	/********************************************************************************/
	Polyc256::Polyc256(Polyc256 const& poly) : expr(), degA(poly.degA), degB(poly.degB) // arg constructor
	{
		for (size_t i = ToIndex(degA); i < ToIndex(degB); ++i) // copy coefficient values
		{
			expr[i] = poly[static_cast<short>(i) + degA];
		}
	}

	/********************************************************************************/
	/*!
	*	\brief
	*	Copy Assignment Operator - Assigns another polynomial to this polynomial (Both with same degree)
	*
	*	\param[in] poly
	*	[Polyc256 const &] Other polynomial of same degree
	*
	*	\return
	*	[Polyc256 const &] This polynomial after assignment
	*/
	/********************************************************************************/
	Polyc256 const& Polyc256::operator=(Polyc256 const& poly) // copy assign
	{
		if (poly.degA != degA || poly.degB != degB)
			throw std::runtime_error("Polyc256: Copy assignment received different polynomial");

		for (size_t i = ToIndex(degA); i < ToIndex(degB); ++i) // copy coefficient values
		{
			expr[i] = poly[static_cast<short>(i) + degA];
		}

		return *this;
	}

	/********************************************************************************/
	/*!
	*	\brief
	*	operator() - Evaluate polynomial with input
	*
	*	\param[in] value
	*	[T const &] Value to input polynomial function with
	*
	*	\return
	*	[T const] Solution to polynomial with given value
	*/
	/********************************************************************************/
	std::complex<float> const Polyc256::operator()(std::complex<float> const& value) const // Evaluate polynomial
	{
		std::complex<float> result{};

		for (size_t i = ToIndex(degA); i <= ToIndex(degB); ++i) // for each deg
		{
			if (i > ToIndex(0)) // more than 0 deg
			{
				std::complex<float> var{ value };
				for (size_t j = ToIndex(0); j < i; ++j) var *= value; // power

				result += expr[i] * var; // add to result

				continue;
			}

			if (i < ToIndex(0)) // less than 0 deg
			{
				std::complex<float> var{ value };
				for (size_t j = ToIndex(degA); j < ToIndex(0); ++j) var *= 1.f / value; // power

				result += expr[i] * var; // add to result

				continue;
			}

			result += expr[i]; // add constant to result
		}

		return result;
	}

	/********************************************************************************/
	/*!
	*	\brief
	*	operator[] - Get coefficient at given degree
	*
	*	\param[in] degree
	*	[std::complex<float> const &] Degree to get coefficient from
	*
	*	\return
	*	[std::complex<float> const &] Coefficient value at given degree
	*/
	/********************************************************************************/
	std::complex<float> const& Polyc256::operator[](short const& degree) const // Const coefficient
	{
		return expr[ToIndex(degree)];
	}
	/********************************************************************************/
	/*!
	*	\brief
	*	operator[] - Set coefficient at given degree
	*
	*	\param[in] degree
	*	[std::complex<float> const &] Degree to set coefficient at
	*
	*	\return
	*	[std::complex<float> const &] Coefficient value at given degree
	*/
	/********************************************************************************/
	std::complex<float>& Polyc256::operator[](short const& degree) // Modifiable coefficient
	{
		return expr[ToIndex(degree)];
	}

	/********************************************************************************/
	/*!
	*	\brief
	*	operator* - Evaluates product of lhs polynomial and rhs polynomial
	*
	*	\param[in] rhs
	*	[Polyc256 const &] RHS polynomial
	*
	*	\return
	*	[Polyc256 const] Product of LHS and RHS
	*/
	/********************************************************************************/
	Polyc256 const Polyc256::operator*(Polyc256 const& rhs) // Multiply with another polynomial
	{
		Polyc256 result(degA + rhs.degA, degB + rhs.degB);

		for (short i = degA; i < 0 && i <= degB; ++i) // for each lhs deg
		{
			for (short j = rhs.degA; j < 0 && i <= rhs.degB; ++j) // for each rhs deg
			{
				result[i + j] += (*this)[i] * rhs[j]; // calculate coefficient per deg
			}
		}

		for (short i = (degA <= 0 ? 0 : degA); i <= degB; ++i) // for each lhs deg
		{
			for (short j = (rhs.degA <= 0 ? 0 : rhs.degA); j <= rhs.degB; ++j) // for each rhs deg
			{
				result[i + j] += (*this)[i] * rhs[j]; // calculate coefficient per deg
			}
		}

		return result;
	}
} 
