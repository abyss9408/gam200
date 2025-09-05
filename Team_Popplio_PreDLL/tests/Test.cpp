/******************************************************************************/
/*!
\file   Test.cpp
\author Team Popplio
\author Ng Jun Heng Shawn
\co-author  Bryan
\contribution Shawn - 40% | Bryan - 20%
\par    Course : CSD2401 / UXGD2400 / DAA2402
\par    Section : A
\date   2024/11/07 11:23:30 PM (Last Edit)
\brief
        This is the source file for engine tests

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/******************************************************************************/

// Includes
#include <pch.h>
#include "Test.h"

#include "../src/Input/InputSystem.h"
#include "../src/PerformanceViewer/PerformanceViewer.h"
#include "../src/Math/MathLib.h"

// -------------------------------------------------------------------- //

namespace PopplioTest
{
    // Shawn ----------------------------------------------- //
    
    //void fmodGuide()
    //{
    //    FMOD_RESULT result;
    //    FMOD::System* pSystem = nullptr;

    //    result = FMOD::System_Create(&pSystem);

    //    pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

    //    FMOD::Sound* pSound = nullptr;
    //    //Change this:
    //    pSystem->createSound(R"(../assets/audio/track.wav)", FMOD_DEFAULT, nullptr, &pSound);

    //    //std::cout << pSound << std::endl;

    //    FMOD::Channel* pChannel = nullptr;
    //    result = pSystem->playSound(pSound, nullptr, false, &pChannel);

    //    if (result == FMOD_OK)
    //    {
    //        Popplio::Logger::Info("Sound is Playing");
    //    }

    //    bool bIsPlaying = true;

    //    while (bIsPlaying)
    //    {
    //        pChannel->isPlaying(&bIsPlaying);
    //        pSystem->update();
    //    }

    //    //std::cout << result << std::endl;
    //}

    //void LoadAudio(Popplio::AudioSystem& audio)
    //{

    //    audio.SetAudio("../assets/audio/track.wav", Popplio::SOUND_TRACK, 
    //        audio.GetCGroup(1), 75.f, { 4, 4 });
    //    audio.SetAudio("../assets/audio/clap.wav", Popplio::SOUND_CLAP, audio.GetCGroup(2));

    //    audio.UseCGroup(1)->setVolume(.25f);
    //    audio.UseCGroup(2)->setVolume(1.f);

    //    audio.GetAudio(0).Play();
    //}

    void InitTestAudio(Popplio::AudioSystem& audio)
    {
        // Audio::UseAudio()
        {
            unsigned int len{ 0 };
            audio.GetAudio(0).UseSound()->getLength(&len, Popplio::AudioSystem::defaultUnit);
            Popplio::Logger::Debug("Audio::UseAudio() | Audio[0] Length: " + std::to_string(len));
        }
        // -----------------------
        // Audio::UseChannel()
        {
            float vol{ 0.f };
            audio.GetAudio(0).UseChannel()->getVolume(&vol);
            Popplio::Logger::Debug("Audio::UseChannel() | Audio[0] Volume: " + std::to_string(vol));
        }
        // -----------------------
        // Audio::GetGroup()
        {
            float vol{ 0.f };
            audio.GetAudio(0).GetGroup()->getVolume(&vol);
            Popplio::Logger::Debug("Audio::GetGroup() | Audio[0] Channel Group Volume: " + 
                std::to_string(vol));
        }
    }

    void TestAudio(Popplio::AudioSystem& audio)
    {
        if (audio.GetAudio(0).IsOnBPM())
        {
            Popplio::Logger::Debug("Before Beat Play: " + std::to_string(audio.GetAudio(0).OffBeat()) + "ms");
            audio.GetAudio(1).Play(); // metronome
            Popplio::Logger::Debug("After Beat Play: " + std::to_string(audio.GetAudio(0).OffBeat()) + "ms");
        }
    }

    /*void InputLagTest(Popplio::KeyPressedEvent& event)
    {
        static_cast<void>(event);
        //Popplio::Logger::Debug("Key Pressed: " + std::to_string(asys->audio[0].OffBeat()) + " ms off beat");
    }*/

    void InputLagTestInit(std::unique_ptr<Popplio::Registry>& reg, std::unique_ptr<Popplio::EventBus>& eventbus)
    {
        static_cast<void>(eventbus);
        static_cast<void>(reg);
        //reg->GetSystem<Popplio::InputSystem>().SubscribeToEvent(eventbus, &InputLagTest);
    }

    /*

    // ++++++++++++++++++++++++++++++ Matrices & Vector2D ++++++++++++++++++++++++++++++++++++++++++ //

    //constexpr float EPSILON = 0.0001f;
    //constexpr double EPSILOND = 0.0001;
    //constexpr float PI = 3.14159265358f;

    ////
    //void PrintVector(char* txt, const Vec2& pVec0)
    //{
    //    printf("%s:\t%f, %f\n", txt, pVec0.x, pVec0.y);
    //}

    //void PrintMtx33(char* pString, Mtx33& pMtx)
    //{
    //    printf("%s:\n", pString);
    //    printf("%8.5f %8.5f %8.5f\n", pMtx.m00, pMtx.m01, pMtx.m02);
    //    printf("%8.5f %8.5f %8.5f\n", pMtx.m10, pMtx.m11, pMtx.m12);
    //    printf("%8.5f %8.5f %8.5f\n", pMtx.m20, pMtx.m21, pMtx.m22);
    //}

    //float CompareMtx33(const Mtx33& pSrc, const Mtx33& pDst)
    //{
    //    float d = 0.0f;

    //    for (long i = 0; i < 9; i++)
    //        d += (float)fabs((double)pSrc.m[i] - (double)pDst.m[i]);

    //    return d;
    //}

    //float CompareVec2(const Vec2& pSrc, const Vec2& pDst)
    //{
    //    float d = 0.0f;

    //    d += (float)fabs((double)pSrc.x - (double)pDst.x);
    //    d += (float)fabs((double)pSrc.y - (double)pDst.y);

    //    return d;
    //};

    //void TestMyMath()
    //{
    //    // Testing Vector2D
    //    //--------------------------------------------------------------------------
    //    Vec2 v1, v2, v3, result;

    //    printf("Testing Vector2D:\n");
    //    printf("-----------------------------\n");

    //    Vec2 v4(1.0f, 2.0f);
    //    result.x = 1.0f;	result.y = 2.0f;
    //    printf("Vec2Set: \t\t%s\n",
    //        (CompareVec2(result, v4) < EPSILON) ? "Pass" : "---Fail");

    //    // Vector2D Negate
    //    //--------------------------------------------------------------------------
    //    v1.x = 2.0f; v1.y = -4.0f;
    //    v2 = -v1;
    //    result.x = -2.0f;	result.y = 4.0f;
    //    printf("Vec2Neg: \t\t%s\n",
    //        (CompareVec2(result, v2) < EPSILON) ? "Pass" : "---Fail");

    //    // Vector2D Add
    //    //--------------------------------------------------------------------------
    //    v1.x = 2.0f; v1.y = -4.0f;
    //    v2.x = 1.0f; v2.y = 7.0f;
    //    v3 = v1 + v2;
    //    result.x = result.y = 3.0f;
    //    printf("Vec2Add 1: \t\t%s\n",
    //        (CompareVec2(result, v3) < EPSILON) ? "Pass" : "---Fail");

    //    v1.x = 2.0f; v1.y = -4.0f;
    //    v2.x = 1.0f; v2.y = 7.0f;
    //    v1 += v2;
    //    result.x = result.y = 3.0f;
    //    printf("Vec2Add 2: \t\t%s\n",
    //        (CompareVec2(result, v1) < EPSILON) ? "Pass" : "---Fail");

    //    // Vector2D Subtract
    //    //--------------------------------------------------------------------------
    //    v1.x = 2.0f; v1.y = -4.0f;
    //    v2.x = 1.0f; v2.y = 7.0f;
    //    v3 = v1 - v2;
    //    result.x = 1.0f;	result.y = -11.0f;
    //    printf("Vec2Sub 1: \t\t%s\n",
    //        (CompareVec2(result, v3) < EPSILON) ? "Pass" : "---Fail");

    //    v1.x = 2.0f; v1.y = -4.0f;
    //    v2.x = 1.0f; v2.y = 7.0f;
    //    v1 -= v2;
    //    result.x = 1.0f;	result.y = -11.0f;
    //    printf("Vec2Sub 2: \t\t%s\n",
    //        (CompareVec2(result, v1) < EPSILON) ? "Pass" : "---Fail");

    //    // Vector2D Normalize
    //    //--------------------------------------------------------------------------
    //    v1.x = 3.0f; v1.y = 4.0f;
    //    Vector2DNormalize(v2, v1);
    //    result.x = 0.6f;	result.y = 0.8f;
    //    printf("Vec2Normalize: \t\t%s\n",
    //        (CompareVec2(result, v2) < EPSILON) ? "Pass" : "---Fail");

    //    // Vector2D Scale
    //    //--------------------------------------------------------------------------
    //    v1.x = 2.0f; v1.y = -5.0f;
    //    v2 = v1 * 3.0f;
    //    result.x = 6.0f;	result.y = -15.0f;
    //    printf("Vec2Scale 1: \t\t%s\n",
    //        (CompareVec2(result, v2) < EPSILON) ? "Pass" : "---Fail");

    //    v1.x = 2.0f; v1.y = -5.0f;
    //    v2 = 3.0f * v1;
    //    result.x = 6.0f;	result.y = -15.0f;
    //    printf("Vec2Scale 2: \t\t%s\n",
    //        (CompareVec2(result, v2) < EPSILON) ? "Pass" : "---Fail");

    //    v1.x = 2.0f; v1.y = -5.0f;
    //    v1 *= 3.0f;
    //    result.x = 6.0f;	result.y = -15.0f;
    //    printf("Vec2Scale 3: \t\t%s\n",
    //        (CompareVec2(result, v1) < EPSILON) ? "Pass" : "---Fail");

    //    v1.x = 6.0f;	v1.y = -15.0f;
    //    v2 = v1 / 3.0f;
    //    result.x = 2.0f; result.y = -5.0f;
    //    printf("Vec2Scale 4: \t\t%s\n",
    //        (CompareVec2(result, v2) < EPSILON) ? "Pass" : "---Fail");

    //    v1.x = 6.0f;	v1.y = -15.0f;
    //    v1 /= 3.0f;
    //    result.x = 2.0f; result.y = -5.0f;
    //    printf("Vec2Scale 5: \t\t%s\n",
    //        (CompareVec2(result, v1) < EPSILON) ? "Pass" : "---Fail");

    //    // Vector2D Length
    //    //--------------------------------------------------------------------------
    //    v1.x = 3.0f; v1.y = -4.0f;
    //    printf("Vec2Length: \t\t%s\n",
    //        (fabs((double)Vector2DLength(v1) - 5.0) < EPSILOND) ? "Pass" : "---Fail");

    //    // Vector2D Square Length
    //    //--------------------------------------------------------------------------
    //    v1.x = 3.0f; v1.y = -4.0f;
    //    printf("Vec2SquareLength: \t%s\n",
    //        (fabs((double)Vector2DSquareLength(v1) - 25.0) < EPSILOND) ? "Pass" : "---Fail");

    //    // Vector2D Distance
    //    //--------------------------------------------------------------------------
    //    v1.x = 2.0f;	v1.y = 3.0f;
    //    v2.x = 4.0f;	v2.y = -1.0f;
    //    printf("Vec2Distance: \t\t%s\n",
    //        (fabs((double)Vector2DDistance(v1, v2) - 4.472136) < EPSILOND) ? "Pass" : "---Fail");

    //    // Vector2D Square Distance
    //    //--------------------------------------------------------------------------
    //    v1.x = 2.0f;	v1.y = 3.0f;
    //    v2.x = 4.0f;	v2.y = -1.0f;
    //    printf("Vec2SquareDistance: \t%s\n",
    //        (fabs((double)Vector2DSquareDistance(v1, v2) - 20.0) < EPSILOND) ? "Pass" : "---Fail");

    //    // Vector2D Dot Product
    //    //--------------------------------------------------------------------------
    //    v1.x = 3.0f;	v1.y = 2.0f;
    //    v2.x = 4.0f;	v2.y = -6.0f;
    //    printf("Vec2DotProduct: \t%s\n",
    //        (fabs((double)Vector2DDotProduct(v1, v2)) < EPSILOND) ? "Pass" : "---Fail");

    //    // Vector2D Cross Product Magnitude
    //    //--------------------------------------------------------------------------
    //    v1.x = 3.0f;	v1.y = 2.0f;
    //    v2.x = 4.0f;	v2.y = -6.0f;
    //    printf("Vec2CrossProductMag: \t%s\n",
    //        (fabs((double)Vector2DCrossProductMag(v1, v2) + 26.0f) < EPSILOND) ? "Pass" : "---Fail");



    //    // Testing Matrix3x3
    //    //--------------------------------------------------------------------------
    //    Mtx33 id, m0, m1;
    //    Vec2  u;
    //    //float d;
    //    float x, y, * p;
    //    long  n;

    //    printf("\nTesting Matrix3x3:\n");
    //    printf("-----------------------------\n");

    //    // create an id matrix for reference
    //    memset(id.m, 0, 9 * sizeof(float));
    //    id.m[0] = id.m[4] = id.m[8] = 1.0f;

    //    // Conversion constructors
    //    //--------------------------------------------------------------------------
    //    // generate an array of random numbers
    //    p = new float[9];
    //    for (int i = 0; i < 9; ++i)
    //        p[i] = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    memcpy(m0.m, p, 9 * sizeof(float));
    //    Mtx33 m2(p);
    //    printf("Mtx33Constructor 1: \t%s\n",
    //        (CompareMtx33(m0, m2) < EPSILON) ? "Pass" : "---Fail");

    //    Mtx33 m3(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
    //    printf("Mtx33Constructor 2: \t%s\n",
    //        (CompareMtx33(m0, m3) < EPSILON) ? "Pass" : "---Fail");

    //    delete[]p;

    //    // Identity Matrix
    //    //--------------------------------------------------------------------------
    //    Mtx33Identity(m0);
    //    //d = 
    //    CompareMtx33(id, m0);
    //    printf("Mtx33Identity: \t\t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // Translate Matrix
    //    //--------------------------------------------------------------------------
    //    // generate 2 random numbers
    //    x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //    y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    Mtx33Translate(m0, x, y);
    //    m0.m02 -= x;
    //    m0.m12 -= y;
    //    printf("Mtx33Translate: \t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // Scale Matrix
    //    //--------------------------------------------------------------------------
    //    // generate 2 random numbers
    //    x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //    y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    Mtx33Scale(m0, x, y);
    //    m0.m00 /= x;
    //    m0.m11 /= y;

    //    printf("Mtx33Scale: \t\t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");


    //    // Matrix Multiplication
    //    //--------------------------------------------------------------------------
    //    Mtx33 mat01, mat02, mat_mult;
    //    Mtx33Identity(mat01);
    //    Mtx33Identity(mat02);
    //    Mtx33Identity(mat_mult);
    //    Mtx33Translate(mat01, 7.0f, 17.0f);
    //    Mtx33Translate(mat02, 8.0f, 18.0f);
    //    mat01.m00 = 2.0f;
    //    mat01.m01 = 2.0f;
    //    mat02.m00 = 5.0f;
    //    mat02.m01 = 5.0f;
    //    mat01 *= mat02;

    //    printf("Matrix Multiplication: \t%s\n",
    //        (mat01.m02 - 59.0f < EPSILON) ? "Pass" : "---Fail");


    //    // Concatenate Matrix
    //    //--------------------------------------------------------------------------
    //    // generate 2 random numbers
    //    x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //    y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    Mtx33Translate(m0, x, y);
    //    Mtx33Scale(m1, x, y);
    //    m0 = m0 * m1;
    //    m0.m02 -= x;
    //    m0.m12 -= y;
    //    m0.m00 /= x;
    //    m0.m11 /= y;

    //    printf("Mtx33Concat 1: \t\t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // generate 2 random numbers
    //    x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //    y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    Mtx33Translate(m0, x, y);
    //    Mtx33Scale(m1, x, y);
    //    m0 = m1 * m0;
    //    m0.m02 -= x * x;
    //    m0.m12 -= y * y;
    //    m0.m00 /= x;
    //    m0.m11 /= y;

    //    printf("Mtx33Concat 2: \t\t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // generate 2 random numbers
    //    x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //    y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    Mtx33Translate(m0, x, y);
    //    Mtx33Scale(m1, x, y);
    //    m0 *= m1;
    //    m0.m02 -= x;
    //    m0.m12 -= y;
    //    m0.m00 /= x;
    //    m0.m11 /= y;

    //    printf("Mtx33Concat 3: \t\t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // Rotation Matrix in radians
    //    //--------------------------------------------------------------------------
    //    n = (rand() % 16) + 15;
    //    Mtx33Identity(m0);
    //    Mtx33RotRad(m1, 2.0f * PI / n);

    //    for (long i = 0; i < n; i++)
    //        m0 = m1 * m0;

    //    printf("Mtx33RotRad: \t\t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // Rotation Matrix in degrees
    //    //--------------------------------------------------------------------------
    //    n = (rand() % 16) + 15;
    //    Mtx33Identity(m0);
    //    Mtx33RotDeg(m1, 360.0f / n);

    //    for (long i = 0; i < n; i++)
    //        m0 = m1 * m0;

    //    printf("Mtx33RotDeg: \t\t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // Transpose Matrix
    //    //--------------------------------------------------------------------------

    //    Mtx33RotRad(m0, rand() / (float)(RAND_MAX) * 2.0f * PI);
    //    Mtx33Transpose(m1, m0);
    //    m0 = m1 * m0;

    //    printf("Mtx33Transpose: \t%s\n",
    //        (CompareMtx33(id, m0) < EPSILON) ? "Pass" : "---Fail");

    //    // Vector Multiplication Matrix
    //    //--------------------------------------------------------------------------
    //    // generate 2 random numbers
    //    x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //    y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    n = (rand() % 16) + 15;
    //    u.x = x;
    //    u.y = y;
    //    Mtx33RotRad(m0, 2.0f * PI / n);

    //    for (long i = 0; i < n; i++)
    //        u = m0 * u;

    //    printf("Mtx33MultVec 1: \t%s\n",
    //        ((fabs((double)u.x - (double)x) + fabs((double)u.y - (double)y)) < EPSILOND) ? "Pass" : "---Fail");

    //    // generate 2 random numbers
    //    x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //    y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;

    //    n = (rand() % 16) + 15;
    //    u.x = x;
    //    u.y = y;
    //    Mtx33Translate(m0, x, y);

    //    for (long i = 1; i < n; i++)
    //        u = m0 * u;

    //    printf("Mtx33MultVec 2: \t%s\n",
    //        ((fabs((double)u.x - (double)(x)*n) + fabs((double)u.y - (double)(y)*n)) < EPSILOND) ? "Pass" : "---Fail");

    //    // Inverse Matrix
    //    //--------------------------------------------------------------------------
    //    Mtx33 mInv;
    //    float det = 0.0f;
    //    bool quit = false;
    //    int count = 0, max_count = 21;

    //    // loop until we find an invertible matrix
    //    while (!quit && (count < max_count))
    //    {
    //        x = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //        y = 2.0f * rand() / (float)(RAND_MAX)-1.0f;
    //        Mtx33Translate(m0, x, y);

    //        n = (rand() % 16) + 15;
    //        Mtx33RotRad(m1, 2.0f * PI / n);

    //        Mtx33Scale(m2, x, y);

    //        m3 = m0 * m1 * m2;

    //        Mtx33Inverse(&mInv, &det, m3);

    //        if (fabs((double)det) <= EPSILOND)
    //        {
    //            count++;

    //            if (count == max_count - 1)
    //            {
    //                printf("Mtx33Inverse: \t\t%s\n",
    //                    (CompareMtx33(mInv, m3) < EPSILON) ? "Pass" : "---Fail");
    //            }
    //            continue;
    //        }

    //        Mtx33Inverse(&mInv, &det, mInv);

    //        if (fabs((double)det) <= EPSILOND)
    //        {
    //            count++;

    //            if (count == max_count - 1)
    //            {
    //                printf("Mtx33Inverse: \t\t%s\n",
    //                    (CompareMtx33(mInv, m3) < EPSILON) ? "Pass" : "---Fail");
    //            }
    //            continue;
    //        }

    //        printf("Mtx33Inverse: \t\t%s\n",
    //            (CompareMtx33(mInv, m3) < EPSILON) ? "Pass" : "---Fail");


    //        quit = true;
    //    }
    //}

    // ++++++++++++++++++++++++++++++ Matrices & Vector2D ++++++++++++++++++++++++++++++++++++++++++ //

    // ++++++++++++++++++++++++++++++ Matrices.h ++++++++++++++++++++++++++++++ //

    //// tests for Matrix::value_type == int
    //void test0(); void test1();
    //// tests for Matrix::value_type == double
    //void test2(); void test3();

    //template <typename T>
    //std::ostream& operator<<(std::ostream& s, PopplioMath::Matrix2D<T> const& m);

    //template <typename T>
    //PopplioMath::Matrix2D<T> matrix_generator(size_t rows, size_t cols);

    //template <typename T>
    //PopplioMath::Matrix2D<T> matrix_generator(PopplioMath::Matrix2D<T> const& m);

    //void TestMatrices(int test)
    //{
    //    constexpr int max_tests{ 4 };
    //    void (*pTests[])() = { test0,test1,test2,test3 };

    //    test = test > 0 ? test : -test;
    //    if (test < max_tests) 
    //    {
    //        Popplio::Logger::Info();
    //        std::cout << "------------------------TEST " << test << " START------------------------\n";
    //        pTests[test]();
    //        std::cout << "------------------------TEST " << test << " END------------------------\n";
    //    }
    //    else 
    //    {
    //        for (int i{}; i < max_tests; ++i) {
    //            std::cout << "------------------------TEST " << i << " START------------------------\n";
    //            pTests[i]();
    //            std::cout << "------------------------TEST " << i << " END------------------------\n";
    //        }
    //    }
    //}

    //template <typename T>
    //std::ostream& operator<<(std::ostream& s, PopplioMath::Matrix2D<T> const& m)
    //{
    //    std::ios::fmtflags old_settings;
    //    if (std::is_floating_point_v<T>) 
    //    {
    //        old_settings = std::cout.flags();
    //        std::cout << std::fixed;
    //        std::cout << std::setprecision(2);
    //    }

    //    using size_type = typename HLP3::Matrix<T>::size_type;
    //    for (size_type i = 0; i < m.get_rows(); ++i) 
    //    {
    //        s << (i == 0 ? '{' : ',');
    //        for (size_type j = 0; j < m.get_cols(); ++j) {
    //            s << (j == 0 ? '{' : ',') << m[i][j];
    //        }
    //        s << '}';
    //    }
    //    s << '}';

    //    if (std::is_floating_point_v<T>) 
    //    {
    //        std::cout.flags(old_settings);
    //    }
    //    return s;
    //}

    //template <typename T>
    //Matrix<T> matrix_generator(size_t rows, size_t cols) {
    //    static_assert(std::is_arithmetic<T>::value == true, "T must be arithmetic type\n");
    //    std::default_random_engine defEngine{ 99 };
    //    if (std::is_integral<T>::value) {
    //        std::uniform_int_distribution<int> intDistro(0, 10);

    //        Matrix<T> tmp(rows, cols);
    //        using size_type = typename Matrix<T>::size_type;
    //        for (size_type i{ 0 }; i < rows; ++i) {
    //            for (size_type j{ 0 }; j < cols; ++j) {
    //                tmp[i][j] = intDistro(defEngine);
    //            }
    //        }
    //        return tmp;
    //    }
    //    else if (std::is_floating_point<T>::value) {
    //        std::uniform_real_distribution<double> intDistro(0.0, 10.0);

    //        Matrix<T> tmp(rows, cols);
    //        using size_type = typename Matrix<T>::size_type;
    //        for (size_type i{ 0 }; i < rows; ++i) {
    //            for (size_type j{ 0 }; j < cols; ++j) {
    //                tmp[i][j] = intDistro(defEngine);
    //            }
    //        }
    //        return tmp;
    //    }
    //}

    //template <typename T>
    //Matrix<T> matrix_generator(Matrix<T> const& m) {
    //    return m;
    //}

    //// test 0 for T == int: ctors, dtor, op=, overloaded op[], op==, and op!=
    //void test0() {
    //    using size_type = typename Matrix<int>::size_type;
    //    std::default_random_engine defEngine(99);
    //    std::uniform_int_distribution<int> intDistro(0, 100);

    //    // test: ctor (two parameter) and operator[]
    //    Matrix<int> A(2, 3);
    //    A[0][0] = 1;  A[0][1] = 2;  A[0][2] = 3;
    //    A[1][0] = 4;  A[1][1] = 5;  A[1][2] = 6;
    //    std::cout << "A:\n" << A << "\n";

    //    size_type rows{ 6 }, cols{ 5 };
    //    Matrix<int> B(rows, cols);
    //    for (size_type i{ 0 }; i < rows; ++i) {
    //        for (size_type j{ 0 }; j < cols; ++j) {
    //            B[i][j] = intDistro(defEngine);
    //        }
    //    }
    //    std::cout << "B:\n" << B << "\n";

    //    // test: copy ctor and op==
    //    Matrix<int> const C{ B };
    //    if (B != C) {
    //        std::cout << "problem with copy ctor!!!\n";
    //        return;
    //    }
    //    std::cout << "C:\n" << C << "\n";

    //    // test: move ctor
    //    Matrix<int> const D(std::move(matrix_generator<int>(C)));
    //    if (false == (D == C)) {
    //        std::cout << "problem with move ctor!!!\n";
    //        return;
    //    }
    //    std::cout << "D:\n" << D << "\n";

    //    Matrix<int> E{
    //      {D[0][0],D[0][1],D[0][2],D[0][3],D[0][4]},
    //      {D[1][0],D[1][1],D[1][2],D[1][3],D[1][4]},
    //      {D[2][0],D[2][1],D[2][2],D[2][3],D[2][4]},
    //      {D[3][0],D[3][1],D[3][2],D[3][3],D[3][4]},
    //      {D[4][0],D[4][1],D[4][2],D[4][3],D[4][4]},
    //      {D[5][0],D[5][1],D[5][2],D[5][3],D[5][4]}
    //    };
    //    if (E != D) {
    //        std::cout << "problem with initializer list ctor!!!\n";
    //        return;
    //    }
    //    std::cout << "E:\n" << E << "\n";

    //    // copy op=
    //    E = A;
    //    std::cout << "E:\n" << E << "\n";

    //    // move op=
    //    E = { {6,5},{4,3},{2,1} }; // E changes from 6x5 to 3x2 
    //    std::cout << "E:\n" << E << "\n";

    //    try {
    //        Matrix<int> F = { {1,2,3},{4,5} };
    //        std::cout << F << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }
    //}

    //// test 1 for T == int: ctors, overloaded op[], op==, op!=, op+, op-, overloaded op*
    //void test1() {
    //    using size_type = typename Matrix<int>::size_type;
    //    std::default_random_engine defEngine(2);
    //    std::uniform_int_distribution<int> intDistro(0, 100);
    //    size_type rows{ 7 }, cols{ 6 };
    //    Matrix<int> A(rows, cols);
    //    for (size_type i{ 0 }; i < rows; ++i) {
    //        for (size_type j{ 0 }; j < cols; ++j) {
    //            A[i][j] = intDistro(defEngine);
    //        }
    //    }
    //    std::cout << "A:\n" << A << "\n";

    //    rows = 6; cols = 7;
    //    Matrix<int> B(rows, cols);
    //    for (size_type i{ 0 }; i < rows; ++i) {
    //        for (size_type j{ 0 }; j < cols; ++j) {
    //            B[i][j] = intDistro(defEngine);
    //        }
    //    }
    //    std::cout << "B:\n" << B << "\n";

    //    Matrix<int> C{ std::move(matrix_generator<int>(A.get_rows(), A.get_cols())) };
    //    std::cout << "C:\n" << C << "\n";

    //    // impossible addition, subtraction, multiplication
    //    try {
    //        Matrix<int> Z = A + B;
    //        std::cout << "Z:\n" << Z << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }

    //    try {
    //        Matrix<int> Z = A - B;
    //        std::cout << "Z:\n" << Z << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }

    //    try {
    //        Matrix<int> Z = A * C;
    //        std::cout << "Z:\n" << Z << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }

    //    Matrix<int> D = A * B; // 7x6 and 6x4 == 7x4
    //    std::cout << "D:\n" << D << "\n";
    //    D = A * (2 * B);
    //    std::cout << "D:\n" << D << "\n";

    //    Matrix<int> E = A + C;
    //    std::cout << "E:\n" << E << "\n";
    //    E = A - C;
    //    std::cout << "E:\n" << E << "\n";

    //    Matrix<int> F = D - 2 * D;
    //    std::cout << "F:\n" << F << "\n";
    //}

    //// test 2 for T == double: ctors, dtor, op=, overloaded op[], op==, and op!=
    //void test2() {
    //    using size_type = typename Matrix<double>::size_type;
    //    std::default_random_engine defEngine(99);
    //    std::uniform_real_distribution<double> intDistro(-10.0, 10.0);

    //    // test: ctor (two parameter) and operator[]
    //    Matrix<double> A(2, 3);
    //    A[0][0] = 1;  A[0][1] = 2;  A[0][2] = 3;
    //    A[1][0] = 4;  A[1][1] = 5;  A[1][2] = 6;
    //    std::cout << "A:\n" << A << "\n";

    //    size_type rows{ 6 }, cols{ 5 };
    //    Matrix<double> B(rows, cols);
    //    for (size_type i{ 0 }; i < rows; ++i) {
    //        for (size_type j{ 0 }; j < cols; ++j) {
    //            B[i][j] = intDistro(defEngine);
    //        }
    //    }
    //    std::cout << "B:\n" << B << "\n";

    //    // test: copy ctor and op==
    //    Matrix<double> const C{ B };
    //    if (B != C) {
    //        std::cout << "problem with copy ctor!!!\n";
    //        return;
    //    }
    //    std::cout << "C:\n" << C << "\n";

    //    // test: move ctor
    //    Matrix<double> const D(std::move(matrix_generator<double>(C)));
    //    if (false == (D == C)) {
    //        std::cout << "problem with move ctor!!!\n";
    //        return;
    //    }
    //    std::cout << "D:\n" << D << "\n";

    //    Matrix<double> E{
    //      {D[0][0],D[0][1],D[0][2],D[0][3],D[0][4]},
    //      {D[1][0],D[1][1],D[1][2],D[1][3],D[1][4]},
    //      {D[2][0],D[2][1],D[2][2],D[2][3],D[2][4]},
    //      {D[3][0],D[3][1],D[3][2],D[3][3],D[3][4]},
    //      {D[4][0],D[4][1],D[4][2],D[4][3],D[4][4]},
    //      {D[5][0],D[5][1],D[5][2],D[5][3],D[5][4]}
    //    };
    //    if (E != D) {
    //        std::cout << "problem with initializer list ctor!!!\n";
    //        return;
    //    }
    //    std::cout << "E:\n" << E << "\n";

    //    // copy op=
    //    E = A;
    //    std::cout << "E:\n" << E << "\n";

    //    // move op=
    //    E = { {6.1,5.2},{4.1,3.2},{2.2,1.3} }; // E changes from 6x5 to 3x2 
    //    std::cout << "E:\n" << E << "\n";

    //    try {
    //        Matrix<double> F = { {1.1,2.2,3.3},{4.4,5.5} };
    //        std::cout << F << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }
    //}

    //// test 3 for T == double: ctors, overloaded op[], op==, op!=, op+, op-, overloaded op*
    //void test3() {
    //    using size_type = Matrix<double>::size_type;

    //    std::default_random_engine defEngine(2);
    //    std::uniform_real_distribution<double> intDistro(-10.0, 10.0);
    //    size_type rows{ 7 }, cols{ 6 };
    //    Matrix<double> A(rows, cols);
    //    for (size_type i{ 0 }; i < rows; ++i) {
    //        for (size_type j{ 0 }; j < cols; ++j) {
    //            A[i][j] = intDistro(defEngine);
    //        }
    //    }
    //    std::cout << "A:\n" << A << "\n";

    //    rows = 6; cols = 7;
    //    Matrix<double> B(rows, cols);
    //    for (size_type i{ 0 }; i < rows; ++i) {
    //        for (size_type j{ 0 }; j < cols; ++j) {
    //            B[i][j] = intDistro(defEngine);
    //        }
    //    }
    //    std::cout << "B:\n" << B << "\n";

    //    Matrix<double> C{ std::move(matrix_generator<double>(A.get_rows(), A.get_cols())) };
    //    std::cout << "C:\n" << C << "\n";

    //    // impossible addition, subtraction, multiplication
    //    try {
    //        Matrix<double> Z = A + B;
    //        std::cout << "Z:\n" << Z << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }

    //    try {
    //        Matrix<double> Z = A - B;
    //        std::cout << "Z:\n" << Z << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }

    //    try {
    //        Matrix<double> Z = A * C;
    //        std::cout << "Z:\n" << Z << "\n";
    //    }
    //    catch (std::exception& e) {
    //        std::cout << e.what() << "\n";
    //    }

    //    Matrix<double> D = A * B; // 7x6 and 6x4 == 7x4
    //    std::cout << "D:\n" << D << "\n";
    //    D = A * (0.9 * B);
    //    std::cout << "D:\n" << D << "\n";

    //    Matrix<double> E = A + C;
    //    std::cout << "E:\n" << E << "\n";
    //    E = A - C;
    //    std::cout << "E:\n" << E << "\n";

    //    Matrix<double> F = D - 0.5 * D;
    //    std::cout << "F:\n" << F << "\n";
    //}

    // ++++++++++++++++++++++++++++++ Matrices.h ++++++++++++++++++++++++++++++ //

    // ++++++++++++++++++++++++++++++ Polynomials.h ++++++++++++++++++++++++++++++ //

    //namespace {
    //    void test0();
    //    void test1();
    //    void test2();
    //    void test3();
    //    void test4();
    //    void test5();
    //    void test6();
    //}

    //int main(int argc, char* argv[]) {
    //    void (*pTests[])() = { test0, test1, test2, test3, test4, test5, test6 };

    //    if (argc > 1) {
    //        int test = std::stoi(std::string(argv[1]));
    //        if (test < 7) {
    //            pTests[test]();
    //        }
    //        else {
    //            for (size_t i{}; i < sizeof(pTests) / sizeof(pTests[0]); ++i) {
    //                pTests[i]();
    //            }
    //        }
    //    }
    //}

    //namespace {
    //     test0: default ctor, overloads of op[], and non-member op<<
    //    void test0() {
    //        std::cout << "-------- " << __func__ << " --------\n";

    //        HLP3::Polynomial<int, 3> p3;
    //        std::cout << p3 << std::endl; // all zeroes

    //        p3[0] = 1;
    //        p3[1] = 1;
    //        p3[2] = 1;
    //        p3[3] = 1;
    //        std::cout << p3 << std::endl; // all ones

    //        HLP3::Polynomial<float, 1> p1;
    //        p1[0] = 2.2;
    //        p1[1] = 10.1;
    //        std::cout << p1 << std::endl;
    //    }

    //     test1: default and single argument conversion ctors, op[] overloads,
    //     op* overloads, and non-member op<< 
    //    void test1() {
    //        std::cout << "-------- " << __func__ << " --------\n";

    //        HLP3::Polynomial<int, 3> p3;
    //        p3[0] = 1;
    //        p3[1] = 1;
    //        p3[2] = 1;
    //        p3[3] = 1; // defines 1+x+x^2+x^3

    //        HLP3::Polynomial<int, 1> p1;
    //        p1[0] = 1;
    //        p1[1] = -1; // defines 1-x
    //        std::cout << p1 << std::endl;

    //        HLP3::Polynomial<int, 4> p4 = p3 * p1;
    //        std::cout << p4 << std::endl; // produces 1+ -1 * x^4
    //         (1+x+x^2+x^3)*(1-x) = 1-x^4

    //        HLP3::Polynomial<float, 2> pf_1;
    //        pf_1[0] = 1.1;
    //        pf_1[1] = -2.2;
    //        pf_1[2] = 1.1;
    //        std::cout << pf_1 << std::endl;

    //         expression pf_1 * p3 should NOT compile because multiplication of two
    //         different types of coefficients should NOT compile!!!
    //         see driver-no-comp-1.cpp
    //         run: make gcc1_NC 
    //    }

    //     test2: default and single-argument conversion ctors, op[] overloads,
    //     and non-member op<<
    //    void test2() {   // conversion
    //        std::cout << "-------- " << __func__ << " --------\n";

    //        HLP3::Polynomial<float, 2> pf_1;
    //        pf_1[0] = 1.1;
    //        pf_1[1] = -2.2;
    //        pf_1[2] = 1.1;
    //        std::cout << pf_1 << std::endl;

    //        HLP3::Polynomial<int, 2> p2(pf_1); // convert
    //        std::cout << p2 << std::endl;

    //         Following definition should NOT compile: HLP3::Polynomial<int,3> p2(pf_1);
    //         since conversion to a different degree should NOT compile!!!
    //         run: make gcc2_NC
    //         see driver-no-comp-2.cpp
    //    }

    //     test3: default and implicit copy ctors, op[] overloads,
    //     implicit copy assignment op= overload, and non-member op<<
    //    void test3() {   // copy 
    //        std::cout << "-------- " << __func__ << " --------\n";

    //        HLP3::Polynomial<int, 2> p2;
    //        p2[0] = 3;
    //        p2[1] = 2;
    //        p2[2] = 1;
    //        std::cout << p2 << std::endl;

    //         The compiler will implicitly generate a copy ctor to copy construct an
    //         object of type HLP3::Polynomial<T,N> from another object of type HLP3::Polynomial<T,N>.
    //         This is ok since the class is not dynamically allocating memory on the free store.
    //         The templated conversion ctor will NOT be used since the implicitly generated
    //         member function is a better match.
    //        HLP3::Polynomial<int, 2> p2_copy(p2);
    //        std::cout << p2_copy << std::endl;

    //        p2[0] = 5;
    //        p2[1] = 5;
    //        p2[2] = 5;
    //         using compiler generated assignment
    //        p2_copy = p2;
    //        std::cout << p2_copy << std::endl;
    //    }

    //     test4: default ctor, op[] overloads, op= overload, and non-member op<<
    //    void test4() {   // templated assignment
    //        std::cout << "-------- " << __func__ << " --------\n";

    //        HLP3::Polynomial<float, 2> pf_2;
    //        pf_2[0] = 1.1;
    //        pf_2[1] = -2.2;
    //        pf_2[2] = 1.1;
    //        std::cout << pf_2 << std::endl;

    //        HLP3::Polynomial<int, 2> p2;
    //        p2[0] = 3;
    //        p2[1] = 2;
    //        p2[2] = 1;
    //        std::cout << p2 << std::endl;

    //        p2 = pf_2;
    //        std::cout << p2 << std::endl;
    //    }

    //     test5: default and single argument conversion ctor, op[] overloads, op* overload,
    //     op() to evaluate the polynomial, and non-member op<<
    //    void test5() {   // evaluate
    //        std::cout << "-------- " << __func__ << " --------\n";

    //        HLP3::Polynomial<int, 2> p2;
    //        p2[0] = 1;
    //        p2[1] = -2;
    //        p2[2] = 1;
    //        std::cout << p2 << std::endl;

    //        HLP3::Polynomial<int, 5> p5;
    //        p5[0] = 3;
    //        p5[1] = -2;
    //        p5[2] = 1;
    //        p5[3] = 1;
    //        p5[4] = -1;
    //        p5[5] = 1;
    //        std::cout << p5 << std::endl;

    //        HLP3::Polynomial<int, 7> p7_1 = p2 * p5;
    //        HLP3::Polynomial<int, 7> p7_2 = p5 * p2; // should be commutative

    //        int values[] = { 1,2,3,4,5 };
    //        for (int i = 0; i < 5; ++i) {
    //            int r1 = p2(values[i]);
    //            int r2 = p5(values[i]);
    //            int r3 = p7_1(values[i]);
    //            int r4 = p7_2(values[i]);
    //            std::cout << r3 << " " << r4 << std::endl;
    //            if (r1 * r2 != r3 or r1 * r2 != r4) {
    //                std::cout << "Error\n";
    //            }
    //        }
    //    }

    //     evaluate randomly generated polynomials using C++11 <random> library ...
    // 
    //    void test6() {   // evaluate randomly generated polynomials
    //        std::cout << "-------- " << __func__ << " --------\n";

    //        std::random_device          rd;
    //        std::mt19937                gen(rd());
    //        std::uniform_int_distribution<>  rand_coeff(-10, 10);

    //        HLP3::Polynomial<int, 5> p5;
    //        for (int i = 0; i <= 5; ++i) {
    //            p5[i] = rand_coeff(gen);
    //        }

    //        HLP3::Polynomial<int, 4> p4;
    //        for (int i = 0; i <= 4; ++i) {
    //            p4[i] = rand_coeff(gen);
    //        }

    //        HLP3::Polynomial<int, 9> p9_1 = p4 * p5;
    //        HLP3::Polynomial<int, 9> p9_2 = p5 * p4; // should be commutative

    //        int values[] = { 1, 2, 3, 4, 5 };
    //        for (int i = 0; i < 5; ++i) {
    //            int r1 = p4(values[i]);
    //            int r2 = p5(values[i]);
    //            int r3 = p9_1(values[i]);
    //            int r4 = p9_2(values[i]);
    //            if (r1 * r2 != r3 or r1 * r2 != r4) {
    //                std::cout << "Error\n";
    //            }
    //            else {
    //                std::cout << "OK\n";
    //            }
    //        }
    //    }

    //}

    // ++++++++++++++++++++++++++++++ Polynomials.h ++++++++++++++++++++++++++++++ //

    */

    // Shawn ----------------------------------------------- //

    void TestCollisionSystem()
    {

    }

    void TestPrefabManager(Popplio::PrefabManager& prefabManager, Popplio::Registry& registry)
    {
        Popplio::Logger::Info("Starting PrefabManager test");

        // Create a prefab
        auto playerPrefab = std::make_unique<Popplio::Prefab>("Player");
        playerPrefab->AddComponent<Popplio::TransformComponent>(0.0f, 0.0f, 1.0f, 1.0f, 0.0);
        playerPrefab->AddComponent<Popplio::RigidBodyComponent>(1.0f, 1.0f);

        // Register the prefab
        prefabManager.RegisterPrefab("Player", std::move(playerPrefab));

        // Test getting the prefab
        auto retrievedPrefab = prefabManager.GetPrefab("Player");
        if (retrievedPrefab) {
            Popplio::Logger::Info("Successfully retrieved Player prefab");
        }
        else {
            Popplio::Logger::Error("Failed to retrieve Player prefab");
            return;
        }

        // Test instantiating the prefab
        auto playerEntity = prefabManager.InstantiatePrefab(registry, "Player");
        if (playerEntity) {
            Popplio::Logger::Info("Successfully instantiated Player entity");

            // Verify components
            auto& transform = playerEntity->GetComponent<Popplio::TransformComponent>();
            auto& rb = playerEntity->GetComponent<Popplio::RigidBodyComponent>();

            //if (transform.pos_x == 0.0f && transform.pos_y == 0.0f &&
                //velocity.dx == 1.0f && velocity.dy == 1.0f) {
              //  velocity.velocity.GetX() == 1.0f && velocity.velocity.GetY() == 1.0f) {
            if (transform.position.X() == 0.0f && transform.position.Y() == 0.0f &&
                rb.velocity.X() == 1.0f && rb.velocity.Y() == 1.0f) {
                Popplio::Logger::Info("Player entity has correct component values");
            }
            else {
                Popplio::Logger::Error("Player entity has incorrect component values");
            }
        }
        else {
            Popplio::Logger::Error("Failed to instantiate Player entity");
        }

        // Test non-existent prefab
        auto nonExistentPrefab = prefabManager.GetPrefab("NonExistent");
        if (!nonExistentPrefab) {
            Popplio::Logger::Info("Correctly handled non-existent prefab");
        }
        else {
            Popplio::Logger::Error("Incorrectly retrieved non-existent prefab");
        }

        Popplio::Logger::Info("PrefabManager test completed");
    }

    void TestSerialization(Popplio::Registry& registry, Popplio::Serializer& serializer)
	{
        Popplio::Logger::Info("Starting Serialization Test");

        // Create test entities
        auto entity1 = registry.CreateEntity();
        entity1.AddComponent<Popplio::TransformComponent>(0.0f, 0.0f, 1.0f, 1.0f, 0.0);
        entity1.AddComponent<Popplio::RigidBodyComponent>(1.0f, 1.0f);
        // entity1.AddComponent<Popplio::RenderComponent>(Popplio::MeshType::Quad, 0); //todo: fix
        entity1.Tag("Player");

        auto entity2 = registry.CreateEntity();
        entity2.AddComponent<Popplio::TransformComponent>(5.0f, 5.0f, 2.0f, 2.0f, 45.0);
        //entity2.AddComponent<Popplio::DebugRenderComponent>(Popplio::debugMeshType::debugCircle); //todo: find debug render
        entity2.Group("Enemies");

        auto entity3 = registry.CreateEntity();
        entity3.AddComponent<Popplio::TransformComponent>(-5.0f, -5.0f, 1.0f, 1.0f, 0.0);
        entity3.AddComponent<Popplio::TextComponent>("../assets/font/StickyGlue.ttf", "Hello, World!", glm::vec2(100.0f, 100.0f));

        // Attempt to save entities to file
        const std::string filename = "test_serialization.json";
        bool saveSuccess = serializer.SaveSceneToFile(filename);

        if (saveSuccess)
        {
            Popplio::Logger::Info("Successfully saved entities to " + filename);
        }
        else
        {
            Popplio::Logger::Error("Failed to save entities to " + filename);
        }

        // Optional: Verify the contents of the file
        std::ifstream file(filename);
        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                std::cout << line << std::endl;
            }
            file.close();
        }
        else
        {
            Popplio::Logger::Error("Unable to open file for verification: " + filename);
        }

        Popplio::Logger::Info("Serialization Test Completed");
	}
}