/******************************************************************************/
/*!
\file   Geometry.h
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Header file for geometry operations

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Vector2D.h"

namespace PopplioMath
{
    /*!*****************************************************************************
    \brief
    2D Quadrilateral object (Geometry)
    *******************************************************************************/
    class Quad2D
    {
        private:

        Vec2f pos, scale, max, min;
        float rot;

        Vec2f const& GetMax();
        Vec2f const& GetMin();

        public:

        Quad2D();
        Quad2D(Vec2f const& position, Vec2f const& size, float const& rotation);

        Vec2f const& GetPos() const;
        Vec2f const& GetScale() const;
        float const& GetRot() const;

        void SetPos(Vec2f const& value);
        void SetScale(Vec2f const& value);
        void SetRot(float const& value);

        /*!*****************************************************************************
        \brief
        Computes if point is within quad using AABB

        \param[in] point
        [Vector2D const&] point in (x,y) coordinates

        \return
        [bool const&] Boolean if point is within quad
        *******************************************************************************/
        bool const AABB(Vec2f const& point) const;

        /*!*****************************************************************************
        \brief
        Computes if point is within quad using OBB

        \param[in] point
        [Vector2D const&] point in (x,y) coordinates

        \return
        [bool const&] Boolean if point is within quad
        *******************************************************************************/
        bool const OBB(Vec2f const& point) const;
    };

    /******************************************************************************/

    class Circle2D
    {
        private:

        Vec2f pos;
        float r;

        public:

        Circle2D();
        Circle2D(Vec2f const& position, float const& radius);

        Vec2f const& GetPos() const;
        float const& GetR() const;

        void SetPos(Vec2f const& value);
        void SetR(float const& value);

        /*!*****************************************************************************
        \brief
        Computes if point is within circle

        \param[in] point
        [Vector2D const&] point in (x,y) coordinates

        \return
        [bool const&] Boolean if point is within circle
        *******************************************************************************/
        bool const IsContain(Vec2f const& point) const;

        /*!*****************************************************************************
        \brief
        Computes if this circle intersects the other circle

        \param[in] circle
        [Circle2D const&] the other circle

        \return
        [bool const&] Boolean if intersection occurs
        *******************************************************************************/
        bool const IsIntersecting(Circle2D const& circle) const;
    };

    /******************************************************************************/

    // General

    float const DistSq2D(Vec2f const& a, Vec2f const& b);

    /*!*****************************************************************************
    \brief
    Computes distance between 2 given points

    \param[in] a , b
    [Vector2D const&] position of points

    \return
    [float const&] Distance between 2 points
    *******************************************************************************/
    float const Dist2D(Vec2f const& a, Vec2f const& b);
}