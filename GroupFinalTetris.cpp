// GroupFinalTetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "common.h"

#pragma comment( lib, "sfml-main-s-d.lib")
#pragma comment( lib, "sfml-graphics-s-d.lib")
#pragma comment( lib, "sfml-system-s-d.lib")
#pragma comment( lib, "sfml-window-s-d.lib")
#pragma comment( lib, "sfml-audio-s-d.lib")
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
    srand(NULL);
    MainEngine game;
    game.MainLoop();

    return 0;
}

