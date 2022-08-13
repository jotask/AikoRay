#include "logo_state.hpp"

#include "title_state.hpp"
#include "utils/raylib_include.hpp"

namespace aiko
{

    void LogoState::init()
    {
        finishScreen = 0;
        framesCounter = 0;
        lettersCount = 0;

        logoPositionX = raylib::GetScreenWidth() / 2 - 128;
        logoPositionY = raylib::GetScreenHeight() / 2 - 128;

        topSideRecWidth = 16;
        leftSideRecHeight = 16;
        bottomSideRecWidth = 16;
        rightSideRecHeight = 16;

        state = 0;
        alpha = 1.0f;
    }

    void LogoState::dispose()
    {

    }

    void LogoState::update()
    {
        if (state == 0)                 // State 0: Top-left square corner blink logic
        {
            framesCounter++;

            if (framesCounter == 80)
            {
                state = 1;
                framesCounter = 0;      // Reset counter... will be used later...
            }
        }
        else if (state == 1)            // State 1: Bars animation logic: top and left
        {
            topSideRecWidth += 8;
            leftSideRecHeight += 8;

            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2)            // State 2: Bars animation logic: bottom and right
        {
            bottomSideRecWidth += 8;
            rightSideRecHeight += 8;

            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3)            // State 3: "raylib" text-write animation logic
        {
            framesCounter++;

            if (lettersCount < 10)
            {
                if (framesCounter / 12)   // Every 12 frames, one more letter!
                {
                    lettersCount++;
                    framesCounter = 0;
                }
            }
            else    // When all letters have appeared, just fade out everything
            {
                if (framesCounter > 200)
                {
                    alpha -= 0.02f;

                    if (alpha <= 0.0f)
                    {
                        alpha = 0.0f;
                        finishScreen = 1;   // Jump to next screen
                    }
                }
            }
        }
    }

    void LogoState::render()
    {
        if (state == 0)         // Draw blinking top-left square corner
        {
            if ((framesCounter / 10) % 2) raylib::DrawRectangle(logoPositionX, logoPositionY, 16, 16, raylib::BLACK);
        }
        else if (state == 1)    // Draw bars animation: top and left
        {
            raylib::DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, raylib::BLACK);
            raylib::DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, raylib::BLACK);
        }
        else if (state == 2)    // Draw bars animation: bottom and right
        {
            raylib::DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, raylib::BLACK);
            raylib::DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, raylib::BLACK);

            raylib::DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, raylib::BLACK);
            raylib::DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, raylib::BLACK);
        }
        else if (state == 3)    // Draw "raylib" text-write animation + "powered by"
        {
            raylib::DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(raylib::BLACK, alpha));
            raylib::DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(raylib::BLACK, alpha));

            raylib::DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(raylib::BLACK, alpha));
            raylib::DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(raylib::BLACK, alpha));

            raylib::DrawRectangle(raylib::GetScreenWidth() / 2 - 112, raylib::GetScreenHeight() / 2 - 112, 224, 224, Fade(raylib::RAYWHITE, alpha));

            raylib::DrawText(raylib::TextSubtext("raylib", 0, lettersCount), raylib::GetScreenWidth() / 2 - 44, raylib::GetScreenHeight() / 2 + 48, 50, Fade(raylib::BLACK, alpha));

            if (framesCounter > 20) raylib::DrawText("powered by", logoPositionX, logoPositionY - 27, 20, Fade(raylib::DARKGRAY, alpha));
        }
    }

    State* LogoState::getNextState()
    {
        return new TitleState();
    }

}