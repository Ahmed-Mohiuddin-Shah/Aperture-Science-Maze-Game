void loadLevel(bool *level, unsigned int currentLevel)
{
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
        DrawTextEx(consolasFont, "_", (Vector2){x, y}, 50, 0.5, CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 140, 120, 50}) ? TERMINALOUTLINEYELLOW : TERMINALTEXTGOLD);
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
    DrawRectangleRec((Rectangle){screenWidth - 470, 12, 160, 160}, TERMINALBROWN);
    DrawRectangleLinesEx((Rectangle){screenWidth - 470, 12, 160, 160}, 5.0f, TERMINALOUTLINEYELLOW);
    if (shouldDrawFPS)
        DrawFPS(1, 1);
    DrawTextEx(consolasFont, "192.\n168.\n18.\n01", (Vector2){screenWidth - 290, 25}, 25, 0.5, TERMINALTEXTGOLD);
    BeginShaderMode(scanlineShader);
    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
    DrawTextureRec(target.texture, (Rectangle){0, 0, (float)target.texture.width, (float)-target.texture.height}, (Vector2){0, 0}, WHITE);
    EndShaderMode();
}

void drawMap()
{
    for (int i = 0; i < RectCount; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            if (level[i][j])
            {
                DrawRectangleRec((Rectangle){rectanglesOfLevel1[i][j].x + screenWidth - 470, rectanglesOfLevel1[i][j].y + 12, rectanglesOfLevel1[i][j].width, rectanglesOfLevel1[i][j].height}, TERMINALOUTLINEYELLOW);
            }
        }
    }
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
    drawBlinkingCursor(120, screenHeight - 140);

    if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){45, screenHeight - 280, 120, 50}))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            layer = 1;
    }

    drawConsoleOverlay();
    EndDrawing();
}

void settingsMenu()
{
    BeginDrawing();
    ClearBackground(TERMINALBROWN);

    DrawTextEx(consolasFont, titleTextASCII, (Vector2){45, 30}, 100, 0.5, TERMINALTEXTGOLD);
    drawConsoleOverlay();
    EndDrawing();
}

void level_1()
{
    ballPosition.x = PlayerOrigin.x;
    ballPosition.z = PlayerOrigin.y;
    worldCamera.target = (Vector3){ballPosition.x, ballPosition.y, ballPosition.z - 25};
    worldCamera.position = (Vector3){ballPosition.x, ballPosition.y - 10, ballPosition.z};
    newPosOrigin = PlayerOrigin;

    if (IsKeyDown(KEY_UP))
        newPosOrigin.y -= 20 * GetFrameTime();

    if (IsKeyDown(KEY_DOWN))
        newPosOrigin.y += 20 * GetFrameTime();

    if (IsKeyDown(KEY_RIGHT))
        newPosOrigin.x += 20 * GetFrameTime();

    if (IsKeyDown(KEY_LEFT))
        newPosOrigin.x -= 20 * GetFrameTime();

    for (int i = 0; i < RectCount; i++)
    {
        for (int j = 0; j < RectCount; j++)
        {
            Vector2 hitPoint = {-100, -100};
            Vector2 hitNormal = {0, 0};
            PointNearestRectanglePoint(rectanglesOfLevel1[i][j], newPosOrigin, &hitPoint, &hitNormal);

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

    PlayerOrigin = newPosOrigin;
    UpdateCamera(&worldCamera);
    BeginMode3D(worldCamera);
    ClearBackground(TERMINALBROWN);
    DrawModelEx(
        floorModel, (Vector3){80.0, 0.0, 80.0}, (Vector3){1.0, 0.0, 0.0}, 90, (Vector3){4.0, 4.0, 0.0}, WHITE);
    DrawSphere(ballPosition, Radius, ORANGE);
    for (int i = 0; i < RectCount; i++)
    {
        for (int j = 0; j < 40; j++)
        {
            if (level[i][j])
            {
                DrawModel(wallCube, (Vector3){rectanglesOfLevel1[i][j].x + rectanglesOfLevel1[i][j].width / 2, 2, rectanglesOfLevel1[i][j].y}, 4.0, WHITE);
            }
        }
    }
    EndMode3D();
    BeginDrawing();
    drawConsoleOverlay();
    drawMap();
    EndDrawing();
}