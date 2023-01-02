float getGravity()
{
    return 100 * GetFrameTime() * 0.98;
}

void moveBall()
{
    if (IsKeyDown(MOVE_UP_KEY))
    {
        newPosOrigin.y -= 20 * GetFrameTime();
    }

    if (IsKeyDown(MOVE_DOWN_KEY))
    {
        newPosOrigin.y += 20 * GetFrameTime();
    }

    if (IsKeyDown(MOVE_RIGHT_KEY))
    {
        newPosOrigin.x += 20 * GetFrameTime();
    }

    if (IsKeyDown(MOVE_LEFT_KEY))
    {
        newPosOrigin.x -= 20 * GetFrameTime();
    }

    newPosOrigin.x += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) * (25 * GetFrameTime());
    newPosOrigin.y += GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) * (25 * GetFrameTime());
    newPosOrigin.x += GetGamepadAxisMovement(1, GAMEPAD_AXIS_LEFT_X) * (25 * GetFrameTime());
    newPosOrigin.y += GetGamepadAxisMovement(1, GAMEPAD_AXIS_LEFT_Y) * (25 * GetFrameTime());
}

void resetBall()
{
    PlayerOrigin = (Vector2){ballPositionForLevel.x, ballPositionForLevel.z};
    newPosOrigin = (Vector2){ballPositionForLevel.x, ballPositionForLevel.z};
    ballPosition = ballPositionForLevel;
}

void loadLevel(int currentLevel)
{
    for (int i = 0; i < RectCount; i++)
    {
        for (int j = 0; j < RectCount; j++)
        {
            rectanglesOfLevel[i][j] = (Rectangle){0, 0, 0, 0};
        }
    }

    for (int i = 0; i < RectCount; i++)
    {
        for (int j = 0; j < RectCount; j++)
        {
            if (levels[currentLevel][i][j])
            {
                rectanglesOfLevel[i][j] = (Rectangle){i * 4, j * 4, 4, 4};
            }
        }
    }
    SeekMusicStream(levelMusic[randomMusic], 0.0);
    SetRandomSeed(time(NULL));
    randomMusic = GetRandomValue(0, MAX_LEVEL_MUSIC_NUM - 1);
    PlayerOrigin = (Vector2){ballPositionForLevel.x, ballPositionForLevel.z};
    newPosOrigin = (Vector2){ballPositionForLevel.x, ballPositionForLevel.z};
    ballPosition = ballPositionForLevel;
}

void PointNearestRectanglePoint(Rectangle rect, Vector2 point, Vector2 *nearest, Vector2 *normal)
{
    // get the closest point on the vertical sides
    float hValue = rect.x;
    float hNormal = -1;
    if (point.x > rect.x + rect.width)
    {
        hValue = rect.x + rect.width;
        hNormal = 1;
    }

    Vector2 vecToPoint = Vector2Subtract((Vector2){hValue, rect.y}, point);
    // get the dot product between the ray and the vector to the point
    float dotForPoint = Vector2DotProduct((Vector2){0, -1}, vecToPoint);
    Vector2 nearestPoint = {hValue, 0};

    if (dotForPoint < 0)
        nearestPoint.y = rect.y;
    else if (dotForPoint >= rect.height)
        nearestPoint.y = rect.y + rect.height;
    else
        nearestPoint.y = rect.y + dotForPoint;

    // get the closest point on the horizontal sides
    float vValue = rect.y;
    float vNormal = -1;
    if (point.y > rect.y + rect.height)
    {
        vValue = rect.y + rect.height;
        vNormal = 1;
    }

    vecToPoint = Vector2Subtract((Vector2){rect.x, vValue}, point);
    // get the dot product between the ray and the vector to the point
    dotForPoint = Vector2DotProduct((Vector2){-1, 0}, vecToPoint);
    *nearest = (Vector2){0, vValue};

    if (dotForPoint < 0)
        nearest->x = rect.x;
    else if (dotForPoint >= rect.width)
        nearest->x = rect.x + rect.width;
    else
        nearest->x = rect.x + dotForPoint;

    if (Vector2LengthSqr(Vector2Subtract(point, nearestPoint)) < Vector2LengthSqr(Vector2Subtract(point, *nearest)))
    {
        *nearest = nearestPoint;

        if (normal)
        {
            normal->x = hNormal;
            normal->y = 0;
        }
    }
    else
    {
        if (normal)
        {
            normal->y = vNormal;
            normal->x = 0;
        }
    }
}

void drawBlinkingCursor(float x, float y)
{
    elapsedTime += GetFrameTime();
    if (elapsedTime > blinkInterval)
    {
        elapsedTime = 0.0f;
        cursorVisible = !cursorVisible;
    }
    if (cursorVisible)
    {
        DrawTextEx(consolasFont, "_", (Vector2){x, y}, 50, 0.5, TERMINALTEXTGOLD);
        if (layer == CREDITS)
        {
            if (GetFPS() == 30)
            {
                creditsHeight -= 2;
            }
            else
            {
                creditsHeight--;
            }
        }
    }
}

void drawConsoleOverlay()
{
    DrawRectangleLinesEx((Rectangle){30, 20, screenWidth - 60, screenHeight - 40}, 5.0f, TERMINALOUTLINEYELLOW);
    DrawRectangleLinesEx((Rectangle){0, 0, screenWidth, screenHeight}, 20.0f, TERMINALBROWN);
    DrawRectangleRec((Rectangle){0, 0, 30, screenHeight}, TERMINALBROWN);
    DrawRectangleRec((Rectangle){screenWidth - 30, 0, 30, screenHeight}, TERMINALBROWN);
    DrawRectangleRec((Rectangle){screenWidth - 210, 12, 160, 160}, TERMINALBROWN);
    DrawRectangleLinesEx((Rectangle){screenWidth - 210, 12, 160, 160}, 5.0f, TERMINALOUTLINEYELLOW);
    DrawRectangleRec((Rectangle){screenWidth - 300, 12, 80, 160}, TERMINALBROWN);
    DrawRectangleLinesEx((Rectangle){screenWidth - 300, 12, 80, 160}, 5.0f, TERMINALOUTLINEYELLOW);
    if (shouldDrawFPS)
    {
        DrawRectangleRec((Rectangle){screenWidth - 580, 12, 100, 45}, TERMINALBROWN);
        DrawRectangleLinesEx((Rectangle){screenWidth - 580, 12, 100, 45}, 5.0f, TERMINALOUTLINEYELLOW);
        DrawTextEx(consolasFont, TextFormat("%d", GetFPS()), (Vector2){screenWidth - 560, 25}, 25, 0.5, TERMINALTEXTGOLD);
    }
    if (IsKeyDown(HELP_KEY) && layer == LEVEL)
    {
        DrawRectangleRec((Rectangle){screenWidth - 910, 12, 320, 160}, TERMINALBROWN);
        DrawRectangleLinesEx((Rectangle){screenWidth - 910, 12, 320, 160}, 5.0f, TERMINALOUTLINEYELLOW);
        // Ignore Warning
        DrawTextEx(consolasFont, TextFormat("Reset Key: %c\nShow Map: %c\nPause Game: %s\nBall Movement: %s", RESET_KEY, SHOW_MAP_KEY, PAUSE_KEY == KEY_ESCAPE ? "Esc" : TextFormat("%c", PAUSE_KEY), TextFormat("%c%c%c%c", MOVE_UP_KEY, MOVE_LEFT_KEY, MOVE_DOWN_KEY, MOVE_RIGHT_KEY)), (Vector2){screenWidth - 890, 25}, 25, 0.5, TERMINALTEXTGOLD);
    }
    apertureLogoRotaion += GetFrameTime() * 100; // Ignore Warning
    if (apertureLogoRotaion > 360)
    {
        apertureLogoRotaion = 0;
    }
    DrawTextEx(consolasFont, TextFormat("%d.\n%d.\n%d.\n0%d", GetRandomValue(100, 200), GetRandomValue(120, 170), GetRandomValue(10, 50), GetRandomValue(0, 9)), (Vector2){screenWidth - 290, 25}, 25, 0.5, TERMINALTEXTGOLD);

    DrawTexturePro(apertureLogo, apertureScienceLogoRectangle, (Rectangle){screenWidth - 130, 92, apertureScienceLogoOrigin.x * 2, apertureScienceLogoOrigin.y * 2}, apertureScienceLogoOrigin, apertureLogoRotaion, WHITE);

    if (shouldDrawCRTEffect)
    {
        UpdateMusicStream(CRTBuzzMusic);
        if (crtEffectRectangleHeight > screenHeight)
        {
            crtEffectRectangleHeight = -screenHeight;
        }
        crtEffectRectangleHeight += GetFrameTime() * 1000;
        DrawRectangleGradientV(0, crtEffectRectangleHeight, screenWidth, screenHeight / 2, (Color){TERMINALTEXTGOLD_CRT_ALPHA.r, TERMINALTEXTGOLD_CRT_ALPHA.g, TERMINALTEXTGOLD_CRT_ALPHA.b, 0}, TERMINALTEXTGOLD_CRT_ALPHA);

        BeginShaderMode(scanlineShader);
        // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
        DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2){0, 0}, WHITE);
        EndShaderMode();
    }
}

void splashScreen()
{
    while (GetTime() < 15)
    {
        PlayMusicStream(mainMenuMusic);
        UpdateMusicStream(mainMenuMusic);
        BeginDrawing();
        ClearBackground(TERMINALBROWN);
        apertureLogoRotaion += GetFrameTime() * 100; // To make logo go faster
        if (apertureLogoRotaion > 360)
        {
            apertureLogoRotaion = 0;
        }

        dotLimit++;
        if (dotLimit > 60)
        {
            strcat(loadingDots, ".");
            dotLimit = 0;
        }

        // Enable if you want aperture logo
        // DrawTexturePro(apertureLogo, apertureScienceLogoRectangle, (Rectangle){screenWidth / 2.0f + apertureLogoGoToCorner.x, screenHeight / 2.0f - apertureLogoGoToCorner.y, apertureScienceLogoOrigin.x * 4, apertureScienceLogoOrigin.y * 4}, (Vector2){apertureScienceLogoOrigin.x * 2, apertureScienceLogoOrigin.y * 2}, apertureLogoRotaion, WHITE);
        DrawTextEx(consolasFont, TextFormat("Loading%s", loadingDots), (Vector2){45, screenHeight - 175}, 35, 0.5, TERMINALTEXTGOLD);
        DrawTextEx(consolasFont, "Hold F1 to show help menu while solving mazes", (Vector2){45, screenHeight - 140}, 35, 0.5, TERMINALTEXTGOLD);
        drawBlinkingCursor(925, screenHeight - 145);
        drawConsoleOverlay();
        EndDrawing();
    }
}

void drawMap()
{
    if (shouldDrawMap || IsKeyDown(SHOW_MAP_KEY))
    {
        DrawRectangleRec((Rectangle){screenWidth - 470, 12, 160, 160}, TERMINALBROWN);

        for (int i = 0; i < RectCount; i++)
        {
            for (int j = 0; j < 40; j++)
            {
                if (levels[currentLevel][i][j])
                {
                    DrawRectangleRec((Rectangle){rectanglesOfLevel[i][j].x + screenWidth - 470, rectanglesOfLevel[i][j].y + 12, rectanglesOfLevel[i][j].width, rectanglesOfLevel[i][j].height}, TERMINALOUTLINEYELLOW);
                }
            }
        }
        DrawRectangleRec((Rectangle){CoordinateOftartAndEndRectangle[0] + screenWidth - 470, CoordinateOftartAndEndRectangle[1] + 12, rectanglesOfLevel[0][0].width, rectanglesOfLevel[0][0].height}, RED);
        DrawRectangleRec((Rectangle){CoordinateOftartAndEndRectangle[2] + screenWidth - 470, CoordinateOftartAndEndRectangle[3] + 12, rectanglesOfLevel[0][0].width, rectanglesOfLevel[0][0].height}, GREEN);
        DrawCircle(PlayerOrigin.x + screenWidth - 470, PlayerOrigin.y + 12, Radius * 3, TERMINALTEXTGOLD);
    }
}

void mainMenu()
{
    PlayMusicStream(mainMenuMusic);
    UpdateMusicStream(mainMenuMusic);
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, titleTextASCII, (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, TextFormat("%s", currentLevel == 0 ? "Play" : "Continue"), (Vector2){45, screenHeight - 350}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), currentLevel == 0 ? (Rectangle){45, screenHeight - 350, 120, 50} : (Rectangle){45, screenHeight - 350, 210, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Credits", (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 200, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 215, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Exit", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(160, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), currentLevel == 0 ? (Rectangle){45, screenHeight - 350, 120, 50} : (Rectangle){45, screenHeight - 350, 210, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            HideCursor();
            SeekMusicStream(mainMenuMusic, 0.0);
            PlaySound(buttonPressSound);
            layer = LEVEL;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 200, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            creditsHeight = screenHeight - 140;
            SeekMusicStream(mainMenuMusic, 0.0);
            PlaySound(buttonPressSound);
            layer = CREDITS;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 215, 210, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            previousLayer = layer;
            PlaySound(buttonPressSound);
            layer = SETTINGS;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            shouldExit = true;
    }

    drawConsoleOverlay();
    EndDrawing();
}

void settingsMenu()
{
    if (previousLayer == MAIN_MENU)
    {
        PlayMusicStream(mainMenuMusic);
        UpdateMusicStream(mainMenuMusic);
    }
    else if (previousLayer == PAUSED)
    {
        PlayMusicStream(levelMusic[randomMusic]);
        UpdateMusicStream(levelMusic[randomMusic]);
    }
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Window Mode: ", (Vector2){45, screenHeight - 490}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("             %s", IsWindowFullscreen() ? "<FULLSCREEN>" : "<WINDOWED>"), (Vector2){45, screenHeight - 490}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), IsWindowFullscreen() ? (Rectangle){400, screenHeight - 490, 330, 50} : (Rectangle){400, screenHeight - 490, 350, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    /// @brief ///////////
    DrawTextEx(consolasFont, "CRT Effect: ", (Vector2){45, screenHeight - 420}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("            %s", shouldDrawCRTEffect ? "<ON>" : "<OFF>"), (Vector2){45, screenHeight - 420}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), shouldDrawCRTEffect ? (Rectangle){375, screenHeight - 420, 115, 50} : (Rectangle){375, screenHeight - 420, 135, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    /// @brief //////////////
    DrawTextEx(consolasFont, "Draw Map: ", (Vector2){45, screenHeight - 350}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("          %s", shouldDrawMap ? "<ON>" : "<OFF>"), (Vector2){45, screenHeight - 350}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), shouldDrawMap ? (Rectangle){315, screenHeight - 350, 115, 50} : (Rectangle){315, screenHeight - 350, 135, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Target FPS: ", (Vector2){45, screenHeight - 280}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("            <%d>", FPS), (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), FPS < 100 ? (Rectangle){380, screenHeight - 280, 100, 50} : (Rectangle){380, screenHeight - 280, 130, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "FPS Counter:", (Vector2){45, screenHeight - 210}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("             %s", shouldDrawFPS ? "<ON>" : "<OFF>"), (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), shouldDrawFPS ? (Rectangle){400, screenHeight - 210, 115, 50} : (Rectangle){400, screenHeight - 210, 135, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Back", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(160, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), IsWindowFullscreen() ? (Rectangle){400, screenHeight - 490, 330, 50} : (Rectangle){400, screenHeight - 490, 280, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);
            ToggleFullscreen();
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        }
    }
    ////////////////////////////
    if (CheckCollisionPointRec(GetMousePosition(), shouldDrawCRTEffect ? (Rectangle){375, screenHeight - 420, 115, 50} : (Rectangle){375, screenHeight - 420, 135, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(CRTOnOffSound);
            SeekMusicStream(CRTBuzzMusic, 0.0);
            shouldDrawCRTEffect = shouldDrawCRTEffect ? false : true;
        }
    }
    /////////////

    if (CheckCollisionPointRec(GetMousePosition(), shouldDrawMap ? (Rectangle){315, screenHeight - 350, 115, 50} : (Rectangle){315, screenHeight - 350, 135, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);
            shouldDrawMap = shouldDrawMap ? false : true;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), FPS < 100 ? (Rectangle){380, screenHeight - 280, 100, 50} : (Rectangle){380, screenHeight - 280, 130, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);

            FPS = FPS + 30;
            if (FPS > 240)
                FPS = 60;

            SetTargetFPS(FPS);
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), shouldDrawFPS ? (Rectangle){400, screenHeight - 210, 115, 50} : (Rectangle){400, screenHeight - 210, 135, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);
            shouldDrawFPS = shouldDrawFPS ? false : true;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);
            layer = previousLayer;
        }
    }
    drawConsoleOverlay();
    EndDrawing();
}

void nextLevel()
{
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, "PASSED", (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Continue", (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 210, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 230, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Main Menu", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(310, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 210, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            HideCursor();
            PlaySound(buttonPressSound);
            layer = LEVEL;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 230, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            previousLayer = layer;
            PlaySound(buttonPressSound);
            layer = SETTINGS;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);
            layer = MAIN_MENU;
        }
    }

    drawConsoleOverlay();
    EndDrawing();
}

void winningScreen()
{
    PlayMusicStream(creditsMusic);
    UpdateMusicStream(creditsMusic);
    if (GetMusicTimePlayed(creditsMusic) > 10)
    {
        creditsHeight = screenHeight - 140;
        layer = CREDITS;
    }
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, "YOU PASSED THE \nTEST", (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Credits", (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 190, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 230, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Main Menu", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(310, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 190, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            creditsHeight = screenHeight - 140;
            PlaySound(buttonPressSound);
            layer = CREDITS;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 230, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            previousLayer = layer;
            PlaySound(buttonPressSound);
            layer = SETTINGS;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);
            SeekMusicStream(creditsMusic, 0.0);
            layer = MAIN_MENU;
        }
    }

    drawConsoleOverlay();
    EndDrawing();
}

void level()
{
    PlayMusicStream(levelMusic[randomMusic]);
    UpdateMusicStream(levelMusic[randomMusic]);
    ballRotation = (Vector2){0, 0};
    if (IsKeyPressed(PAUSE_KEY))
    {
        ShowCursor();
        PlaySound(buzzerSound);
        layer = PAUSED;
    }

    if (IsKeyPressed(RESET_KEY))
    {
        PlayerOrigin = (Vector2){ballPositionForLevel.x, ballPositionForLevel.z};
        newPosOrigin = (Vector2){ballPositionForLevel.x, ballPositionForLevel.z};
        ballPosition = ballPositionForLevel;
    }

    ballPosition.x = PlayerOrigin.x;
    ballPosition.z = PlayerOrigin.y;
    worldCamera.target = ballPosition;
    worldCamera.up.x = 0.0;
    worldCamera.position = (Vector3){ballPosition.x, ballPosition.y + 40, ballPosition.z + 10};
    newPosOrigin = PlayerOrigin;

    moveBall();

    for (int i = 0; i < RectCount; i++)
    {
        for (int j = 0; j < RectCount; j++)
        {
            Vector2 hitPoint = {-100, -100};
            Vector2 hitNormal = {0, 0};
            PointNearestRectanglePoint(rectanglesOfLevel[i][j], newPosOrigin, &hitPoint, &hitNormal);

            Vector2 vectorToHit = Vector2Subtract(hitPoint, newPosOrigin);

            bool inside = Vector2LengthSqr(vectorToHit) < Radius * Radius;

            if (inside)
            {

                // normalize the vector along the point to where we are nearest
                vectorToHit = Vector2Normalize(vectorToHit);

                // project that out to the radius to find the point that should be 'deepest' into the rectangle.
                Vector2 projectedPoint = Vector2Add(newPosOrigin, Vector2Scale(vectorToHit, Radius));

                // compute the shift to take the deepest point out to the edge of our nearest hit, based on the vector direction
                Vector2 delta = {0, 0};

                if (hitNormal.x != 0)
                    delta.x = hitPoint.x - projectedPoint.x;
                else
                    delta.y = hitPoint.y - projectedPoint.y;

                // shift the new point by the delta to push us outside of the rectangle
                newPosOrigin = Vector2Add(newPosOrigin, delta);
            }
        }
    }

    if (!CheckCollisionBoxSphere(floorBoundingBox, ballPosition, Radius))
    {
        ballPosition.y -= getGravity();
    }

    if (CheckCollisionBoxSphere(endGateBoundingBox, ballPosition, Radius))
    {
        ShowCursor();
        currentLevel++;
        if (currentLevel < LEVEL_COUNT)
        {
            loadLevel(currentLevel);
            PlaySound(winSound);
            layer = PLAY_NEXT_LEVEL;
        }
        else
        {
            currentLevel = 0;
            loadLevel(currentLevel);
            PlaySound(winSound);
            layer = WON_SCREEN;
        }
    }

    PlayerOrigin = newPosOrigin;
    UpdateCamera(&worldCamera);
    BeginMode3D(worldCamera);
    ClearBackground(TERMINALBROWN);
    DrawModelEx(
        floorModel, (Vector3){80.0, 0.0, 80.0}, (Vector3){1.0, 0.0, 0.0}, 90, (Vector3){4.0, 4.0, 0.0}, WHITE);
    DrawModel(ballModel, (Vector3){ballPosition.x, ballPosition.y + 1, ballPosition.z - 1}, 2.0, WHITE);
    for (int i = 0; i < RectCount; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            if (levels[currentLevel][i][j])
            {
                DrawModel(wallCube, (Vector3){rectanglesOfLevel[i][j].x + rectanglesOfLevel[i][j].width / 2, 2, rectanglesOfLevel[i][j].y}, 4.0, WHITE);
            }
        }
    }
    DrawModel(targetCube, (Vector3){154, 2, 152}, 4.0, WHITE);
    EndMode3D();

    BeginDrawing();
    drawConsoleOverlay();
    drawMap();
    EndDrawing();
}

void paused()
{
    PlayMusicStream(levelMusic[randomMusic]);
    UpdateMusicStream(levelMusic[randomMusic]);
    if (IsKeyPressed(PAUSE_KEY))
    {
        HideCursor();
        PlaySound(buzzerSound);
        layer = LEVEL;
    }
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, "Paused", (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Resume", (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 170, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 230, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Main Menu", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(310, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 170, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            HideCursor();
            PlaySound(buttonPressSound);
            layer = LEVEL;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 230, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            previousLayer = layer;
            PlaySound(buttonPressSound);
            layer = SETTINGS;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(buttonPressSound);
            SeekMusicStream(levelMusic[randomMusic], 0.0);
            layer = MAIN_MENU;
        }
    }

    drawConsoleOverlay();
    EndDrawing();
}

void creditScreen()
{
    PlayMusicStream(creditsMusic);
    UpdateMusicStream(creditsMusic);
    BeginDrawing();
    ClearBackground(TERMINALBROWN);

    DrawTextEx(consolasFont, "President:\tAhmed Mohiuddin Shah\nCEO:\tAhmed Mohiuddin Shah\nCOO:\tAhmed Mohiuddin Shah\nProducer:\tAhmed Mohiuddin Shah\n\n\nGame/Development Director:\tAhmed Mohiuddin Shah\nProject Leader/Manage:\tAhmed Mohiuddin Shah\nDesign:\tAhmed Mohiuddin Shah\nOriginal Concept/Idea:\tAhmed Mohiuddin Shah\nDesign Director:\tAhmed Mohiuddin Shah\n\n\nResearch:\tAhmed Mohiuddin Shah\nGame Design:\tAhmed Mohiuddin Shah\nPlanning:\tAhmed Mohiuddin Shah\nProgrammer:\tAhmed Mohiuddin Shah\nEngineer:\tAhmed Mohiuddin Shah\nRendering:\tAhmed Mohiuddin Shah\nPhysics:\tAhmed Mohiuddin Shah\n\n\nBased on game from:\tPortal Series By Valve\n\n\n2D/3D Artist:\tAhmed Mohiuddin Shah\nUI Artist/Graphics:\tAhmed Mohiuddin Shah\nMenu/HUD Graphics:\tAhmed Mohiuddin Shah\nArt Director:\tAhmed Mohiuddin Shah\nVisuals:\tAhmed Mohiuddin Shah\nInterface:\tAhmed Mohiuddin Shah\nConcept:\tAhmed Mohiuddin Shah\nArt:\tAhmed Mohiuddin Shah\nCharacter Design:\tAhmed Mohiuddin Shah\nStoryboard:\tAhmed Mohiuddin Shah\nIllustrationists:\tAhmed Mohiuddin Shah\nModeler:\tAhmed Mohiuddin Shah\nDesign:\tAhmed Mohiuddin Shah\nEnvironment:\tAhmed Mohiuddin Shah\n\n\nQA Manager:\tAhmed Mohiuddin Shah\nQA Team:\tMy Fellow Hostelites and Class Mates\nPlay Tester:\tAmaan\nPlay Tester:\tAbubakar\nPlay Tester:\tHannan\nPlay Tester:\tHamza\nPlay Tester:\tHaider\nPlay Tester:\tHussein\nPlay Tester:\tHammad\nPlay Tester:\tAbdullah\nPlay Tester:\tHaider\n\n\nSpecial Thanks:\tRaysan\nSpecial Thanks:\tSir Jaudat\nSpecial Thanks:\tHamza\nSpecial Thanks:\tMy Parents\n\n\n\nMADE WITH RAYLIB", (Vector2){45, creditsHeight}, 50, 0.5, TERMINALTEXTGOLD);
    DrawRectangle(0, 0, screenWidth, 120, TERMINALBROWN);
    DrawRectangleGradientV(0, 120, screenWidth, screenHeight / 2 - 200, TERMINALBROWN, (Color){0, 0, 0, 0}); // ofset of 80 from center (-80 from 200 to get 120 for center)
    DrawRectangle(0, screenHeight - 150, screenWidth, 150, TERMINALBROWN);
    DrawRectangleGradientV(0, screenHeight / 2, screenWidth, screenHeight / 2 - 150, (Color){0, 0, 0, 0}, TERMINALBROWN); // ofset of 80 from center (remove +80 for center)
    DrawTextEx(consolasFont, "CREDITS", (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Main Menu", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(310, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 260, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            creditsHeight = screenHeight - 140;
            SeekMusicStream(creditsMusic, 0.0);
            PlaySound(buttonPressSound);
            layer = MAIN_MENU;
        }
    }

    if (GetMusicTimePlayed(creditsMusic) > GetMusicTimeLength(creditsMusic) - 1)
    {
        creditsHeight = screenHeight - 140;
        SeekMusicStream(creditsMusic, 0.0);
        layer = MAIN_MENU;
    }

    if (creditsHeight < -4200)
    {
        creditsHeight = screenHeight - 140;
    }
    drawConsoleOverlay();
    EndDrawing();
}