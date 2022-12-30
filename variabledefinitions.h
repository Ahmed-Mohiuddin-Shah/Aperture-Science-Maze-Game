
#define RectCount 40

const int screenWidth = 1600;
const int screenHeight = 900;

unsigned int FPS = 120;
unsigned int layer = 0;
unsigned int cursorBlinkCounter = 0;
bool cursorVisible = true;
float elapsedTime = 0.0f;
float blinkInterval = 0.5f;

char titleText[] = "Aperture Science Maze Testing Initiative";
char titleTextASCII[] = "Maze Testing\nInitiative";

float Radius = 1.0;

bool shouldDrawFPS = false;

Rectangle rectanglesOfLevel1[RectCount][RectCount] = {};

Camera worldCamera = {0};

Model wallCube, floorModel;

Texture2D wallTexture;
RenderTexture2D target;

Shader scanlineShader;

Vector2 newPosOrigin;
Vector2 PlayerOrigin = {0, 0};

Vector3 ballPosition = {0, 2.5, 0};

Font consolasFont;

Color TERMINALBROWN = {99, 59, 7, 255};
Color TERMINALOUTLINEYELLOW = {159, 121, 25, 255};
Color TERMINALTEXTGOLD = {255, 185, 9, 255};

Sound menuOptionsSound;