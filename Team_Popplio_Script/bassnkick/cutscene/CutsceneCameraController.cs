/******************************************************************************/
/*!
\file   CutsceneCameraController.cs
\author Team Popplio
\author Val Tay Yun Ying
\contribution Val - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/06 09:18:00 AM (Last Edit)
\brief
        C# file for cutscene camera controller

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Team_Popplio.Extensions;
using Team_Popplio.Extern;

namespace BassNKick
{
    public class CutsceneCameraController : PopplioMonoScript
    {
        public float originalCamPosX = 0f;
        public float originalCamPosY = 0f;

        [Inspector("Camera Default Zoom")]
        public float originalCamZoom = 560f;

        [Inspector("Time per Camera Movement")]
        public float tweenTime = 1.0f;


        private bool isMoving = false;
        public bool isDone = false;
        private Vector2<float> targetPos = Vector2<float>.Zero;
        private float targetZoom = 0;
        private float currTime = 0;
        private float startTime = 0;
        private float endTime = 0;
        private float zoomOutTime = 0;      // time where should zoom out the camera (relative to the TweenTime)
        private float zoomInTime = 0;       // time where should zoom in the camera (relative to the TweenTime)

        public void SetCameraTarget(Vector2<float> camPos, float camZoom)
        {
            Logger.Info($"Setting camera target {camPos}, {camZoom}");
            targetZoom = camZoom;
            targetPos = camPos;
            currTime = startTime = Time.ElapsedTimeF;
            endTime = Time.ElapsedTimeF + tweenTime;

            isMoving = true;
            isDone = false;
            //Camera.Zoom = camZoom;
            //Camera.Position = camPos;
        }

        public void ResetCamera()
        {
            Camera.Zoom = originalCamZoom;
            Camera.Position = new Vector2<float>(originalCamPosX, originalCamPosY);
        }

        // timeNow is normalized between the timeStart and timeEnd to zoom
        public float CamZoomIn(float timeNow, float target)
        {
            float toHalf = timeNow /** 0.5f*/;
            double whereAmI = Math.Sin(toHalf) * target;

            return (float)whereAmI;

        }
        public float CamZoomOut(float timeNow, float target)
        {
            float toHalf = timeNow /** 0.5f + 0.5f*/;
            double whereAmI = Math.Sin(toHalf) * target;
            Logger.Info($"{whereAmI}, {target}");

            return (float)whereAmI;

        }

        public float ZoomCamera(float zoom, float normalizedTime)
        {
            float difference = zoom - Camera.Zoom;
            return Camera.Zoom + difference * normalizedTime;
        }

        public Vector2<float> MoveCamera(Vector2<float> target, float normalizedTime)
        {
            Vector2<float> difference = target - Camera.Position;

            return new Vector2<float>(Camera.Position.X + difference.X * normalizedTime, Camera.Position.Y + difference.Y * normalizedTime);
        }

        public override void Setup()
        {
            zoomOutTime = tweenTime * 0.1f;
            zoomInTime = tweenTime * 0.9f;
        }

        public override void FixedUpdate()
        {
            if (!isMoving) return;

            if (isDone) return;

            currTime += Time.FixedDeltaTimeF;

            if (currTime > endTime)
            {
                isDone = true;
                isMoving = false;
            }
            float timeNormalized = (currTime - startTime) / tweenTime;
            //if (currTime < startTime + zoomOutTime)
            //{
            //    //zoom out
            //    Camera.Zoom = CamZoomOut(timeNormalized, originalCamZoom);
            //    Camera.Position = MoveCamera(new Vector2<float>(originalCamPosX, originalCamPosY), timeNormalized);

            //}
            //else if (currTime > startTime + zoomInTime)
            //{
            //    // zoom in
            //    Camera.Zoom = CamZoomIn(timeNormalized, targetZoom);

            //}
            //else
            //{
            Camera.Position = MoveCamera(targetPos, timeNormalized);
            Camera.Zoom = ZoomCamera(targetZoom, timeNormalized);
            //}
        }
    }
}
