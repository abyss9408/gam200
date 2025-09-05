/******************************************************************************/
/*!
\file   Mat2i.h
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

    // Mat2i class to manipulate int 2D matrices conveniently
    class Mat2i
    {
    public:

        /*****************************************************************************/

        // Proxy class of Mat2i to modify values in an object m of type Mat2i m using m[r][c] syntax
        class Proxy
        {
        private:
            Mat2i& parent;
            size_t row;
        public:
            /*!*****************************************************************************
            *   \brief
            *   Mat2i::Proxy class arg constructor
            *
            *   \param[in] m
            *   [Mat2i&] Mat2i class parent of this Proxy class
            *
            *   \param[in] r
            *   [size_t const&] row of 2D Mat2i data to access
            *******************************************************************************/
            Proxy(Mat2i& m, size_t const& r) : parent(m), row(r) {}

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

        // ProxyConst class of Mat2i to access values in an object m of type Mat2i m using m[r][c] syntax
        class ProxyConst
        {
        private:
            Mat2i const& parent;
            size_t const row;
        public:
            /*!*****************************************************************************
            *   \brief
            *   Mat2i::ProxyConst class arg constructor
            *
            *   \param[in] m
            *   [Mat2i const&] const Mat2i class parent of this Proxy class
            *
            *   \param[in] r
            *   [size_t const&] row of 2D Mat2i data to access
            *******************************************************************************/
            ProxyConst(Mat2i const& m, size_t const& r) : parent(m), row(r) {}

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
        *   Mat2i class arg constructor
        *   creates a matrix with nr rows and nc columns
        *
        *   \param[in] nr
        *   [size_t const&] row count of 2D matrix
        *
        *   \param[in] nc
        *   [size_t const&] column count of 2D matrix
        *******************************************************************************/
        Mat2i(size_t const& nr, size_t const& nc);

        /*!*****************************************************************************
        *   \brief
        *   Mat2i class copy constructor
        *   creates a matrix deep copy of matrix rhs
        *
        *   \param[in] rhs
        *   [Mat2i const&] Mat2i to copy from
        *******************************************************************************/
        Mat2i(Mat2i const& rhs);
        /*!*****************************************************************************
        *   \brief
        *   Mat2i class move constructor
        *   moves rhs to new matrix via move semantics
        *
        *   \param[in] rhs
        *   [Mat2i const&] Mat2i to move from
        *******************************************************************************/
        //Mat2i(Mat2i&& rhs) noexcept;

        /*!*****************************************************************************
        *   \brief
        *   Mat2i class arg constructor
        *   creates a nr x nc matrix from an initializer list that has nr rows and nc columns if possible
        *
        *   \param[in] list
        *   [std::initializer_list<std::initializer_list<int>>&] 2D Initializer list to construct from
        *******************************************************************************/
        Mat2i(std::initializer_list<std::initializer_list<int>>& list);

        /*!*****************************************************************************
        *   \brief
        *   Mat2i class destructor
        *   destroys the matrix by explicitly returning storage to free store
        *******************************************************************************/
        ~Mat2i() noexcept;

        /*!*****************************************************************************
        *   \brief
        *   Mat2i copy assignment operator
        *   replaces matrix with a deep copy of rhs
        *
        *   \param[in] rhs
        *   [Mat2i const&] 2D matrix to copy from
        *
        *   \return
        *   [Mat2i&] this matrix
        *******************************************************************************/
        Mat2i& operator=(Mat2i const& rhs);
        /*!*****************************************************************************
        *   \brief
        *   Mat2i move assignment operator
        *   replaces matrix with rhs
        *
        *   \param[in] rhs
        *   [Mat2i &&] 2D matrix to move from
        *
        *   \return
        *   [Mat2i&] this matrix
        *******************************************************************************/
        //Mat2i& operator=(Mat2i&& rhs) noexcept;

        /*!*****************************************************************************
        *   \brief
        *   Mat2i compound multiplication assignment operator
        *   replaces matrix with matrix * rhs
        *
        *   \param[in] rhs
        *   [Mat2i &] 2D matrix to multiply with
        *
        *   \return
        *   [Mat2i&] this matrix
        *******************************************************************************/
        Mat2i& operator*=(Mat2i const& rhs);

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
        *   [int**] Mat2i 2D array data
        *******************************************************************************/
        int** UseData() noexcept { return data; }

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
        *   Sets data of matrix (dangerous! make sure that data is not out of control)
        *
        *   \param[in] c
        *   [int** const&] Data of 2D array of Mat2i
        *******************************************************************************/
        void SetData(int** const& d) { data = d; }

        /*!*****************************************************************************
        *   \brief
        *   Fills matrix with value
        *
        *   \param[in] val
        *   [int const&] Value to fill Mat2i with
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
        std::pair<size_t, size_t> FindFirst(int const& value) const;

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
        *   Mat2i::Proxy subscript operator
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
        *   Mat2i::ProxyConst subscript operator
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
        int** data;
    };

#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

    /*****************************************************************************/

    // Global functions

    /*!*****************************************************************************
    *   \brief
    *   Mat2i + operator
    *   returns matrix with sum (M + N) of matrices M and N
    *
    *   \param[in] M
    *   [Mat2i const&] Left Mat2i operand
    *
    *   \param[in] N
    *   [Mat2i const&] Right Mat2i operand
    *
    *   \return
    *   [Mat2i const] Sum of M + N
    *******************************************************************************/
    Mat2i const operator+(Mat2i const& M, Mat2i const& N);

    /*!*****************************************************************************
    *   \brief
    *   Mat2i - operator
    *   returns matrix with difference (M - N) of matrices M and N
    *
    *   \param[in] M
    *   [Mat2i const&] Left Mat2i operand
    *
    *   \param[in] N
    *   [Mat2i const&] Right Mat2i operand
    *
    *   \return
    *   [Mat2i const] Difference of M - N
    *******************************************************************************/
    Mat2i const operator-(Mat2i const& M, Mat2i const& N);

    /*!*****************************************************************************
    *   \brief
    *   Mat2i * operator
    *   returns matrix that is product (MN) of matrices M and N
    *
    *   \param[in] M
    *   [Mat2i const&] Left Mat2i operand
    *
    *   \param[in] N
    *   [Mat2i const&] Right Mat2i operand
    *
    *   \return
    *   [Mat2i const] Product of MN
    *******************************************************************************/
    Mat2i const operator*(Mat2i const& M, Mat2i const& N);

    /*!*****************************************************************************
    *   \brief
    *   Mat2i * operator
    *   returns matrix which is obtained by scaling every element of matrix M by scale factor r
    *
    *   \param[in] r
    *   [int const &] value to scale matrix by
    *
    *   \param[in] N
    *   [Mat2i const&] Mat2i
    *
    *   \return
    *   [Mat2i const] N scaled with r
    *******************************************************************************/
    Mat2i const operator*(int const& r, Mat2i const& N);

    /*!*****************************************************************************
    *   \brief
    *   Mat2i == operator
    *   returns true if matrices M and N are exactly equivalent
    *
    *   \param[in] M
    *   [Mat2i const&] Left Mat2i operand
    *
    *   \param[in] N
    *   [Mat2i const&] Right Mat2i operand
    *
    *   \return
    *   [bool] Boolean of matrices being equivalent
    *******************************************************************************/
    bool operator==(Mat2i const& M, Mat2i const& N);

    /*!*****************************************************************************
    *   \brief
    *   Mat2i != operator
    *   returns true if matrices M and N are not equivalent
    *
    *   \param[in] M
    *   [Mat2i const&] Left Mat2i operand
    *
    *   \param[in] N
    *   [Mat2i const&] Right Mat2i operand
    *
    *   \return
    *   [bool] Boolean of matrices being not equivalent
    *******************************************************************************/
    bool operator!=(Mat2i const& M, Mat2i const& N);
}