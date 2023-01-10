#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <time.h>
#include <string.h>
#include "maze.h"
#include "variabledefinitions.h"
#include "functions.h"

int main(void)
{
	InitWindow(screenWidth, screenHeight, titleText);
	InitAudioDevice();
	// ToggleFullscreen(); Enable Afterwards
	SetRandomSeed(time(NULL));
	apertureLogo = LoadTexture("resources/ap_logo.png");
	apertureScienceLogoRectangle = (Rectangle){0.0f, 0.0f, apertureLogo.width, apertureLogo.height};
	apertureScienceLogoOrigin = (Vector2){apertureLogo.width / 4, apertureLogo.height / 4};
	windowIcon = LoadImageFromTexture(apertureLogo);
	SetWindowIcon(windowIcon);

	mainMenuMusic = LoadMusicStream("resources/MainMenu.mp3");
	creditsMusic = LoadMusicStream("resources/Credits.mp3");
	CRTBuzzMusic = LoadMusicStream("resources/CRTBuzz.wav");

	splitFlapSound = LoadSound("resources/SplitFlap.mp3");
	buttonPressSound = LoadSound("resources/ButtonPress.mp3");
	buzzerSound = LoadSound("resources/BuzzerSound.mp3");
	winSound = LoadSound("resources/WinSound.mp3");
	CRTOnOffSound = LoadSound("resources/CRTEffect.wav");

	for (int i = 0; i < MAX_LEVEL_MUSIC_NUM; i++)
	{
		levelMusic[i] = LoadMusicStream(TextFormat("resources/Music%d.mp3", i + 1));
	}

	consolasFont = LoadFontEx("resources/consolas.ttf", 96, 0, 0);

	worldCamera.position = (Vector3){-0.002371f, 15.0f, 43.558060f};
	worldCamera.target = (Vector3){0.0f, 0.0f, 0.0f};
	worldCamera.up = (Vector3){0.0f, 1.0f, 0.0f};
	worldCamera.fovy = 70.0f;
	worldCamera.projection = CAMERA_PERSPECTIVE;

	target = LoadRenderTexture(screenWidth, screenHeight);

	SetCameraMode(worldCamera, CAMERA_PERSPECTIVE);

	scanlineShader = LoadShader(0, "resources/scanlines.fs");

	loadLevel(currentLevel);

	wallCube = LoadModel("Wall.obj");
	wallTexture = LoadTexture("Wall.png");
	wallCube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;

	targetCube = LoadModel("Target.obj");
	targetCube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTexture("Target.png");

	ballModel = LoadModel("Ball.obj");

	floorModel = LoadModel("Floor.obj");
	floorBoundingBox.min = (Vector3){0.0, 0.0, 0.0};
	floorBoundingBox.max = (Vector3){160.0, 2.0, 160.0};

	endGateBoundingBox.min = (Vector3){152.0, -4.0, 152.0};
	endGateBoundingBox.max = (Vector3){156.0, 4.0, 156.0};

	SetTargetFPS(FPS);

	PlaySound(splitFlapSound);
	randomMusic = GetRandomValue(0, 5);
	SetExitKey(KEY_F4 && KEY_LEFT_ALT);
	PlayMusicStream(CRTBuzzMusic);
	splashScreen();
	while (!shouldExit && !WindowShouldClose())
	{
		switch (layer)
		{
		case MAIN_MENU:
			mainMenu();
			break;
		case LEVEL:
			level();
			break;
		case SETTINGS:
			settingsMenu();
			break;
		case PAUSED:
			paused();
			break;
		case PLAY_NEXT_LEVEL:
			nextLevel();
			break;
		case WON_SCREEN:
			winningScreen();
			break;
		case CREDITS:
			creditScreen();
			break;
		}
	}

	UnloadShader(scanlineShader);
	UnloadModel(ballModel);
	UnloadModel(targetCube);
	UnloadModel(wallCube);
	UnloadModel(floorModel);
	UnloadMusicStream(mainMenuMusic);
	UnloadMusicStream(creditsMusic);
	UnloadMusicStream(CRTBuzzMusic);
	for (int i = 0; i < 6; i++)
	{
		UnloadMusicStream(levelMusic[i]);
	}
	UnloadSound(splitFlapSound);
	UnloadSound(buzzerSound);
	UnloadSound(winSound);
	UnloadSound(CRTOnOffSound);
	UnloadSound(buttonPressSound);
	CloseWindow();

	return 0;
}