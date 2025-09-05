/******************************************************************************/
/*!
\file   Polynomial.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		This is the header file for the Polynomial classes that have mathematical polynomial functionality:
		Polynomial evaluation, arithmetic, and modification to polynomials

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <complex>

#include <ostream>

namespace PopplioMath
{
    class Polyf256 // Mathematical polynomial (float w/ deg 0 - 255)
    {
        private:
        short degA, degB; // start & end of degree

        float expr[256]; // expression

        size_t const ToIndex(short const& deg) const { return deg - degA; }
        size_t const GetSize() const { return degB - degA + 1; }

        public:
        short const& GetDegA() const { return degA;}
        short const& GetDegB() const { return degB; }

        /********************************************************************************/
        /*!	
        *	\brief
        *	Default constructor for Polyf256 - Creates zero polynomial
        */	
        /********************************************************************************/
        Polyf256();

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
        Polyf256(short const& a, short const& b);

        /********************************************************************************/
        /*!	
        *	\brief
        *	Argument constructor for Polyf256 to construct from another same degree polynomial
        *
        *	\param[in] poly
        *	[Polyf256 const &] Other polynomial of same degree
        */	
        /********************************************************************************/
        Polyf256(Polyf256 const &poly); 

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
        Polyf256 const &operator=(Polyf256 const &poly); 

        /********************************************************************************/
        /*!	
        *	\brief
        *	operator() - Evaluate polynomial with input
        *
        *	\param[in] value
        *	[float const &] Value to input polynomial function with
        *
        *	\return 
        *	[float const] Solution to polynomial with given value
        */	
        /********************************************************************************/
        float const operator()(float const &value) const; 
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
        float const & operator[](short const &degree) const; // Const coefficient
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
        float& operator[](short const& degree); // Modifiable coefficient

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
        Polyf256 const operator*(Polyf256 const &rhs); // Multiply with another polynomial
    };

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
    std::ostream &operator<<(std::ostream &out, Polyf256 const &pol); // output overload

    // ****************************************************************************** //

    class Polyc256 // Mathematical polynomial (complex<float> w/ deg 0 - 255)
    {
    private:
        short degA, degB; // start & end of degree

        std::complex<float> expr[256]; // expression

        size_t const ToIndex(short const& deg) const { return deg - degA; }
        size_t const GetSize() const { return degB - degA + 1; }

    public:
        short const& GetDegA() const { return degA; }
        short const& GetDegB() const { return degB; }

        /********************************************************************************/
        /*!
        *	\brief
        *	Default constructor for Polyc256 - Creates zero polynomial
        */
        /********************************************************************************/
        Polyc256();

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
        Polyc256(short const& a, short const& b);

        /********************************************************************************/
        /*!
        *	\brief
        *	Argument constructor for Polyc256 to construct from another same degree polynomial
        *
        *	\param[in] poly
        *	[Polyc256 const &] Other polynomial of same degree
        */
        /********************************************************************************/
        Polyc256(Polyc256 const& poly);

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
        Polyc256 const& operator=(Polyc256 const& poly);

        /********************************************************************************/
        /*!
        *	\brief
        *	operator() - Evaluate polynomial with input
        *
        *	\param[in] value
        *	[std::complex<float> const &] Value to input polynomial function with
        *
        *	\return
        *	[std::complex<float> const] Solution to polynomial with given value
        */
        /********************************************************************************/
        std::complex<float> const operator()(std::complex<float> const& value) const;
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
        std::complex<float> const& operator[](short const& degree) const; // Const coefficient
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
        std::complex<float>& operator[](short const& degree); // Modifiable coefficient

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
        Polyc256 const operator*(Polyc256 const& rhs); // Multiply with another polynomial
    };
}

#endif
