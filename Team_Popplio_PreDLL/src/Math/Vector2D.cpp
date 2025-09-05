/******************************************************************************/
/*!
\file   Vector2D.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Source file for Vector2D classes and vector space operations.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h> // PCH

// Float
namespace PopplioMath
{
	Vec2f::Vec2f() : x(0.f), y(0.f) {}

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
	Vec2f::Vec2f(float _x, float _y) : x(_x), y(_y) {}

	/**************************************************************************/
	/*!
		Returns the length of the vector
	*/
	/**************************************************************************/
	float const	Vec2f::Len()
	{
		return sqrtf(this->SqLen());
	}

	/**************************************************************************/
	/*!
		Returns the square of the vector length
	*/
	/**************************************************************************/
	float const Vec2f::SqLen()
	{
		return (x * x) + (y * y);
	}

	/**************************************************************************/
	/*!
		In this function, this vector and rhs are considered as 2D points.
		The distance between these 2 2D points is returned
	*/
	/**************************************************************************/
	float const	Vec2f::Dist(const Vec2f& rhs)
	{
		return sqrtf(this->SqDist(rhs));
	}

	/**************************************************************************/
	/*!
		In this function, this vector and rhs are considered as 2D points.
		The squared distance between these 2 2D points is returned.
	*/
	/**************************************************************************/
	float const	Vec2f::SqDist(const Vec2f& rhs)
	{
		return ((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
	}

	/**************************************************************************/
	/*!
		Returns the dot product between vector and rhs
	*/
	/**************************************************************************/
	float const	Vec2f::Dot(const Vec2f& rhs)
	{
		return (x * rhs.x) + (y * rhs.y);
	}

	/**************************************************************************/
	/*!
		Returns the cross product magnitude between vector and rhs
	*/
	/**************************************************************************/
	float const	Vec2f::CrossMag(const Vec2f& rhs)
	{
		return (x * rhs.y) - (y * rhs.x);
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2f += operator overload to add right operand to left operand
	\param[in] rhs
		[const Vec2f &] right operand
	\return
		[Vec2f &] reference to modified left operand Vec2f
	*/
	/**************************************************************************/

	Vec2f& Vec2f::operator += (const Vec2f& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2f -= operator overload to subtract right operand from left operand
	\param[in] rhs
		[const Vec2f &] right operand
	\return
		[Vec2f &] reference to modified left operand Vec2f
	*/
	/**************************************************************************/

	Vec2f& Vec2f::operator -= (const Vec2f& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2f *= operator overload to multiply scale Vec2f with a float
	\param[in] rhs
		[float const&] right operand
	\return
		[Vec2f &] reference to modified left operand Vec2f
	*/
	/**************************************************************************/

	Vec2f& Vec2f::operator *= (float const& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2f *= operator overload to divide scale Vec2f with a float
	\param[in] rhs
		[float const&] right operand
	\return
		[Vec2f &] reference to modified left operand Vec2f
	*/
	/**************************************************************************/

	Vec2f& Vec2f::operator /= (float const& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	// Unary operators
	Vec2f Vec2f::operator-() const { return Vec2f(-x, -y); }

	/**************************************************************************/
	/*!
		Returns unit vector of the vector
	*/
	/**************************************************************************/

	Vec2f const Vec2f::Normal()
	{
		x /= this->Len();
		y /= this->Len();

		return *this;
	}

	// Non-member functions
	//-------------------------------------------------------------------------

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
	Vec2f operator + (const Vec2f& lhs, const Vec2f& rhs)
	{
		return Vec2f(lhs.x + rhs.x, lhs.y + rhs.y);
	}

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
	Vec2f operator - (const Vec2f& lhs, const Vec2f& rhs)
	{
		return Vec2f(lhs.x - rhs.x, lhs.y - rhs.y);
	}

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
	Vec2f operator * (const Vec2f& lhs, float const& rhs)
	{
		return Vec2f(lhs.x * rhs, lhs.y * rhs);
	}

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
	Vec2f operator * (float const& lhs, const Vec2f& rhs)
	{
		return Vec2f(lhs * rhs.x, lhs * rhs.y);
	}

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
	Vec2f operator / (const Vec2f& lhs, float rhs)
	{
		return Vec2f(lhs.x / rhs, lhs.y / rhs);
	}

	bool operator == (const Vec2f& lhs, const Vec2f& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y) ? true : false;
	}

	bool operator != (const Vec2f& lhs, const Vec2f& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y) ? false : true;
	}

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/

	void Vec2fNormal(Vec2f& pResult, const Vec2f& pVec0)
	{
		pResult.x = pVec0.x / Vec2fLen(pVec0);
		pResult.y = pVec0.y / Vec2fLen(pVec0);
	}

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	*/
	/**************************************************************************/
	float Vec2fSqLen(const Vec2f& pVec0)
	{
		return (pVec0.x * pVec0.x) + (pVec0.y * pVec0.y);
	}

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	*/
	/**************************************************************************/
	float Vec2fLen(const Vec2f& pVec0)
	{
		return sqrtf(Vec2fSqLen(pVec0));
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	*/
	/**************************************************************************/
	float Vec2fSqDist(const Vec2f& pVec0, const Vec2f& pVec1)
	{
		return ((pVec0.x - pVec1.x) * (pVec0.x - pVec1.x) + (pVec0.y - pVec1.y) * (pVec0.y - pVec1.y));
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	*/
	/**************************************************************************/
	float Vec2fDist(const Vec2f& pVec0, const Vec2f& pVec1)
	{
		return sqrtf(Vec2fSqDist(pVec0, pVec1));
	}

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	*/
	/**************************************************************************/
	float Vec2fDot(const Vec2f& pVec0, const Vec2f& pVec1)
	{
		return (pVec0.x * pVec1.x) + (pVec0.y * pVec1.y);
	}

	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	*/
	/**************************************************************************/
	float Vec2fCrossMag(const Vec2f& pVec0, const Vec2f& pVec1)
	{
		return (pVec0.x * pVec1.y) - (pVec0.y * pVec1.x);
	}

	/**************************************************************************/

	// Integer

	Vec2i::Vec2i() : x(0), y(0) {}

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
	Vec2i::Vec2i(int _x, int  _y) : x(_x), y(_y) {}


	// Unary operators
	Vec2i Vec2i::operator-() const { return Vec2i(-x, -y); }

	/**************************************************************************/
	/*!
		Returns the length of the vector
	*/
	/**************************************************************************/
	int Vec2i::Len()
	{
		return static_cast<int>(sqrtf(static_cast<float>(this->SqLen())));
	}

	/**************************************************************************/
	/*!
		Returns the square of the vector length
	*/
	/**************************************************************************/
	int Vec2i::SqLen()
	{
		return (x * x) + (y * y);
	}

	/**************************************************************************/
	/*!
		In this function, this vector and rhs are considered as 2D points.
		The distance between these 2 2D points is returned
	*/
	/**************************************************************************/
	int Vec2i::Dist(const Vec2i& rhs)
	{
		return static_cast<int>(sqrtf(static_cast<float>(this->SqDist(rhs))));
	}

	/**************************************************************************/
	/*!
		In this function, this vector and rhs are considered as 2D points.
		The squared distance between these 2 2D points is returned.
	*/
	/**************************************************************************/
	int Vec2i::SqDist(const Vec2i& rhs)
	{
		return ((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
	}

	/**************************************************************************/
	/*!
		Returns the dot product between vector and rhs
	*/
	/**************************************************************************/
	int Vec2i::Dot(const Vec2i& rhs)
	{
		return (x * rhs.x) + (y * rhs.y);
	}

	/**************************************************************************/
	/*!
		Returns the cross product magnitude between vector and rhs
	*/
	/**************************************************************************/
	int Vec2i::CrossMag(const Vec2i& rhs)
	{
		return (x * rhs.y) - (y * rhs.x);
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2i += operator overload to add right operand to left operand
	\param[in] rhs
		[const Vec2i &] right operand
	\return
		[Vec2i &] reference to modified left operand Vec2i
	*/
	/**************************************************************************/

	Vec2i& Vec2i::operator += (const Vec2i& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2i -= operator overload to subtract right operand from left operand
	\param[in] rhs
		[const Vec2i &] right operand
	\return
		[Vec2i &] reference to modified left operand Vec2i
	*/
	/**************************************************************************/

	Vec2i& Vec2i::operator -= (const Vec2i& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2i *= operator overload to multiply scale Vec2i with a int
	\param[in] rhs
		[int const&] right operand
	\return
		[Vec2i &] reference to modified left operand Vec2i
	*/
	/**************************************************************************/

	Vec2i& Vec2i::operator *= (int rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	/**************************************************************************/
	/*!
	\brief
		Vec2i *= operator overload to divide scale Vec2i with a int
	\param[in] rhs
		[int const&] right operand
	\return
		[Vec2i &] reference to modified left operand Vec2i
	*/
	/**************************************************************************/

	Vec2i& Vec2i::operator /= (int rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	/**************************************************************************/
	/*!
		Returns unit vector of the vector
	*/
	/**************************************************************************/

	Vec2i Vec2i::Normal()
	{
		x /= this->Len();
		y /= this->Len();

		return *this;
	}

	// Non-member functions
	//-------------------------------------------------------------------------

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
	Vec2i operator + (const Vec2i& lhs, const Vec2i& rhs)
	{
		return Vec2i(lhs.x + rhs.x, lhs.y + rhs.y);
	}

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
	Vec2i operator - (const Vec2i& lhs, const Vec2i& rhs)
	{
		return Vec2i(lhs.x - rhs.x, lhs.y - rhs.y);
	}

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
	Vec2i operator * (const Vec2i& lhs, int rhs)
	{
		return Vec2i(lhs.x * rhs, lhs.y * rhs);
	}

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
	Vec2i operator * (int lhs, const Vec2i& rhs)
	{
		return Vec2i(lhs * rhs.x, lhs * rhs.y);
	}

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
	Vec2i operator / (const Vec2i& lhs, int const& rhs)
	{
		return Vec2i(lhs.x / rhs, lhs.y / rhs);
	}

	bool operator == (const Vec2i& lhs, const Vec2i& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y) ? true : false;
	}

	bool operator != (const Vec2i& lhs, const Vec2i& rhs)
	{
		return (lhs.x == rhs.x && lhs.y == rhs.y) ? false : true;
	}

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/

	void Vec2iNormal(Vec2i& pResult, const Vec2i& pVec0)
	{
		pResult.x = pVec0.x / Vec2iLen(pVec0);
		pResult.y = pVec0.y / Vec2iLen(pVec0);
	}

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	*/
	/**************************************************************************/
	int Vec2iSqLen(const Vec2i& pVec0)
	{
		return (pVec0.x * pVec0.x) + (pVec0.y * pVec0.y);
	}

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	*/
	/**************************************************************************/
	int Vec2iLen(const Vec2i& pVec0)
	{
		return static_cast<int>(sqrtf(static_cast<float>(Vec2iSqLen(pVec0))));
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The squared distance between these 2 2D points is returned.
		Avoid the square root
	*/
	/**************************************************************************/
	int Vec2iSqDist(const Vec2i& pVec0, const Vec2i& pVec1)
	{
		return ((pVec0.x - pVec1.x) * (pVec0.x - pVec1.x) + (pVec0.y - pVec1.y) * (pVec0.y - pVec1.y));
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 2D points.
		The distance between these 2 2D points is returned
	*/
	/**************************************************************************/
	int Vec2iDist(const Vec2i& pVec0, const Vec2i& pVec1)
	{
		return static_cast<int>(sqrtf(static_cast<float>(Vec2iSqDist(pVec0, pVec1))));
	}

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	*/
	/**************************************************************************/
	int Vec2iDot(const Vec2i& pVec0, const Vec2i& pVec1)
	{
		return (pVec0.x * pVec1.x) + (pVec0.y * pVec1.y);
	}

	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	*/
	/**************************************************************************/
	int Vec2iCrossMag(const Vec2i& pVec0, const Vec2i& pVec1)
	{
		return (pVec0.x * pVec1.y) - (pVec0.y * pVec1.x);
	}

	glm::vec2 Vec2f::ToGLM() const
	{
		return glm::vec2(x, y);
	}
}