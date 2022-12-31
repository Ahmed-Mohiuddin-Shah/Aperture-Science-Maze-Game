float getGravity()
{
    return 100 * GetFrameTime() * 0.98;
}

void drawTypwriterEffect(int x, int y, int textHeight, int textLength)
{
}

void loadLevel(int currentLevel)
{
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
    DrawTextEx(consolasFont, TextFormat("%d.\n%d.\n%d.\n0%d", GetRandomValue(100, 200), GetRandomValue(120, 170), GetRandomValue(10, 50), GetRandomValue(0, 9)), (Vector2){screenWidth - 290, 25}, 25, 0.5, TERMINALTEXTGOLD);
    BeginShaderMode(scanlineShader);
    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
    DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2){0, 0}, WHITE);
    EndShaderMode();
}

void drawMap()
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

void mainMenu()
{
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, titleTextASCII, (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Play", (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 120, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 240, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Exit", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(160, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 120, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            layer = LEVEL;
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 240, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            previousLayer = layer;
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
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Window Mode: ", (Vector2){45, screenHeight - 350}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("             %s", IsWindowFullscreen() ? "<FULLSCREEN>" : "<WINDOWED>"), (Vector2){45, screenHeight - 350}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), IsWindowFullscreen() ? (Rectangle){400, screenHeight - 350, 330, 50} : (Rectangle){400, screenHeight - 350, 350, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Target FPS: ", (Vector2){45, screenHeight - 280}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("            <%d>", FPS), (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), FPS < 100 ? (Rectangle){380, screenHeight - 280, 100, 50} : (Rectangle){380, screenHeight - 280, 130, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "FPS Counter:", (Vector2){45, screenHeight - 210}, 50, 0.5, TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, TextFormat("             %s", shouldDrawFPS ? "<ON>" : "<OFF>"), (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), shouldDrawFPS ? (Rectangle){400, screenHeight - 210, 115, 50} : (Rectangle){400, screenHeight - 210, 135, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Back", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(160, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), IsWindowFullscreen() ? (Rectangle){400, screenHeight - 350, 330, 50} : (Rectangle){400, screenHeight - 350, 280, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            ToggleFullscreen();
    }

    if (CheckCollisionPointRec(GetMousePosition(), FPS < 100 ? (Rectangle){380, screenHeight - 280, 100, 50} : (Rectangle){380, screenHeight - 280, 130, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            FPS = FPS + 30;
            if (FPS > 240)
                FPS = 30;

            SetTargetFPS(FPS);
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), shouldDrawFPS ? (Rectangle){400, screenHeight - 210, 115, 50} : (Rectangle){400, screenHeight - 210, 135, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            shouldDrawFPS = shouldDrawFPS ? false : true;
    }

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            layer = previousLayer;
    }
    drawConsoleOverlay();
    EndDrawing();
}

void level()
{
    if (IsKeyPressed(KEY_P))
    {
        layer = PAUSED;
    }
    ballPosition.x = PlayerOrigin.x;
    ballPosition.z = PlayerOrigin.y;
    worldCamera.target = ballPosition;
    worldCamera.up.x = 0.0;
    worldCamera.position = (Vector3){ballPosition.x, ballPosition.y + 40, ballPosition.z + 10};
    newPosOrigin = PlayerOrigin;

    if (IsKeyDown(KEY_UP))
    {
        newPosOrigin.y -= 20 * GetFrameTime();
        worldCamera.target.z -= 1;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        newPosOrigin.y += 20 * GetFrameTime();
        worldCamera.target.z += 1;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        newPosOrigin.x += 20 * GetFrameTime();
        worldCamera.up.x -= 0.01;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        newPosOrigin.x -= 20 * GetFrameTime();
        worldCamera.up.x += 0.01;
    }

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
        // currentLevel++;
        loadLevel(currentLevel);
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
    DrawModel(wallCube, (Vector3){154, 2, 152}, 4.0, WHITE);
    DrawBoundingBox(endGateBoundingBox, RED);
    EndMode3D();
    BeginDrawing();
    drawConsoleOverlay();
    drawMap();
    EndDrawing();
}

void paused()
{
    BeginDrawing();
    ClearBackground(TERMINALBROWN);
    DrawTextEx(consolasFont, "Paused", (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);

    DrawTextEx(consolasFont, "Resume", (Vector2){45, screenHeight - 280}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 180, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Settings", (Vector2){45, screenHeight - 210}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 260, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    DrawTextEx(consolasFont, "Main Menu", (Vector2){45, screenHeight - 140}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 300, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
    drawBlinkingCursor(310, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 180, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            layer = LEVEL;
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 210, 260, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            previousLayer = layer;
            layer = SETTINGS;
        }
    }
    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 300, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            layer = MAIN_MENU;
    }

    drawConsoleOverlay();
    EndDrawing();
}