/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens.hpp"    // NOTE: Declares global (extern) variables and screens functions

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// Update and draw game frame
static void UpdateDrawFrame(void)
{

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            case ENDING: DrawEndingScreen(); break;
            default: break;
        }

        // Draw full screen rectangle in front of everything
        if (onTransition) DrawTransition();

        //DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
