/******************************************************************************/
/*!
\file   Mat2i.cpp
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

#include <pch.h> // PCH

#include <algorithm>
#include <stdexcept>

namespace PopplioMath
{
    // Mat2i member function definitions

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

    Mat2i::Mat2i(size_t const& nr, size_t const& nc) : rows(nr), cols(nc)
    {
        data = new int* [rows];

        for (size_t r = 0; r < rows; ++r)
        {
            data[r] = new int[cols];
        }
    }

    /*!*****************************************************************************
    *   \brief
    *   Mat2i class copy constructor
    *   creates a matrix deep copy of matrix rhs
    *
    *   \param[in] rhs
    *   [Mat2i const&] Mat2i to copy from
    *******************************************************************************/

    Mat2i::Mat2i(Mat2i const& rhs) : rows(rhs.GetRows()), cols(rhs.GetCols())
    {
        data = new int* [rows];

        for (size_t r = 0; r < rows; ++r)
            data[r] = new int[cols];

        for (size_t r = 0; r < rows; ++r)
        {
            for (size_t c = 0; c < cols; ++c)
            {
                int temp = rhs[r][c];
                data[r][c] = temp;
            }
        }
    }
    /*!*****************************************************************************
    *   \brief
    *   Mat2i class move constructor
    *   moves rhs to new matrix via move semantics
    *
    *   \param[in] rhs
    *   [Mat2i const&] Mat2i to move from
    *******************************************************************************/

    //Mat2i::Mat2i(Mat2i&& rhs) noexcept : rows(rhs.GetRows()), cols(rhs.GetCols()), data(rhs.UseData())
    //{
    //    rhs.SetData(nullptr);
    //    rhs.SetRows(0);
    //    rhs.SetCols(0);
    //}

    /*!*****************************************************************************
    *   \brief
    *   Mat2i class arg constructor
    *   creates a nr x nc matrix from an initializer list that has nr rows and nc columns if possible
    *
    *   \param[in] list
    *   [std::initializer_list<std::initializer_list<int>>&] 2D Initializer list to construct from
    *******************************************************************************/

    Mat2i::Mat2i(std::initializer_list<std::initializer_list<int>>& list) : rows(list.size()), cols(list.begin()->size())
    {
        for (size_t i = 0; i < rows; ++i)
            if ((list.begin() + i)->size() != cols) throw std::runtime_error("bad initializer list");

        data = new int* [rows];

        for (size_t r = 0; r < rows; ++r)
            data[r] = new int[cols];

        for (size_t r = 0; r < rows; ++r)
        {
            for (size_t c = 0; c < cols; ++c)
            {
                data[r][c] = *((list.begin() + r)->begin() + c);
            }
        }
    }

    /*!*****************************************************************************
    *   \brief
    *   Mat2i class destructor
    *   destroys the matrix by explicitly returning storage to free store
    *******************************************************************************/

    Mat2i::~Mat2i() noexcept
    {
        for (size_t r = 0; r < rows; ++r)
        {
            delete[] data[r];
        }

        delete[] data;
    }

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

    Mat2i& Mat2i::operator=(Mat2i const& rhs)
    {
        this->~Mat2i();

        rows = rhs.GetRows();
        cols = rhs.GetCols();

        data = new int* [rows];

        for (size_t r = 0; r < rows; ++r)
            data[r] = new int[cols];

        for (size_t r = 0; r < rows; ++r)
        {
            for (size_t c = 0; c < cols; ++c)
            {
                int temp = rhs[r][c];
                data[r][c] = temp;
            }
        }

        return *this;
    }

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

    //Mat2i& Mat2i::operator=(Mat2i&& rhs) noexcept
    //{
    //    this->~Mat2i();

    //    rows = rhs.GetRows();
    //    cols = rhs.GetCols();

    //    data = rhs.UseData();

    //    rhs.SetData(nullptr);
    //    rhs.SetRows(0);
    //    rhs.SetCols(0);

    //    return *this;
    //}

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

    Mat2i& Mat2i::operator*=(Mat2i const& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    /*!*****************************************************************************
    *   \brief
    *   Fills matrix with value
    *
    *   \param[in] val
    *   [int const&] Value to fill Mat2i with
    *******************************************************************************/

    void Mat2i::Fill(int const& val)
    {
        for (size_t r = 0; r < rows; ++r)
        {
            for (size_t c = 0; c < cols; ++c)
            {
                data[r][c] = val;
            }
        }
    }

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

    std::pair<size_t, size_t> Mat2i::FindFirst(int const& value) const
    {
        for (size_t r{}; r < rows; ++r)
        {
            for (size_t c{}; c < cols; ++c)
            {
                if (data[r][c] == value) return { r, c };
            }
        }

        return { rows, cols };
    }

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

    size_t const Mat2i::GetCount(int const& value) const
    {
        size_t count{};
        for (size_t r = 0; r < 3; r++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                if (value == data[r][c]) count++;
            }
        }
        return count;
    }

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

    Mat2i const operator+(Mat2i const& M, Mat2i const& N)
    {
        if (M.GetRows() != N.GetRows() || M.GetCols() != N.GetCols())
            throw std::runtime_error("operands for matrix addition must have same dimensions");

        Mat2i m(M.GetRows(), M.GetCols());

        for (size_t r = 0; r < m.GetRows(); ++r)
        {
            for (size_t c = 0; c < m.GetCols(); ++c)
            {
                m[r][c] = M[r][c] + N[r][c];
            }
        }

        return m;
    }

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

    Mat2i const operator-(Mat2i const& M, Mat2i const& N)
    {
        if (M.GetRows() != N.GetRows() || M.GetCols() != N.GetCols())
            throw std::runtime_error("operands for matrix subtraction must have same dimensions");

        Mat2i m(M.GetRows(), M.GetCols());

        for (size_t r = 0; r < m.GetRows(); ++r)
        {
            for (size_t c = 0; c < m.GetCols(); ++c)
            {
                m[r][c] = M[r][c] - N[r][c];
            }
        }

        return m;
    }

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

    Mat2i const operator*(Mat2i const& M, Mat2i const& N)
    {
        if (M.GetCols() != N.GetRows())
            throw std::runtime_error("number of columns in left operand must match number of rows in right operand");

        Mat2i m(M.GetRows(), N.GetCols());

        for (size_t r = 0; r < m.GetRows(); ++r)
        {
            for (size_t c = 0; c < m.GetCols(); ++c)
            {
                int&& temp{};
                for (size_t k = 0; k < M.GetCols(); ++k)
                {
                    temp += M[r][k] * N[k][c];
                }

                m[r][c] = temp;
            }
        }

        return m;
    }

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

    Mat2i const operator*(int const& r, Mat2i const& N)
    {
        Mat2i m(N);

        for (size_t i = 0; i < m.GetRows(); ++i)
        {
            for (size_t j = 0; j < m.GetCols(); ++j)
            {
                m[i][j] *= r;
            }
        }

        return m;
    }

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

    bool operator==(Mat2i const& M, Mat2i const& N)
    {
        if (M.GetRows() != N.GetRows() || M.GetCols() != N.GetCols()) return false;
        for (size_t r = 0; r < M.GetRows(); ++r)
        {
            for (size_t c = 0; c < M.GetCols(); ++c)
            {
                if (M[r][c] != N[r][c]) return false;
            }
        }
        return true;
    }

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

    bool operator!=(Mat2i const& M, Mat2i const& N)
    {
        if (M.GetRows() != N.GetRows() || M.GetCols() != N.GetCols()) return true;
        for (size_t r = 0; r < M.GetRows(); ++r)
        {
            for (size_t c = 0; c < M.GetCols(); ++c)
            {
                if (M[r][c] != N[r][c]) return true;
            }
        }
        return false;
    }
}