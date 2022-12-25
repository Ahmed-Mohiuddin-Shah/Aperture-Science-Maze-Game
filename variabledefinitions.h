bool CollisionWithFloor = false, CollisionWithBoundary = false, isBallCollidedWithWall = false;

Camera camera;

Model wallCube, ball;

Mesh ModelMesh;

Vector3 BallPosition, FloorPosition, FloorSize, CubePosition, CubeSize;
float BallRadius = 0.5f;
Vector3 BallSpeedVector;

BoundingBox FloorBoundingBox, CubeBoundingBox, ballBoundingBox;

Rectangle ExitButton, InputButton, SettingsButton, ScoreBoard, PlayButton, ballRectangle, wallRectangle, collision;

Font HelloHeadline;

Music GladosMusicSelectionVoice;

Vector2 vect;

Texture2D wallTexture, ballTexture;

Image WindowIcon;

Color TERMINALBROWN = {99, 59, 7, 255};
Color TERMINALOUTLINEYELLOW = {119, 91, 19, 255};

int WindowWidth = 1600;
int WindowHeight = 900;

float MainMenuButtonWidth = 400;
float MainMenuButtonHeight = 50;
float spacing = 20;
float FPS = 120.0;

float gravity;

int shouldExit = 0;
int layers = 2;

int delayFrame = 0;
int delay = 3;

float difX = 0;
float difZ = 0;
bool collidedWithX = false;
bool collidedWithY = false;