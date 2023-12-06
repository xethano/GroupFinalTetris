// GroupFinalTetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//needs lib folder

#include <iostream>
#include "common.h"

#if _DEBUG
    // debug mode requires loading debug libraries from SMFL
    // #pragma comment lib only works on windows compilers!
    #pragma comment( lib, "sfml-main-s-d.lib")
    #pragma comment( lib, "sfml-graphics-s-d.lib")
    #pragma comment( lib, "sfml-system-s-d.lib")
    #pragma comment( lib, "sfml-window-s-d.lib")
    #pragma comment( lib, "sfml-audio-s-d.lib")
#else
    #pragma comment( lib, "sfml-main-s.lib")
    #pragma comment( lib, "sfml-graphics-s.lib")
    #pragma comment( lib, "sfml-system-s.lib")
    #pragma comment( lib, "sfml-window-s.lib")
    #pragma comment( lib, "sfml-audio-s.lib")
#endif

#pragma comment( lib, "vorbis.lib")
#pragma comment( lib, "ogg.lib")
#pragma comment( lib, "flac.lib")
#pragma comment( lib, "openal32.lib")
#pragma comment( lib, "vorbisfile.lib")
#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")
#pragma comment( lib, "freetype.lib")

#include "MainEngine.h"

int main()
{
    srand(time(NULL));


   


    MainEngine game;
    game.MainLoop();

    return 0;
}

