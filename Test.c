/*******************************************************************************************
 *
 *   raylib [textures] example - Texture loading and drawing
 *
 *   Example originally created with raylib 1.0, last time updated with raylib 1.0
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - texture loading and drawing");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
    Texture2D texture = LoadTexture("resources/ap_logo.png"); // Texture loading
    //---------------------------------------------------------------------------------------
    float rotation = 0;
    int frameWidth = texture.width;
    int frameHeight = texture.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = {0.0f, 0.0f, (float)frameWidth, (float)frameHeight};

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = {screenWidth / 2.0f, screenHeight / 2.0f, frameWidth * 2.0f, frameHeight * 2.0f};

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin = {(float)frameWidth, (float)frameHeight};
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexturePro(texture, sourceRec, destRec, origin, (float)rotation++, WHITE);

        DrawText("this IS a texture!", 360, 370, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(texture); // Texture unloading

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}