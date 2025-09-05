/******************************************************************************/
/*!
\file   Vector2D.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
		Header file for Vector2D classes and vector space operations.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <glm/glm.hpp>

// Float
namespace PopplioMath
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

	struct Vec2f
	{
		float x, y;

		// Constructors

		Vec2f();

		/**************************************************************************/
		/*!
		\brief
			Vec2f class constructor
		\param[in] _x
			[float const&] x axis value
		\param[in] _y
			[float const&] y axis value
		*/
		/**************************************************************************/
		Vec2f(float _x, float _y);

		// Default Class Functions
		Vec2f& operator=(const Vec2f& rhs) = default;
		Vec2f(const Vec2f& rhs) = default;

		// Assignment operators
		Vec2f& operator += (const Vec2f& rhs);
		Vec2f& operator -= (const Vec2f& rhs);
		Vec2f& operator *= (float const& rhs);
		Vec2f& operator /= (float const& rhs);

		// Unary operators
		Vec2f operator -() const;

		// Helpers

		/**************************************************************************/
		/*!
			Returns unit vector of the vector
		*/
		/**************************************************************************/
		Vec2f const Normal();

		/**************************************************************************/
		/*!
			Returns the length of the vector
		*/
		/**************************************************************************/
		float const	Len();

		/**************************************************************************/
		/*!
			Returns the square of the vector length
		*/
		/**************************************************************************/
		float const SqLen();

		/**************************************************************************/
		/*!
			In this function, this vector and rhs are considered as 2D points.
			The distance between these 2 2D points is returned
		*/
		/**************************************************************************/
		float const	Dist(const Vec2f& rhs);

		/**************************************************************************/
		/*!
			In this function, this vector and rhs are considered as 2D points.
			The squared distance between these 2 2D points is returned.
		*/
		/**************************************************************************/
		float const	SqDist(const Vec2f& rhs);

		/**************************************************************************/
		/*!
			Returns the dot product between vector and rhs
		*/
		/**************************************************************************/
		float const	Dot(const Vec2f& rhs);

		/**************************************************************************/
		/*!
			Returns the cross product magnitude between vector and rhs
		*/
		/**************************************************************************/
		float const	CrossMag(const Vec2f& rhs);

		glm::vec2 ToGLM() const;
	};


#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

	// Binary operators

	/**************************************************************************/
	/*!
	\brief
		Vec2f + operator overload to sum left and right operands
	\param[in] lhs
		[const Vec2f &] left operand
	\param[in] rhs
		[const Vec2f &] right operand
	\return
		[Vec2f] Vec2f sum value
	*/
	/**************************************************************************/
	Vec2f operator + (const Vec2f& lhs, const Vec2f& rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2f - operator overload to subtract right from left operand
	\param[in] lhs
		[const Vec2f &] left operand
	\param[in] rhs
		[const Vec2f &] right operand
	\return
		[Vec2f] Vec2f new value
	*/
	/**************************************************************************/
	Vec2f operator - (const Vec2f& lhs, const Vec2f& rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2f * operator overload to multiply scale Vec2f with a float
	\param[in] lhs
		[const Vec2f &] left operand
	\param[in] rhs
		[float const&] right operand
	\return
		[Vec2f] Vec2f new value
	*/
	/**************************************************************************/
	Vec2f operator * (const Vec2f& lhs, float const& rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2f * operator overload to multiply scale Vec2f with a float
	\param[in] lhs
		[float const&] left operand
	\param[in] rhs
		[const Vec2f &] right operand
	\return
		[Vec2f] Vec2f new value
	*/
	/**************************************************************************/
	Vec2f operator * (float const& lhs, const Vec2f& rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2f \ operator overload to divide scale Vec2f with a float
	\param[in] lhs
		[const Vec2f &] left operand
	\param[in] rhs
		[float const&] right operand
	\return
		[Vec2f] Vec2f new value
	 */
	 /**************************************************************************/
	Vec2f operator / (const Vec2f& lhs, float rhs);

	bool operator == (const Vec2f& lhs, const Vec2f& rhs);

	bool operator != (const Vec2f& lhs, const Vec2f& rhs);

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	*/
	/**************************************************************************/
	void Vec2fNormal(Vec2f& pResult, const Vec2f& pVec0);

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	*/
	/**************************************************************************/
	float Vec2fSqLen(const Vec2f& pVec0);

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	*/
	/**************************************************************************/
	float Vec2fLen(const Vec2f& pVec0);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	*/
	/**************************************************************************/
	float Vec2fSqDist(const Vec2f& pVec0, const Vec2f& pVec1);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	*/
	/**************************************************************************/
	float Vec2fDist(const Vec2f& pVec0, const Vec2f& pVec1);

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	*/
	/**************************************************************************/
	float Vec2fDot(const Vec2f& pVec0, const Vec2f& pVec1);

	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	*/
	/**************************************************************************/
	float Vec2fCrossMag(const Vec2f& pVec0, const Vec2f& pVec1);

	// Integer

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

	struct Vec2i
	{
		int x, y;

		// Constructors

		Vec2i();

		/**************************************************************************/
		/*!
		\brief
			Vec2i class constructor
		\param[in] _x
			[int const&] x axis value
		\param[in] _y
			[int const&] y axis value
		*/
		/**************************************************************************/
		Vec2i(int _x, int _y);

		// Default Class Functions
		Vec2i& operator=(const Vec2i& rhs) = default;
		Vec2i(const Vec2i& rhs) = default;

		// Assignment operators
		Vec2i& operator += (const Vec2i& rhs);
		Vec2i& operator -= (const Vec2i& rhs);
		Vec2i& operator *= (int rhs);
		Vec2i& operator /= (int rhs);

		// Unary operators
		Vec2i operator -() const;

		// Helpers

		/**************************************************************************/
		/*!
			Returns unit vector of the vector
		*/
		/**************************************************************************/
		Vec2i Normal();

		/**************************************************************************/
		/*!
			Returns the length of the vector
		*/
		/**************************************************************************/
		int Len();

		/**************************************************************************/
		/*!
			Returns the square of the vector length
		*/
		/**************************************************************************/
		int SqLen();

		/**************************************************************************/
		/*!
			In this function, this vector and rhs are considered as 2D points.
			The distance between these 2 2D points is returned
		*/
		/**************************************************************************/
		int Dist(const Vec2i& rhs);

		/**************************************************************************/
		/*!
			In this function, this vector and rhs are considered as 2D points.
			The squared distance between these 2 2D points is returned.
		*/
		/**************************************************************************/
		int SqDist(const Vec2i& rhs);

		/**************************************************************************/
		/*!
			Returns the dot product between vector and rhs
		*/
		/**************************************************************************/
		int Dot(const Vec2i& rhs);

		/**************************************************************************/
		/*!
			Returns the cross product magnitude between vector and rhs
		*/
		/**************************************************************************/
		int CrossMag(const Vec2i& rhs);
	};


#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

	// Binary operators

	/**************************************************************************/
	/*!
	\brief
		Vec2i + operator overload to sum left and right operands
	\param[in] lhs
		[const Vec2i &] left operand
	\param[in] rhs
		[const Vec2i &] right operand
	\return
		[Vec2i] Vec2i sum value
	*/
	/**************************************************************************/
	Vec2i operator + (const Vec2i& lhs, const Vec2i& rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2i - operator overload to subtract right from left operand
	\param[in] lhs
		[const Vec2i &] left operand
	\param[in] rhs
		[const Vec2i &] right operand
	\return
		[Vec2i] Vec2i new value
	*/
	/**************************************************************************/
	Vec2i operator - (const Vec2i& lhs, const Vec2i& rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2i * operator overload to multiply scale Vec2i with a int
	\param[in] lhs
		[const Vec2i &] left operand
	\param[in] rhs
		[int const&] right operand
	\return
		[Vec2i] Vec2i new value
	*/
	/**************************************************************************/
	Vec2i operator * (const Vec2i& lhs, int rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2i * operator overload to multiply scale Vec2i with a int
	\param[in] lhs
		[int const&] left operand
	\param[in] rhs
		[const Vec2i &] right operand
	\return
		[Vec2i] Vec2i new value
	*/
	/**************************************************************************/
	Vec2i operator * (int lhs, const Vec2i& rhs);

	/**************************************************************************/
	/*!
	\brief
		Vec2i \ operator overload to divide scale Vec2i with a int
	\param[in] lhs
		[const Vec2i &] left operand
	\param[in] rhs
		[int const&] right operand
	\return
		[Vec2i] Vec2i new value
	 */
	 /**************************************************************************/
	Vec2i operator / (const Vec2i& lhs, int const& rhs);

	bool operator == (const Vec2i& lhs, const Vec2i& rhs);

	bool operator != (const Vec2i& lhs, const Vec2i& rhs);

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	*/
	/**************************************************************************/
	void Vec2iNormal(Vec2i& pResult, const Vec2i& pVec0);

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	*/
	/**************************************************************************/
	int Vec2iSqLen(const Vec2i& pVec0);

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	*/
	/**************************************************************************/
	int Vec2iLen(const Vec2i& pVec0);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	*/
	/**************************************************************************/
	int Vec2iSqDist(const Vec2i& pVec0, const Vec2i& pVec1);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	*/
	/**************************************************************************/
	int Vec2iDist(const Vec2i& pVec0, const Vec2i& pVec1);

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	*/
	/**************************************************************************/
	int Vec2iDot(const Vec2i& pVec0, const Vec2i& pVec1);

	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	*/
	/**************************************************************************/
	int Vec2iCrossMag(const Vec2i& pVec0, const Vec2i& pVec1);
}