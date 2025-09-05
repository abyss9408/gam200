/******************************************************************************/
/*!
\file   Geometry.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\contribution Shawn - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        Source file for geometry operations

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#include <pch.h> // PCH

namespace PopplioMath
{
    // Quad2D

    Quad2D::Quad2D() : pos(Vec2f(0.f, 0.f)), scale(Vec2f(0.f, 0.f)), rot(0.f), 
        max(Vec2f(0.f, 0.f)), min(Vec2f(0.f, 0.f))
    {
        GetMax();
        GetMin();
    }
    Quad2D::Quad2D(Vec2f const& position, Vec2f const& size, float const& rotation) 
        : pos(position), scale(size), rot(rotation), max(Vec2f(0.f, 0.f)), min(Vec2f(0.f, 0.f))
    {
        GetMax();
        GetMin();
    }

    Vec2f const& Quad2D::GetPos() const { return Quad2D::pos; }
    Vec2f const& Quad2D::GetScale() const { return Quad2D::scale; }
    float const& Quad2D::GetRot() const { return Quad2D::rot; }

    void Quad2D::SetPos(Vec2f const& value) { Quad2D::pos = value; }
    void Quad2D::SetScale(Vec2f const& value) { Quad2D::scale = value; }
    void Quad2D::SetRot(float const& value) { Quad2D::rot = value; }

    Vec2f const& Quad2D::GetMax() 
    { 
        //todo: return AABB / OBB
        return Quad2D::max; 
    }
    Vec2f const& Quad2D::GetMin() 
    { 
        //todo: return AABB / OBB
        return Quad2D::min; 
    }

    /*!*****************************************************************************
    \brief
    Computes if point is within quad using AABB

    \param[in] point
    [Vector2D const&] point in (x,y) coordinates

    \return
    [bool const&] Boolean if point is within quad
    *******************************************************************************/
    bool const Quad2D::AABB(Vec2f const& point) const
    {
        if (!((point.x < Quad2D::min.x || point.y < Quad2D::min.y) ||
            (point.x > Quad2D::max.x || point.y > Quad2D::max.y)))
            return true;

        return false;
    }

    /*!*****************************************************************************
    \brief
    Computes if point is within quad using OBB

    \param[in] point
    [Vector2D const&] point in (x,y) coordinates

    \return
    [bool const&] Boolean if point is within quad
    *******************************************************************************/
    bool const Quad2D::OBB(Vec2f const& point) const
    {
        //todo: return OBB
        static_cast<void>(point);

        return false;
    }

    /******************************************************************************/

    // Circle2D

    Circle2D::Circle2D() : pos(Vec2f(0.f, 0.f)), r(0.f) {}
    Circle2D::Circle2D(Vec2f const& position, float const& radius) : pos(position), r(radius) {}

    Vec2f const& Circle2D::GetPos() const { return Circle2D::pos; }
    float const& Circle2D::GetR() const { return Circle2D::r; }

    void Circle2D::SetPos(Vec2f const& value) { Circle2D::pos = value; }
    void Circle2D::SetR(float const& value) { Circle2D::r = value; }

    /*!*****************************************************************************
    \brief
    Computes if point is within circle

    \param[in] point
    [Vector2D const&] point in (x,y) coordinates

    \return
    [bool const&] Boolean if point is within circle
    *******************************************************************************/
    bool const Circle2D::IsContain(Vec2f const& point) const
    {
        if (Dist2D(Circle2D::GetPos(), point) <= Circle2D::GetR() * Circle2D::GetR())
            return true;
        
        return false;
    }

    /*!*****************************************************************************
    \brief
    Computes if this circle intersects the other circle

    \param[in] circle
    [Circle2D const&] the other circle

    \return
    [bool const&] Boolean if intersection occurs
    *******************************************************************************/
    bool const Circle2D::IsIntersecting(Circle2D const& circle) const
    {
        if (Dist2D(Circle2D::GetPos(), circle.GetPos()) > Circle2D::GetR() * Circle2D::GetR() &&
            Dist2D(Circle2D::GetPos(), circle.GetPos()) > circle.GetR() * circle.GetR())
            return false;

        return true;
    }

    /******************************************************************************/

    // General

    float const DistSq2D(Vec2f const& a, Vec2f const& b)
    {
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
        //return powf(a.GetX() - b.GetX(), 2.f) + powf(a.GetY() - b.GetY(), 2.f);
    }

    /*!*****************************************************************************
    \brief
    Computes distance between 2 given points

    \param[in] a , b
    [Vector2D const&] position of points

    \return
    [float const&] Distance between 2 points
    *******************************************************************************/
    float const Dist2D(Vec2f const& a, Vec2f const& b)
    {
        return fabsf(sqrt(DistSq2D(a, b)));
    }
}