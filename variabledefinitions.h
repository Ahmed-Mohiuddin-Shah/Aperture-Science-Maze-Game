
#define RectCount 40
#define MAX_LEVEL_MUSIC_NUM 6
enum LAYERS
{
    MAIN_MENU,
    LEVEL,
    SETTINGS,
    PAUSED,
    WON_SCREEN,
    PLAY_NEXT_LEVEL,
    CREDITS
};

enum KEY_DEFINITIONS
{
    PAUSE_KEY = KEY_ESCAPE,
    HELP_KEY = KEY_F1,
    RESET_KEY = KEY_R,
    SHOW_MAP_KEY = KEY_M,
    MOVE_UP_KEY = KEY_W, // When Changing movement keys keep in mind that the keys should have sinlge character name otherwse they dont display in help menu
    MOVE_DOWN_KEY = KEY_S,
    MOVE_LEFT_KEY = KEY_A,
    MOVE_RIGHT_KEY = KEY_D
};

int screenWidth = 1600;
int screenHeight = 900;

const int constScreenWidth = 1600;
const int constScreenHeight = 900;

unsigned int FPS = 120;
unsigned int layer = MAIN_MENU;
unsigned int previousLayer = 0;
unsigned int currentLevel = 0;
unsigned int cursorBlinkCounter = 0;
unsigned int randomLinePosition = 0;

int creditsHeight = 120;
int crtEffectRectangleHeight = -constScreenHeight;
int randomMusic = 0;
int ballRotationSpeed = 0;

bool cursorVisible = true;

float elapsedTime = 0.0f;
float blinkInterval = 0.5f;

char titleText[] = "Aperture Science Maze Testing Initiative";
char titleTextASCII[] = "Maze Testing\nInitiative";

float Radius = 1.0;
float apertureLogoRotaion = 0;

char loadingDots[30] = "";
int dotLimit = 0;

bool shouldDrawFPS = false;
bool shouldExit = false;
bool shouldDrawMap = true;
bool shouldDrawCRTEffect = true;
bool shouldDisplayHelp = false;

Rectangle rectanglesOfLevel[RectCount][RectCount] = {}, apertureScienceLogoRectangle;

Camera worldCamera = {0};

Model wallCube, floorModel, ballModel, targetCube;

float swap;

BoundingBox floorBoundingBox, ballBoundingBox, endGateBoundingBox;

Texture2D wallTexture, apertureLogo;
Image windowIcon;

RenderTexture2D target;

Shader scanlineShader;

Vector2 newPosOrigin;
Vector2 apertureScienceLogoOrigin;
Vector2 PlayerOrigin = {0, 0};
Vector2 apertureLogoGoToCorner = {0, 0};

Vector3 ballPosition = {0, 2.5, 0};
Vector3 ballRotation = {0, 1, 0};

Font consolasFont;

Color TERMINALBROWN = {99, 59, 7, 255};
Color TERMINALOUTLINEYELLOW = {159, 121, 25, 255};
Color TERMINALTEXTGOLD = {255, 185, 9, 255};
Color TERMINALTEXTGOLD_CRT_ALPHA = {255, 185, 9, 15};

Music mainMenuMusic, creditsMusic, levelMusic[MAX_LEVEL_MUSIC_NUM], CRTBuzzMusic;

Sound splitFlapSound, buttonPressSound, buzzerSound, winSound, CRTOnOffSound;