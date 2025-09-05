/******************************************************************************/
/*!
\file   CutsceneManager.cs
\author Team Popplio
\author Val Tay Yun Ying
\contribution Val - 100%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2025/04/06 02:54:00 PM (Last Edit)
\brief
        C# file for cutscene manager

Copyright (C) 2024 - 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

using BassNKick.Lib;
using Mono.Linker;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Team_Popplio.Extern;

namespace BassNKick
{
    public struct OneCutscene
    {
        public string name;
        public string texture;
        public bool hasNextImage;

        // level is -1 if does not go to game scene
        public int level;

        public Queue<Vector2<float>> cameraPointPosList;
        public Queue<float> cameraZoomList;
        public Queue<string> dialogueQueueList;
        public Queue<List<string>> audioQueueList;
    };

    public class CutsceneManager : PopplioMonoScript
    {
        // inspector variables
        [Inspector("Cutscenes file")]
        public string cutsceneFile = "";

        [Inspector("Cutscene Image Obj")]
        public Entity cutsceneImageObj = Entity.EntityField();
        Render2D? cutsceneImage;

        [Inspector("Cutscene name")]
        public string cutsceneToPlay = "Cutscene_1_Prolouge_1";

        public Entity cameraControllerObj = Entity.EntityField();
        CutsceneCameraController? cameraController;

        [Inspector("Cutscene Audio Obj")]
        public Entity cutsceneAudioController = Entity.EntityField();
        CutsceneAudioController? audioController;

        //public Entity dialogueUIObj = Entity.EntityField();
        //DialogueUI? dialogueUI;

        //[Inspector("Debug Text")]
        //public Entity textObj = Entity.EntityField();
        //Text2D? debugText;

        // private variables
        private List<OneCutscene> cutsceneList = new List<OneCutscene>();

        private OneCutscene? currCutscene;
        private int currCutsceneID = 0;

        private float bufferTime = 0.0f;
        private float inputBufferTime = 0.1f;

        private int isSpecialTime = -1;

        //private Queue<Vector2<float>>? cameraPointQueue;

        private bool goNext = false;

        private void LoadCutscene()
        {
            Logger.Info("Loading cutscene!");
            if (cutsceneImage == null) return;

            if (StaticCutsceneData.PlayThisCutscene == "")
            {
                StaticCutsceneData.PlayThisCutscene = cutsceneToPlay;
            }

            currCutsceneID = 0;
            foreach (var scene in cutsceneList)
            {
                Logger.Info($"checking scene: {scene.name}");

                if (scene.name == StaticCutsceneData.PlayThisCutscene)
                {
                    Logger.Info("found scene! " + scene.name);
                    currCutscene = scene;
                    cutsceneImage.Texture = scene.texture;

                    break;
                }
                ++currCutsceneID;
            }

            if (currCutscene == null)
            {
                Logger.Info($"Unable to find scene: {StaticCutsceneData.PlayThisCutscene}");
                return;
            }

            //cutsceneImageObj.Transform2D.Translate(currCutscene.Value.cameraPointPosList.Dequeue());
            //Camera.Position = currCutscene.Value.cameraPointPosList.Dequeue();
            if (cameraController == null) return;

            //cameraController.ResetCamera();
            cameraController.SetCameraTarget(currCutscene.Value.cameraPointPosList.Dequeue(), currCutscene.Value.cameraZoomList.Dequeue());

            if (audioController == null)
            {
                return;
            }

            if (currCutscene.Value.audioQueueList.Count > 0)
            {
                if (currCutscene.Value.audioQueueList.First().Count() > 0 &&
                currCutscene.Value.audioQueueList.First()[0] == "special")
                {
                    isSpecialTime = 0;
                    audioController.PlayBackingTrack();
                    return;

                }
                List<string> audios = currCutscene.Value.audioQueueList.Dequeue();

                foreach (var audio in audios)
                {
                    string[] bgmChecker = audio.Split('|');

                    if (bgmChecker.Length > 1)
                        audioController.PlayAudio(bgmChecker[0], true);
                    else
                        audioController.PlayAudio(audio);
                }
            }
        }

        public override void Setup()
        {
            cutsceneImage = cutsceneImageObj.GetComponent<Render2D>();
            cameraController = cameraControllerObj.GetScript<CutsceneCameraController>();
            audioController = cutsceneAudioController.GetScript<CutsceneAudioController>();
            //dialogueUI = dialogueUIObj.GetScript<DialogueUI>();

            try
            {
                if (Json.LoadJson(cutsceneFile) == IntPtr.Zero)
                    throw new Exception("Failed to get Json file: " + cutsceneFile);

                Logger.Critical("CutsceneManager.cs | Loading Cutscenes");

                IntPtr[] cutscenes = Json.LoadObjArray("cutscenes");

                foreach (var cutscene in cutscenes)
                {
                    Logger.Info("Loading a cutscene!");
                    OneCutscene newCutscene = new OneCutscene();
                    newCutscene.name = Json.LoadObjString(cutscene, "name");
                    newCutscene.texture = Json.LoadObjString(cutscene, "texture");
                    newCutscene.hasNextImage = Json.LoadObjBool(cutscene, "hasNextImage");
                    newCutscene.level = Json.LoadObjInt(cutscene, "level");

                    newCutscene.cameraPointPosList = new Queue<Vector2<float>>();
                    newCutscene.cameraZoomList = new Queue<float>();
                    string cutscenePointsString = Json.LoadObjString(cutscene, "camera points");

                    string[] points = cutscenePointsString.Split(' ');


                    foreach (var point in points)
                    {
                        if (point == "") break;

                        string[] xyPair = point.Split(',');

                        Vector2<float>? coord = new Vector2<float>();
                        coord.X = float.Parse(xyPair[0], CultureInfo.InvariantCulture.NumberFormat);
                        coord.Y = float.Parse(xyPair[1], CultureInfo.InvariantCulture.NumberFormat);
                        newCutscene.cameraPointPosList.Enqueue(coord);
                        newCutscene.cameraZoomList.Enqueue(float.Parse(xyPair[2], CultureInfo.InvariantCulture.NumberFormat));
                    }

                    newCutscene.audioQueueList = new Queue<List<string>>();
                    string cutsceneAudioString = Json.LoadObjString(cutscene, "sfx");

                    // audio is in format [num] "audio name", ... 
                    // eg 2 buttonClick ManaYell, 1 buttonClick,
                    string[] tokens = cutsceneAudioString.Split(',');
                    foreach (var token in tokens)
                    {
                        if (token == "") break;

                        if (token == "special")
                        {
                            newCutscene.audioQueueList.Enqueue(new List<string> { token });
                            continue;
                        }

                        string[] oneSceneAudio = token.Split(' ');

                        int howManySfx = int.Parse(oneSceneAudio[0]);

                        List<string> sfx = new List<string>();
                        for (int i = 1; i <= howManySfx; ++i)
                        {
                            sfx.Add(oneSceneAudio[i]);
                        }
                        Logger.Info($"{sfx.Count}");
                        newCutscene.audioQueueList.Enqueue(sfx);
                    }

                    cutsceneList.Add(newCutscene);

                    //Logger.Info("Loaded a cutscene!" + newCutscene.name);
                    //Logger.Info("Loaded a cutscene!" + newCutscene.texture);
                    //Logger.Info("Loaded a cutscene!" + newCutscene.hasNextImage);
                    //Logger.Info("Loaded a cutscene!" + newCutscene.level);
                    //Logger.Info("Loaded a cutscene!" + newCutscene.cameraPointPosList.Count);
                    //Logger.Info("Loaded a cutscene!" + newCutscene.cameraZoomList.Count);

                }

                Json.Close();
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
            }

            if (StaticCutsceneData.PlayThisCutscene == "")
                StaticCutsceneData.PlayThisCutscene = cutsceneToPlay;
        }

        public override void Init()
        {
            LoadCutscene();
            bufferTime = inputBufferTime;
        }

        public override void Update()
        {
            goNext = Input.IsKeyTriggered(GLFW.KEY_SPACE);
            if (goNext)
            {
                GoNext();
            }
        }

        void GoNext()
        {
            if (currCutscene == null) return;

            if (currCutscene.Value.cameraPointPosList.Count > 0)
            {
                //cutsceneImageObj.Transform2D.Translate(currCutscene.Value.cameraPointPosList.Dequeue());
                if (cameraController == null) return;

                foreach (var pos in currCutscene.Value.cameraPointPosList)
                {
                    Logger.Info($"{pos}");
                }

                var nextPos = currCutscene.Value.cameraPointPosList.Dequeue();
                var nextZoom = currCutscene.Value.cameraZoomList.Dequeue();
                cameraController.SetCameraTarget(nextPos, nextZoom);

                if (audioController == null)
                {
                    return;
                }

                if (isSpecialTime > -1)
                {
                    ++isSpecialTime;

                    switch (isSpecialTime)
                    {
                        case 1:
                            audioController.PlayGuitarLouder();
                            break;
                        case 2:
                            audioController.PlayDrumsLouder();
                            break;
                        case 3:
                            audioController.EqualSpecial();
                            break;
                    }
                    return;
                }

                if (currCutscene.Value.audioQueueList.Count > 0)
                {
                    List<string> audios = currCutscene.Value.audioQueueList.Dequeue();
                    foreach (var audio in audios)
                    {
                        string[] bgmChecker = audio.Split('|');

                        if (bgmChecker.Length > 1)
                            audioController.PlayAudio(audio, true);
                        else
                            audioController.PlayAudio(audio);
                    }
                }
            }
            else
            {
                // stop all audio
                if (audioController != null)
                {
                    isSpecialTime = -1;
                    audioController.RefreshAudio();
                }

                if (currCutscene.Value.hasNextImage)
                {
                    //Logger.Info(currCutsceneID.ToString());

                    currCutscene = cutsceneList.ElementAt(currCutsceneID + 1);

                    StaticCutsceneData.PlayThisCutscene = currCutscene.Value.name;
                    LoadCutscene();
                    goNext = false;
                }
                else // load the game scene for which level 
                {
                    if (currCutscene.Value.level == -1)
                    {
                        // Load level select here
                        SceneManager.LoadScene("LevelSelect");
                        TempInputFix.ResetKeyCooldown();
                    }
                    else
                    {
                        //Logger.Critical("Loading level: " + currCutscene.Value.level.ToString());
                        // first time
                        //Logger.Critical("Loading level: " + Lib.Profile.playerNoob);
                        if (Lib.Profile.playerNoob)
                        {
                            // now first time is false
                            //Lib.Profile.Load();
                            if (Lib.Profile.playerNoob)
                            {
                                Lib.Profile.playerNoob = false;
                                Lib.Profile.Save();
                            }
                            // load tutorial
                            SceneManager.LoadScene("Tutorial");
                        }
                        else
                        {
                            // Load respective game level here
                            SceneManager.LoadScene($"Track{currCutscene.Value.level}", true);
                        }
                        TempInputFix.ResetKeyCooldown();
                    }
                }

            }
        }

        public override void FixedUpdate()
        {
            //Lib.TempInputFix.Update();

            //if (Input.IsMouseButtonTriggered(Input.MouseButton.Left))

            //if (Input.IsKeyTriggered(Keybinds.options)) SceneManager.LoadScene("OptionsMenu");

            //// debug
            //if (Input.IsKeyTriggered(Keybinds.mainMenu)) SceneManager.LoadScene("MainMenu");
            //if (Input.IsKeyTriggered(Keybinds.levelSelect)) SceneManager.LoadScene("LevelSelect");
            //if (Input.IsKeyTriggered(Keybinds.results)) SceneManager.LoadScene("Results");

            //if (Input.IsKeyTriggered(GLFW.KEY_4)) SceneManager.LoadScene("Track1");
            //if (Input.IsKeyTriggered(GLFW.KEY_5)) SceneManager.LoadScene("Track2");
            //if (Input.IsKeyTriggered(GLFW.KEY_6)) SceneManager.LoadScene("Track3");

            //if (Input.IsKeyTriggered(Keybinds.showFPS))
            //{
            //    Logger.Critical("FPS Counter Toggled");
            //    StaticSettings.showFPS = !StaticSettings.showFPS;
            //}
        }
    }
}
