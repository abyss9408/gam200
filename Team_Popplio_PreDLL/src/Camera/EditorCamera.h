/******************************************************************************/
/*!
\file   EditorCamera.h
\author Team Popplio
\author Bryan Ang Wei Ze
\co-author Pearl Goh
\contribution Bryan - 10%
\contribution Pearl - 90%
\par    Course : CSD2401
\par    Section : A
\date   2024/11/07
\brief
    This header file defines the EditorCamera class within the Popplio namespace. 
    The EditorCamera class is responsible for managing the editor camera's position, 
    zoom level, and the view and projection matrices used in the editor.

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "../Math/MatVec.h"

namespace Popplio
{
    /**
     * \class EditorCamera
     * \brief Class responsible for managing the camera's position, zoom level, and the view and projection matrices used in the editor.
     */
    class EditorCamera
    {
    public:
        /**
         * \brief Constructor for EditorCamera class.
         * \param width The width of the viewport.
         * \param height The height of the viewport.
         */
        EditorCamera(int width, int height);

        /**
         * \brief Sets the position of the camera.
         * \param pos The new position of the camera.
         */
        void SetPosition(const PopplioMath::Vec2f& pos);

        /**
         * \brief Sets the zoom level of the camera.
         * \param newZoom The new zoom level of the camera.
         */
        void SetZoom(float newZoom);

        /**
         * \brief Gets the position of the camera.
         * \return The position of the camera.
         */
        PopplioMath::Vec2f GetPosition() const;

        /**
         * \brief Gets the zoom level of the camera.
         * \return The zoom level of the camera.
         */
        float GetZoom() const;

        /**
         * \brief Gets the view matrix of the camera.
         * \return The view matrix of the camera.
         */
        PopplioMath::M3x3f GetViewMatrix() const;

        /**
         * \brief Gets the projection matrix of the camera.
         * \return The projection matrix of the camera.
         */
        PopplioMath::M3x3f GetProjectionMatrix() const;


    private:
        PopplioMath::Vec2f position; ///< The position of the camera.
        float zoom; ///< The zoom level of the camera.
        float aspectRatio; ///< The aspect ratio of the viewport.
        int viewportWidth; ///< The width of the viewport.
        int viewportHeight; ///< The height of the viewport.
        PopplioMath::M3x3f viewMatrix; ///< The view matrix of the camera.
        PopplioMath::M3x3f projectionMatrix; ///< The projection matrix of the camera.

        /**
         * \brief Updates the view matrix of the camera.
         */
        void UpdateViewMatrix();

        /**
         * \brief Updates the projection matrix of the camera.
         */
        void UpdateProjectionMatrix();
    };
}