/******************************************************************************/
/*!
\file   DialogueUI.cs
\author Team Popplio
\author Val Tay Yun Ying
\contribution Val - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/03/30 05:54:00 PM (Last Edit)
\brief
        C# file for Dialogue UI

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

namespace BassNKick
{
    public class DialogueUI : Team_Popplio.PopplioMonoScript
    {

        [Inspector("Dialogue Obj")]
        public Entity dialogueObj = Entity.EntityField();
        private Text2D? dialogueText;
        private Transform2D? dialogueTransform;

        public float tweenTime = 0.1f;
        private bool isTweening = false;
        private float countDown = 0f;

        public override void Setup()
        {
        }

        public override void Init()
        {
            dialogueText = dialogueObj.GetComponent<Text2D>();
            dialogueTransform = dialogueObj.GetComponent<Transform2D>();
        }

        public override void Update()
        {
            if (dialogueText == null) return;
            if (isTweening)
            {
                if (countDown < 0.0f)
                {
                    isTweening = false;
                    countDown = 0.0f;
                    dialogueText.Color.A = 1.0f;
                    return;
                }

                dialogueText.Color.A = 1.0f - (countDown / tweenTime);
                countDown -= Time.DeltaTimeF;
            }
        }

        public override void FixedUpdate()
        {

        }

        public override void End()
        {

        }
        public void SetDialogue(string dialogue, Vector2<float> dialogLocation)
        {
            if (dialogueText == null || dialogueTransform == null) return;

            dialogueText.Text = dialogue;
            dialogueTransform.Position = dialogLocation;
            dialogueText.Color.A = 0f;
            isTweening = true;
        }
    }
}