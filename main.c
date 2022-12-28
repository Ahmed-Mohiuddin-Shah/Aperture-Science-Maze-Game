#include <raylib.h>
#include <raymath.h>
#include "maze.h"

#define RectCount 40
Rectangle rectanglesOfLevel1[RectCount][RectCount] = {};

Camera worldCamera = {0};

Model wallCube;

Texture2D wallTexture;

Vector2 newPosOrigin;

Vector3 ballPosition = {0, 2.5, 0};

Color TERMINALBROWN = {99, 59, 7, 255};
Color TERMINALOUTLINEYELLOW = {119, 91, 19, 255};

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

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------

	const int screenWidth = 1600;
	const int screenHeight = 900;

	InitWindow(screenWidth, screenHeight, "Aperture Science Maze Testing Initiative");

	worldCamera.position = (Vector3){-0.002371f, 15.0f, 43.558060f};
	worldCamera.target = (Vector3){0.0f, 0.0f, 0.0f};
	worldCamera.up = (Vector3){0.0f, 1.0f, 0.0f};
	worldCamera.fovy = 70.0f;
	worldCamera.projection = CAMERA_PERSPECTIVE;

	SetCameraMode(worldCamera, CAMERA_FREE);

	for (int i = 0; i < RectCount; i++)
	{
		for (int j = 0; j < RectCount; j++)
		{
			if (level1[i][j])
			{
				rectanglesOfLevel1[i][j] = (Rectangle){i * 4, j * 4, 4, 4};
			}
		}
	}

	wallCube = LoadModel("Wall.obj");
	wallTexture = LoadTexture("Wall.png");
	wallCube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;

	SetTargetFPS(120);

	Vector2 PlayerOrigin = {0, 0};

	float Radius = 1.0;

	// Main game loop
	while (!WindowShouldClose()) // Detect window close button or ESC key
	{
		ballPosition.x = PlayerOrigin.x;
		ballPosition.z = PlayerOrigin.y;
		worldCamera.target = (Vector3){ballPosition.x, 20, ballPosition.z};
		worldCamera.position = (Vector3){ballPosition.x, 1, ballPosition.z};
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
		BeginDrawing();
		ClearBackground(TERMINALBROWN);
		BeginMode3D(worldCamera);
		DrawSphere(ballPosition, Radius, ORANGE);
		for (int i = 0; i < RectCount; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				if (level1[i][j])
				{
					// DrawCube((Vector3){rectanglesOfLevel1[i][j].x + rectanglesOfLevel1[i][j].width / 2, 2, rectanglesOfLevel1[i][j].y + rectanglesOfLevel1[i][j].height / 2}, rectanglesOfLevel1[i][j].width, 2, rectanglesOfLevel1[i][j].height, BLUE);
					DrawModel(wallCube, (Vector3){rectanglesOfLevel1[i][j].x + rectanglesOfLevel1[i][j].width / 2, 2, rectanglesOfLevel1[i][j].y - rectanglesOfLevel1[i][j].height / 2}, 4.0, WHITE);
				}
			}
		}
		DrawGrid(10, 1.0f);
		EndMode3D();

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
		for (int i = 0; i < RectCount; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				if (level1[i][j])
				{
					DrawRectangleRec((Rectangle){rectanglesOfLevel1[i][j].x + screenWidth - 470, rectanglesOfLevel1[i][j].y + 12, rectanglesOfLevel1[i][j].width, rectanglesOfLevel1[i][j].height}, RED);
				}
			}
		}
		DrawCircle(PlayerOrigin.x + screenWidth - 470, PlayerOrigin.y + 12, Radius * 2, BLUE);

		DrawFPS(1, 1);
		EndDrawing();
	}
	CloseWindow();

	return 0;
}