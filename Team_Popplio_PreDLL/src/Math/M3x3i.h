/******************************************************************************/
/*!
\file   M3x3i.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the header file for the Matrix2D classes API.
        This file implements convenient matrix classes
        Dynamically allocates 2D arrays, accessible via subscripts
        Constructible with Initializer lists
        Utilises proxy classes for convenient 2D subscript usage
        Able to use move semantics to optimize performance
        Handles exceptions

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "glm/glm.hpp"

#include <list>

namespace PopplioMath
{

#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

    class M3x3i
    {
    public:

        /*****************************************************************************/

        // Proxy class of M3x3i to modify values in an object m of type M3x3i m using m[r][c] syntax
        class Proxy
        {
        private:
            M3x3i& parent;
            size_t row;
        public:
            /*!*****************************************************************************
            *   \brief
            *   M3x3i::Proxy class arg constructor
            *
            *   \param[in] m
            *   [M3x3i&] M3x3i class parent of this Proxy class
            *
            *   \param[in] r
            *   [size_t const&] row of 2D M3x3i data to access
            *******************************************************************************/
            Proxy(M3x3i& m, size_t const& r) : parent(m), row(r) {}

            /*!*****************************************************************************
            *   \brief
            *   Access modifiable value at [row, c] of parent's data
            *
            *   \param[in] c
            *   [size_t const&] column of 2D matrix data
            *
            *   \return
            *   [int&] modifiable reference to value at [row, c]
            *******************************************************************************/
            int& operator[](size_t const& c) noexcept { return parent.data[row][c]; }
        };

        // ProxyConst class of M3x3i to access values in an object m of type M3x3i m using m[r][c] syntax
        class ProxyConst
        {
        private:
            M3x3i const& parent;
            size_t const row;
        public:
            /*!*****************************************************************************
            *   \brief
            *   M3x3i::ProxyConst class arg constructor
            *
            *   \param[in] m
            *   [M3x3i const&] const M3x3i class parent of this Proxy class
            *
            *   \param[in] r
            *   [size_t const&] row of 2D M3x3i data to access
            *******************************************************************************/
            ProxyConst(M3x3i const& m, size_t const& r) : parent(m), row(r) {}

            /*!*****************************************************************************
            *   \brief
            *   Access read-only value at [row, c] of parent's data
            *
            *   \param[in] c
            *   [size_t const&] column of 2D matrix data
            *
            *   \return
            *   [int const&] read-only reference to value at [row, c]
            *******************************************************************************/
            int const& operator[](size_t const& c) const noexcept { return parent.data[row][c]; }
        };

        /*****************************************************************************/

        /*!*****************************************************************************
        *   \brief
        *   M3x3i class arg constructor
        *   creates a matrix with 3 rows and 3 columns
        *******************************************************************************/
        M3x3i();

        /*!*****************************************************************************
        *   \brief
        *   M3x3i class copy constructor
        *   creates a matrix deep copy of matrix rhs
        *
        *   \param[in] rhs
        *   [M3x3i const&] M3x3i to copy from
        *******************************************************************************/
        M3x3i(M3x3i const& rhs);
        /*!*****************************************************************************
        *   \brief
        *   M3x3i class move constructor
        *   moves rhs to new matrix via move semantics
        *
        *   \param[in] rhs
        *   [M3x3i const&] M3x3i to move from
        *******************************************************************************/
        //M3x3i(M3x3i&& rhs) noexcept;

        /*!*****************************************************************************
        *   \brief
        *   M3x3i class arg constructor
        *   creates a nr x nc matrix from an initializer list that has nr rows and nc columns if possible
        *
        *   \param[in] list
        *   [std::initializer_list<std::initializer_list<int>>&] 2D Initializer list to construct from
        *******************************************************************************/
        M3x3i(std::initializer_list<std::initializer_list<int>>& list);

        /*!*****************************************************************************
        *   \brief
        *   M3x3i class destructor
        *   destroys the matrix by explicitly returning storage to free store
        *******************************************************************************/
        ~M3x3i() = default;

        /*!*****************************************************************************
        *   \brief
        *   M3x3i copy assignment operator
        *   replaces matrix with a deep copy of rhs
        *
        *   \param[in] rhs
        *   [M3x3i const&] 2D matrix to copy from
        *
        *   \return
        *   [M3x3i&] this matrix
        *******************************************************************************/
        M3x3i& operator=(M3x3i const& rhs);
        /*!*****************************************************************************
        *   \brief
        *   M3x3i move assignment operator
        *   replaces matrix with rhs
        *
        *   \param[in] rhs
        *   [M3x3i &&] 2D matrix to move from
        *
        *   \return
        *   [M3x3i&] this matrix
        *******************************************************************************/
        //M3x3i& operator=(M3x3i&& rhs) noexcept;

        /*!*****************************************************************************
        *   \brief
        *   M3x3i compound multiplication assignment operator
        *   replaces matrix with matrix * rhs
        *
        *   \param[in] rhs
        *   [M3x3i &] 2D matrix to multiply with
        *
        *   \return
        *   [M3x3i&] this matrix
        *******************************************************************************/
        M3x3i& operator*=(M3x3i const& rhs);

        /*!*****************************************************************************
        *   \brief
        *   Returns read-only number of rows in matrix
        *
        *   \return
        *   [size_t const&] row count of matrix
        *******************************************************************************/
        size_t const& GetRows() const noexcept { return rows; }

        /*!*****************************************************************************
        *   \brief
        *   Returns read-only number of columns in matrix
        *
        *   \return
        *   [size_t const&] column count of matrix
        *******************************************************************************/
        size_t const& GetCols() const noexcept { return cols; }

        /*!*****************************************************************************
        *   \brief
        *   Returns modifiable 2D array data of matrix (Dangerous!)
        *
        *   \return
        *   [int**] M3x3i 2D array data
        *******************************************************************************/
        // int** UseData() noexcept { return data; }

        /*!*****************************************************************************
        *   \brief
        *   Sets number of rows in matrix
        *
        *   \param[in] r
        *   [size_t const&] number of rows to set
        *******************************************************************************/
        void SetRows(size_t const& r) { rows = r; }

        /*!*****************************************************************************
        *   \brief
        *   Sets number of columns in matrix
        *
        *   \param[in] c
        *   [size_t const&] number of columns to set
        *******************************************************************************/
        void SetCols(size_t const& c) { cols = c; }

        /*!*****************************************************************************
        *   \brief
        *   Fills matrix with value
        *
        *   \param[in] val
        *   [int const&] Value to fill M3x3i with
        *******************************************************************************/
        void Fill(int const& val);

        /*!*****************************************************************************
        *   \brief
        *   Finds first instance found in matrix for target value as (row, col)
        *
        *   \param[in] value
        *   [int const&] Target value to be found
        *
        *   \return
        *   [std::pair<size_t, size_t> const&] Index of first instance found as (row, col), return {rows, cols} if none found
        *******************************************************************************/
        std::pair<size_t, size_t> const FindFirst(int const& value) const;

        /*!*****************************************************************************
        *   \brief
        *   Counts instances of given value within given matrix
        *
        *   \param[in] value
        *   [int const&] Target value to be counted
        *
        *   \return
        *   [size_t const&] Count of given value in matrix
        *******************************************************************************/
        size_t const GetCount(int const& value) const;

        /*!*****************************************************************************
        *   \brief
        *   M3x3i::Proxy subscript operator
        *   returns index where r'th row of matrix data to modify
        *
        *   \param[in] r
        *   [size_t const&] row of matrix
        *
        *   \return
        *   [Proxy] Proxy class to use for 2D subscripts
        *******************************************************************************/
        Proxy operator[](size_t const& r) { return Proxy(*this, r); }
        /*!*****************************************************************************
        *   \brief
        *   M3x3i::ProxyConst subscript operator
        *   returns index where r'th row of matrix data to access
        *
        *   \param[in] r
        *   [size_t const&] row of matrix
        *
        *   \return
        *   [Proxy] Proxy class to use for 2D subscripts
        *******************************************************************************/
        ProxyConst operator[](size_t const& r) const { return ProxyConst(*this, r); }

        /*****************************************************************************/

    private:

        size_t rows;
        size_t cols;

        union // save space w/ union (hlp3 lecture 17)
        {
            int data[3][3]{};

            struct // 3 by 3 matrix
            {
                int r0[3], r1[3], r2[3];
            };

            int mtx[9];
        };
        // void AssignMTX(); // assign mtx

    public:

        M3x3i(const int& f) : M3x3i() { Fill(f); } // fill

        //M3x3i(M3x3i&& rhs) noexcept : M3x3i(rhs) { AssignMTX(); }

        M3x3i(const int* pArr); // array constructor

        M3x3i(int const& m00, int const& m01, int const& m02,
            int const& m10, int const& m11, int const& m12,
            int const& m20, int const& m21, int const& m22); // 9 value constructor

        int& MTX(size_t const& i); // get mtx

        glm::mat3 ToGLM() const // GLM converter
        {
            return glm::mat3(r0[0], r1[0], r2[0],
                            r0[1], r1[1], r2[1],
                            r0[2], r1[2], r2[2]);
        }

        /**************************************************************************/
        /*!
            This function sets the matrix to the identity matrix
        */
        /**************************************************************************/
        void Identity();
        /**************************************************************************/
        /*!
            This function creates a translation matrix from x & y
        */
        /**************************************************************************/
        void Translate(int const& x, int const& y);
        /**************************************************************************/
        /*!
            This function creates a scaling matrix from x & y
        */
        /**************************************************************************/
        void Scale(int const& x, int const& y);
        /**************************************************************************/
        /*!
            This matrix creates a rotation matrix from "radAngle" whose value is in radian
        */
        /**************************************************************************/
        void RotRad(int const& radAngle);
        /**************************************************************************/
        /*!
            This matrix creates a rotation matrix from "radAngle" whose value is in degree
        */
        /**************************************************************************/
        void RotDeg(int const& radAngle);
        /**************************************************************************/
        /*!
            This function calculates the transpose matrix
        */
        /**************************************************************************/
        void Transpose();
        /**************************************************************************/
        /*!
            This function calculates the inverse matrix. If the matrix inversion fails, return false.
        */
        /**************************************************************************/
        bool Inverse(int& determinant);
    };

#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

    /*****************************************************************************/

    // Global functions

    /*!*****************************************************************************
    *   \brief
    *   M3x3i + operator
    *   returns matrix with sum (M + N) of matrices M and N
    *
    *   \param[in] M
    *   [M3x3i const&] Left M3x3i operand
    *
    *   \param[in] N
    *   [M3x3i const&] Right M3x3i operand
    *
    *   \return
    *   [M3x3i const] Sum of M + N
    *******************************************************************************/
    M3x3i const operator+(M3x3i const& M, M3x3i const& N);

    /*!*****************************************************************************
    *   \brief
    *   M3x3i - operator
    *   returns matrix with difference (M - N) of matrices M and N
    *
    *   \param[in] M
    *   [M3x3i const&] Left M3x3i operand
    *
    *   \param[in] N
    *   [M3x3i const&] Right M3x3i operand
    *
    *   \return
    *   [M3x3i const] Difference of M - N
    *******************************************************************************/
    M3x3i const operator-(M3x3i const& M, M3x3i const& N);

    /*!*****************************************************************************
    *   \brief
    *   M3x3i * operator
    *   returns matrix that is product (MN) of matrices M and N
    *
    *   \param[in] M
    *   [M3x3i const&] Left M3x3i operand
    *
    *   \param[in] N
    *   [M3x3i const&] Right M3x3i operand
    *
    *   \return
    *   [M3x3i const] Product of MN
    *******************************************************************************/
    M3x3i const operator*(M3x3i const& M, M3x3i const& N);

    /*!*****************************************************************************
    *   \brief
    *   M3x3i * operator
    *   returns matrix which is obtained by scaling every element of matrix M by scale factor r
    *
    *   \param[in] r
    *   [int const &] value to scale matrix by
    *
    *   \param[in] N
    *   [M3x3i const&] M3x3i
    *
    *   \return
    *   [M3x3i const] N scaled with r
    *******************************************************************************/
    M3x3i const operator*(int const& r, M3x3i const& N);

    /*!*****************************************************************************
    *   \brief
    *   M3x3i == operator
    *   returns true if matrices M and N are exactly equivalent
    *
    *   \param[in] M
    *   [M3x3i const&] Left M3x3i operand
    *
    *   \param[in] N
    *   [M3x3i const&] Right M3x3i operand
    *
    *   \return
    *   [bool] Boolean of matrices being equivalent
    *******************************************************************************/
    bool operator==(M3x3i const& M, M3x3i const& N);

    /*!*****************************************************************************
    *   \brief
    *   M3x3i != operator
    *   returns true if matrices M and N are not equivalent
    *
    *   \param[in] M
    *   [M3x3i const&] Left M3x3i operand
    *
    *   \param[in] N
    *   [M3x3i const&] Right M3x3i operand
    *
    *   \return
    *   [bool] Boolean of matrices being not equivalent
    *******************************************************************************/
    bool operator!=(M3x3i const& M, M3x3i const& N);

    /**************************************************************************/
    /*!
        This function sets the matrix pResult to the identity matrix
    */
    /**************************************************************************/
    void M3x3iIdentity(M3x3i& pResult);

    /**************************************************************************/
    /*!
        This function creates a translation matrix from x & y
        and saves it in pResult
    */
    /**************************************************************************/
    void M3x3iTranslate(M3x3i& pResult, int const& x, int const& y);

    /**************************************************************************/
    /*!
        This function creates a scaling matrix from x & y
        and saves it in pResult
    */
    /**************************************************************************/
    void M3x3iScale(M3x3i& pResult, int const& x, int const& y);

    /**************************************************************************/
    /*!
        This matrix creates a rotation matrix from "radAngle" whose value
        is in radian. Save the resultant matrix in pResult.
    */
    /**************************************************************************/
    void M3x3iRotRad(M3x3i& pResult, int const& radAngle);

    /**************************************************************************/
    /*!
        This matrix creates a rotation matrix from "radAngle" whose value
        is in degree. Save the resultant matrix in pResult.
    */
    /**************************************************************************/
    void M3x3iRotDeg(M3x3i& pResult, int const& radAngle);

    /**************************************************************************/
    /*!
        This functions calculated the transpose matrix of pMtx
        and saves it in pResult
    */
    /**************************************************************************/
    void M3x3iTranspose(M3x3i& pResult, const M3x3i& pMtx);

    /**************************************************************************/
    /*!
        This function calculates the inverse matrix of pMtx and saves the
        result in pResult. If the matrix inversion fails, pResult
        would be set to NULL.
    */
    /**************************************************************************/
    void M3x3iInverse(M3x3i* pResult, int& determinant, const M3x3i& pMtx);
}