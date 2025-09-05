/******************************************************************************/
/*!
\file   M3x3i.cpp
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
    //void M3x3i::AssignMTX() // assign mtx
    //{
    //    mtx[0] = &r0[0];
    //    mtx[1] = &r0[1];
    //    mtx[2] = &r0[2];
    //    mtx[3] = &r1[0];
    //    mtx[4] = &r1[1];
    //    mtx[5] = &r1[2];
    //    mtx[6] = &r2[0];
    //    mtx[7] = &r2[1];
    //    mtx[8] = &r2[2];
    //}

    /*!*****************************************************************************
    *   \brief
    *   M3x3i class arg constructor
    *   creates a matrix with 3 rows and 3 columns
    *******************************************************************************/

    M3x3i::M3x3i() : rows(3), cols(3) {}

    /*!*****************************************************************************
    *   \brief
    *   M3x3i class copy constructor
    *   creates a matrix deep copy of matrix rhs
    *
    *   \param[in] rhs
    *   [M3x3i const&] M3x3i to copy from
    *******************************************************************************/

    M3x3i::M3x3i(M3x3i const& rhs) : rows(3), cols(3)
    {
        //data[0] = r0;
        //data[1] = r1;
        //data[2] = r2;

        for (size_t r = 0; r < rows; ++r)
        {
            for (size_t c = 0; c < cols; ++c)
            {
                int temp = rhs[r][c];
                data[r][c] = temp;
            }
        }

        //AssignMTX();
    }

    /*!*****************************************************************************
    *   \brief
    *   M3x3i class move constructor
    *   moves rhs to new matrix via move semantics
    *
    *   \param[in] rhs
    *   [M3x3i const&] M3x3i to move from
    *******************************************************************************/

    //M3x3i::M3x3i(M3x3i&& rhs) noexcept : rows(rhs.GetRows()), cols(rhs.GetCols()), data(rhs.UseData())
    //{
    //    rhs.SetData(nullptr);
    //    rhs.SetRows(0);
    //    rhs.SetCols(0);
    //}

    /*!*****************************************************************************
    *   \brief
    *   M3x3i class arg constructor
    *   creates a nr x nc matrix from an initializer list that has nr rows and nc columns if possible
    *
    *   \param[in] list
    *   [std::initializer_list<std::initializer_list<int>>&] 2D Initializer list to construct from
    *******************************************************************************/

    M3x3i::M3x3i(std::initializer_list<std::initializer_list<int>>& list) : rows(3), cols(3)
    {
        if (list.size() != rows)
            std::runtime_error("M3x3i: initializer_list must be 3 x 3"); // throw exception + destroy this

        for (size_t i = 0; i < rows; ++i)
            if ((list.begin() + i)->size() != cols)
                std::runtime_error("M3x3i: initializer_list must be 3 x 3"); // throw exception + destroy this

        //data[0] = r0;
        //data[1] = r1;
        //data[2] = r2;

        for (size_t r = 0; r < rows; ++r)
        {
            for (size_t c = 0; c < cols; ++c)
            {
                data[r][c] = *((list.begin() + r)->begin() + c);
            }
        }

        //AssignMTX();
    }

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

    M3x3i& M3x3i::operator=(M3x3i const& rhs)
    {
        this->~M3x3i();

        //data[0] = r0;
        //data[1] = r1;
        //data[2] = r2;

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
    *   M3x3i move assignment operator
    *   replaces matrix with rhs
    *
    *   \param[in] rhs
    *   [M3x3i &&] 2D matrix to move from
    *
    *   \return
    *   [M3x3i&] this matrix
    *******************************************************************************/

    //M3x3i& M3x3i::operator=(M3x3i&& rhs) noexcept
    //{
    //    this->~M3x3i();

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
    *   M3x3i compound multiplication assignment operator
    *   replaces matrix with matrix * rhs
    *
    *   \param[in] rhs
    *   [M3x3i &] 2D matrix to multiply with
    *
    *   \return
    *   [M3x3i&] this matrix
    *******************************************************************************/

    M3x3i& M3x3i::operator*=(M3x3i const& rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    /*!*****************************************************************************
    *   \brief
    *   Fills matrix with value
    *
    *   \param[in] val
    *   [int const&] Value to fill M3x3i with
    *******************************************************************************/

    void M3x3i::Fill(int const& val)
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

    std::pair<size_t, size_t> const M3x3i::FindFirst(int const& value) const
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

    size_t const M3x3i::GetCount(int const& value) const
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

    M3x3i const operator+(M3x3i const& M, M3x3i const& N)
    {
        M3x3i m{};

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

    M3x3i const operator-(M3x3i const& M, M3x3i const& N)
    {
        M3x3i m{};

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

    M3x3i const operator*(M3x3i const& M, M3x3i const& N)
    {
        M3x3i m{};

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

    M3x3i const operator*(int const& r, M3x3i const& N)
    {
        M3x3i m(N);

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

    bool operator==(M3x3i const& M, M3x3i const& N)
    {
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

    bool operator!=(M3x3i const& M, M3x3i const& N)
    {
        for (size_t r = 0; r < M.GetRows(); ++r)
        {
            for (size_t c = 0; c < M.GetCols(); ++c)
            {
                if (M[r][c] != N[r][c]) return true;
            }
        }
        return false;
    }

    M3x3i::M3x3i(const int* pArr) : M3x3i() // array constructor
    {
        // todo: undefined behaviour if < 9 or > 9

        r0[0] = pArr[0];
        r0[1] = pArr[1];
        r0[2] = pArr[2];
        r1[0] = pArr[3];
        r1[1] = pArr[4];
        r1[2] = pArr[5];
        r2[0] = pArr[6];
        r2[1] = pArr[7];
        r2[2] = pArr[8];

        //AssignMTX();
    }


    M3x3i::M3x3i(int const& m00, int const& m01, int const& m02,
        int const& m10, int const& m11, int const& m12,
        int const& m20, int const& m21, int const& m22) : M3x3i() // 9 value constructor
    {
        r0[0] = m00;
        r0[1] = m01;
        r0[2] = m02;
        r1[0] = m10;
        r1[1] = m11;
        r1[2] = m12;
        r2[0] = m20;
        r2[1] = m21;
        r2[2] = m22;

        //AssignMTX();
    }


    int& M3x3i::MTX(size_t const& i) // get mtx
    {
        if (i > 8) throw std::runtime_error("M3x3i: accessing index greater than 8"); // throw exception

        if (mtx == nullptr) throw std::runtime_error("M3x3i: accessing nullptr"); // throw exception

        return mtx[i];
    }

    /**************************************************************************/
    /*!
        This function sets the matrix to the identity matrix
    */
    /**************************************************************************/

    void M3x3i::Identity()
    {
        (*this)[0][0] = 1;
        (*this)[0][1] = 0;
        (*this)[0][2] = 0;
        (*this)[1][0] = 0;
        (*this)[1][1] = 1;
        (*this)[1][2] = 0;
        (*this)[2][0] = 0;
        (*this)[2][1] = 0;
        (*this)[2][2] = 1;
    }
    /**************************************************************************/
    /*!
        This function creates a translation matrix from x & y
    */
    /**************************************************************************/

    void M3x3i::Translate(int const& x, int const& y)
    {
        this->Identity();

        (*this)[0][2] = x;
        (*this)[1][2] = y;
    }
    /**************************************************************************/
    /*!
        This function creates a scaling matrix from x & y
    */
    /**************************************************************************/

    void M3x3i::Scale(int const& x, int const& y)
    {
        this->Identity();

        (*this)[0][0] = x;
        (*this)[1][1] = y;
    }
    /**************************************************************************/
    /*!
        This matrix creates a rotation matrix from "radAngle" whose value is in radian
    */
    /**************************************************************************/

    void M3x3i::RotRad(int const& radAngle)
    {
        this->Identity();

        (*this)[0][0] = static_cast<int>(cosf(static_cast<float>(radAngle)));
        (*this)[1][0] = static_cast<int>(sinf(static_cast<float>(radAngle)));
        (*this)[0][1] = static_cast<int>(-sinf(static_cast<float>(radAngle)));
        (*this)[1][1] = static_cast<int>(cosf(static_cast<float>(radAngle)));
    }
    /**************************************************************************/
    /*!
        This matrix creates a rotation matrix from "radAngle" whose value is in degree
    */
    /**************************************************************************/

    void M3x3i::RotDeg(int const& radAngle)
    {
        this->Identity();

        (*this)[0][0] = static_cast<int>(cosf(static_cast<float>(radAngle) * M_PI / 180.f));
        (*this)[1][0] = static_cast<int>(sinf(static_cast<float>(radAngle) * M_PI / 180.f));
        (*this)[0][1] = static_cast<int>(-sinf(static_cast<float>(radAngle) * M_PI / 180.f));
        (*this)[1][1] = static_cast<int>(cosf(static_cast<float>(radAngle) * M_PI / 180.f));
    }
    /**************************************************************************/
    /*!
        This function calculates the transpose matrix
    */
    /**************************************************************************/

    void M3x3i::Transpose()
    {
        const M3x3i temp(*this);

        for (size_t r{}; r < 3; ++r)
        {
            for (size_t c{}; c < 3; ++c)
            {
                (*this)[c][r] = temp[r][c];
            }
        }
    }
    /**************************************************************************/
    /*!
        This function calculates the inverse matrix. If the matrix inversion fails, return false.
    */
    /**************************************************************************/

    bool M3x3i::Inverse(int& determinant)
    {
        determinant =
            (((*this)[0][0] * (*this)[1][1] * (*this)[2][2]) + ((*this)[0][1] * (*this)[1][2] * (*this)[2][0]) + ((*this)[0][2] * (*this)[1][0] * (*this)[2][1])) -
            (((*this)[2][0] * (*this)[1][1] * (*this)[0][2]) + ((*this)[2][1] * (*this)[1][2] * (*this)[0][0]) + ((*this)[2][2] * (*this)[1][0] * (*this)[0][1]));

        if (determinant == 0) return false;

        M3x3i CofactorMtx;

        CofactorMtx[0][0] = ((*this)[1][1] * (*this)[2][2]) - ((*this)[1][2] * (*this)[2][1]);
        CofactorMtx[0][1] = -(((*this)[1][0] * (*this)[2][2]) - ((*this)[1][2] * (*this)[2][0]));
        CofactorMtx[0][2] = ((*this)[1][0] * (*this)[2][1]) - ((*this)[1][1] * (*this)[2][0]);
        CofactorMtx[1][0] = -(((*this)[0][1] * (*this)[2][2]) - ((*this)[0][2] * (*this)[2][1]));
        CofactorMtx[1][1] = ((*this)[0][0] * (*this)[2][2]) - ((*this)[0][2] * (*this)[2][0]);
        CofactorMtx[1][2] = -(((*this)[0][0] * (*this)[2][1]) - ((*this)[0][1] * (*this)[2][0]));
        CofactorMtx[2][0] = ((*this)[0][1] * (*this)[1][2]) - ((*this)[0][2] * (*this)[1][1]);
        CofactorMtx[2][1] = -(((*this)[0][0] * (*this)[1][2]) - ((*this)[0][2] * (*this)[1][0]));
        CofactorMtx[2][2] = ((*this)[0][0] * (*this)[1][1]) - ((*this)[0][1] * (*this)[1][0]);

        CofactorMtx.Transpose();

        for (size_t i = 0; i < 9; ++i)
        {
            MTX(i) = CofactorMtx.MTX(i) / determinant;
        }

        return true;
    }

    /*****************************************************************************/

    // Global functions

    /**************************************************************************/
    /*!
        This function sets the matrix pResult to the identity matrix
    */
    /**************************************************************************/

    void M3x3iIdentity(M3x3i& pResult)
    {
        pResult[0][0] = 1;
        pResult[0][1] = 0;
        pResult[0][2] = 0;
        pResult[1][0] = 0;
        pResult[1][1] = 1;
        pResult[1][2] = 0;
        pResult[2][0] = 0;
        pResult[2][1] = 0;
        pResult[2][2] = 1;
    }

    /**************************************************************************/
    /*!
        This function creates a translation matrix from x & y
        and saves it in pResult
    */
    /**************************************************************************/

    void M3x3iTranslate(M3x3i& pResult, int const& x, int const& y)
    {
        pResult.Identity();

        pResult[0][2] = x;
        pResult[1][2] = y;
    }

    /**************************************************************************/
    /*!
        This function creates a scaling matrix from x & y
        and saves it in pResult
    */
    /**************************************************************************/

    void M3x3iScale(M3x3i& pResult, int const& x, int const& y)
    {
        pResult.Identity();

        pResult[0][0] = x;
        pResult[1][1] = y;
    }

    /**************************************************************************/
    /*!
        This matrix creates a rotation matrix from "radAngle" whose value
        is in radian. Save the resultant matrix in pResult.
    */
    /**************************************************************************/

    void M3x3iRotRad(M3x3i& pResult, int const& radAngle)
    {
        pResult.Identity();

        pResult[0][0] = static_cast<int>(cosf(static_cast<float>(radAngle)));
        pResult[1][0] = static_cast<int>(sinf(static_cast<float>(radAngle)));
        pResult[0][1] = static_cast<int>(-sinf(static_cast<float>(radAngle)));
        pResult[1][1] = static_cast<int>(cosf(static_cast<float>(radAngle)));
    }

    /**************************************************************************/
    /*!
        This matrix creates a rotation matrix from "radAngle" whose value
        is in degree. Save the resultant matrix in pResult.
    */
    /**************************************************************************/

    void M3x3iRotDeg(M3x3i& pResult, int const& radAngle)
    {
        pResult.Identity();

        pResult[0][0] = static_cast<int>(cosf(static_cast<float>(radAngle) * M_PI / 180.f));
        pResult[1][0] = static_cast<int>(sinf(static_cast<float>(radAngle) * M_PI / 180.f));
        pResult[0][1] = static_cast<int>(-sinf(static_cast<float>(radAngle) * M_PI / 180.f));
        pResult[1][1] = static_cast<int>(cosf(static_cast<float>(radAngle) * M_PI / 180.f));
    }

    /**************************************************************************/
    /*!
        This functions calculated the transpose matrix of pMtx
        and saves it in pResult
    */
    /**************************************************************************/

    void M3x3iTranspose(M3x3i& pResult, const M3x3i& pMtx)
    {
        const M3x3i mtx(pMtx);

        for (size_t r{}; r < 3; ++r)
        {
            for (size_t c{}; c < 3; ++c)
            {
                pResult[c][r] = mtx[r][c];
            }
        }
    }

    /**************************************************************************/
    /*!
        This function calculates the inverse matrix of pMtx and saves the
        result in pResult. If the matrix inversion fails, pResult
        would be set to NULL.
    */
    /**************************************************************************/

    void M3x3iInverse(M3x3i* pResult, int& determinant, const M3x3i& pMtx)
    {
        determinant =
            ((pMtx[0][0] * pMtx[1][1] * pMtx[2][2]) + (pMtx[0][1] * pMtx[1][2] * pMtx[2][0]) + (pMtx[0][2] * pMtx[1][0] * pMtx[2][1])) -
            ((pMtx[2][0] * pMtx[1][1] * pMtx[0][2]) + (pMtx[2][1] * pMtx[1][2] * pMtx[0][0]) + (pMtx[2][2] * pMtx[1][0] * pMtx[0][1]));

        if (determinant == 0)
        {
            pResult = NULL;
            return;
        }

        M3x3i CofactorMtx;

        CofactorMtx[0][0] = (pMtx[1][1] * pMtx[2][2]) - (pMtx[1][2] * pMtx[2][1]);
        CofactorMtx[0][1] = -((pMtx[1][0] * pMtx[2][2]) - (pMtx[1][2] * pMtx[2][0]));
        CofactorMtx[0][2] = (pMtx[1][0] * pMtx[2][1]) - (pMtx[1][1] * pMtx[2][0]);
        CofactorMtx[1][0] = -((pMtx[0][1] * pMtx[2][2]) - (pMtx[0][2] * pMtx[2][1]));
        CofactorMtx[1][1] = (pMtx[0][0] * pMtx[2][2]) - (pMtx[0][2] * pMtx[2][0]);
        CofactorMtx[1][2] = -((pMtx[0][0] * pMtx[2][1]) - (pMtx[0][1] * pMtx[2][0]));
        CofactorMtx[2][0] = (pMtx[0][1] * pMtx[1][2]) - (pMtx[0][2] * pMtx[1][1]);
        CofactorMtx[2][1] = -((pMtx[0][0] * pMtx[1][2]) - (pMtx[0][2] * pMtx[1][0]));
        CofactorMtx[2][2] = (pMtx[0][0] * pMtx[1][1]) - (pMtx[0][1] * pMtx[1][0]);

        CofactorMtx.Transpose();

        for (size_t i = 0; i < 9; ++i)
        {
            pResult->MTX(i) = CofactorMtx.MTX(i) / determinant;
        }
    }
}