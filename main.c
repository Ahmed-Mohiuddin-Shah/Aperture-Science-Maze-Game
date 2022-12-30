#include <raylib.h>
#include <raymath.h>
#include "maze.h"
#include "variabledefinitions.h"
#include "functions.h"

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, titleText);
	// ToggleFullscreen();

	consolasFont = LoadFontEx("resources/consolas.ttf", 96, 0, 0);

	worldCamera.position = (Vector3){-0.002371f, 15.0f, 43.558060f};
	worldCamera.target = (Vector3){0.0f, 0.0f, 0.0f};
	worldCamera.up = (Vector3){0.0f, 1.0f, 0.0f};
	worldCamera.fovy = 70.0f;
	worldCamera.projection = CAMERA_PERSPECTIVE;

	target = LoadRenderTexture(screenWidth, screenHeight);

	SetCameraMode(worldCamera, CAMERA_FREE);

	scanlineShader = LoadShader(0, "resources/scanlines.fs");

	for (int i = 0; i < RectCount; i++)
	{
		for (int j = 0; j < RectCount; j++)
		{
			if (level[i][j])
			{
				rectanglesOfLevel1[i][j] = (Rectangle){i * 4, j * 4, 4, 4};
			}
		}
	}

	wallCube = LoadModel("Wall.obj");
	wallTexture = LoadTexture("Wall.png");
	wallCube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;
	floorModel = LoadModel("Floor.obj");

	SetTargetFPS(FPS);

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		switch (layer)
		{
		case 0:
			mainMenu();
			break;
		case 1:
			level_1();
			break;
		}
	}
	UnloadShader(scanlineShader);
	UnloadModel(wallCube);
	UnloadModel(floorModel);
	CloseWindow();

	return 0;
}