#include "editor.h"
//#include "math.h"
#include "fontManager.h"
#include "pause.h"
#include "gamepadx.h"
#include "viewManager.h"
#include "map.h"
#include "CustomMath.h"

#define EDITOR_SCALE 2.f

sfSprite* tileCursor;
sfSprite* hudEditor;
sfRectangleShape* rectCursor;

sfSprite* otherSprite;
sfTexture* tileSetTexture;

int currentTile;
sfIntRect tileCursorRect;
sfBool canPlaceTile;

sfText* txtSpace;
sfText* txtMap;
sfText* txtNbMap;
sfText* txtMove;
sfText* txtCopy;

sfBool isEditorHUD;
sfBool canHud;

void initEditor()
{
	otherSprite = sfSprite_create();
	tileSetTexture = GetTexture("tileset");
	
	//initParallax();

	//initMap();
	/// TILE CURSOR
	tileCursor = sfSprite_create();
	sfSprite_setTexture(tileCursor, tileSetTexture, sfFalse);
	currentTile = 2;
	tileCursorRect = IntRect(0 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);

	hudEditor = sfSprite_create();
	sfSprite_setTexture(hudEditor, tileSetTexture, sfFalse);
	sfSprite_setPosition(hudEditor, vector2f(0.f, 0.f));
	sfSprite_setScale(hudEditor, vector2f(2.f, 2.f));

	rectCursor = sfRectangleShape_create();
	sfRectangleShape_setSize(rectCursor, vector2f(1920.f, 1080.f));
	sfRectangleShape_setFillColor(rectCursor, color(0.f, 0.f, 0.f, 200.f));

	//initView(); 

	txtSpace = sfText_create();
	sfText_setFont(txtSpace, GetFont("GingerSoda"));
	sfText_setString(txtSpace, "Space to open the editor");
	sfText_setPosition(txtSpace, vector2f(20.f, 920.f));

	txtMap = sfText_create();
	sfText_setFont(txtMap, GetFont("GingerSoda"));
	sfText_setString(txtMap, "Numpad 1 - 9 to change maps");
	sfText_setPosition(txtMap, vector2f(20.f, 950.f));

	txtNbMap = sfText_create();
	sfText_setFont(txtNbMap, GetFont("GingerSoda"));
	sfText_setString(txtNbMap, "Current Map : 1 (M to save)");
	sfText_setPosition(txtNbMap, vector2f(20.f, 1010.f));

	txtMove = sfText_create();
	sfText_setFont(txtMove, GetFont("GingerSoda"));
	sfText_setString(txtMove, "Z - Q - S - D to move");
	sfText_setPosition(txtMove, vector2f(20.f, 890.f));

	txtCopy = sfText_create();
	sfText_setFont(txtCopy, GetFont("GingerSoda"));
	sfText_setString(txtCopy, "C + Numpad 1 - 9 to copy into this Map");
	sfText_setPosition(txtCopy, vector2f(20.f, 980.f));
	
	isEditorHUD = sfFalse;
	canPlaceTile = sfFalse;
	canHud = sfTrue;
}

void updateEditor(Window* _window)
{
	//updateView(_window);
	//updateParallax();

	static float timer = 0.f;
	timer += getDeltaTime();
	float dt = getDeltaTime();
	static float saveTimer = 0.f;
	saveTimer += dt;
	
	if (!isEditorHUD)
	{
		sfText_setString(txtSpace, "Space to open the editor");
		/// CHANGE TILE
		if (sfKeyboard_isKeyPressed(sfKeyLeft) && timer > 0.1f)
		{
			currentTile--;
			if (currentTile < 0)
				currentTile = NB_MAX_TILES;
			timer = 0.f;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyRight) && timer > 0.1f)
		{
			currentTile++;
			if (currentTile > NB_MAX_TILES)
				currentTile = 0;
			timer = 0.f;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyUp) && timer > 0.1f)
		{
			currentTile -= 8;
			if (currentTile < 0)
				currentTile = NB_MAX_TILES;
			timer = 0.f;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyDown) && timer > 0.1f)
		{
			currentTile += 8;
			if (currentTile > NB_MAX_TILES)
				currentTile = 0;
			timer = 0.f;
		}

		if (!sfKeyboard_isKeyPressed(sfKeySpace)) {
			canHud = sfTrue;
		}
		if (sfKeyboard_isKeyPressed(sfKeySpace) && canHud)
		{
			isEditorHUD = !isEditorHUD;
			canHud = sfFalse;
		}
	
		/// SET TILE CURSOR RECT
		switch (currentTile)
		{
			case T_STAIR: tileCursorRect = IntRect(0 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_CASTLEBLOCK: tileCursorRect = IntRect(0 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_GROUND: tileCursorRect = IntRect(1 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_L_BUSH: tileCursorRect = IntRect(1 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BLOCK: tileCursorRect = IntRect(2 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_M_BUSH: tileCursorRect = IntRect(2 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_QUESTION: tileCursorRect = IntRect(3 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_R_BUSH: tileCursorRect = IntRect(3 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_UL_CLOUD: tileCursorRect = IntRect(4 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BL_CLOUD: tileCursorRect = IntRect(4 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_UM_CLOUD: tileCursorRect = IntRect(5 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BM_CLOUD: tileCursorRect = IntRect(5 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_UR_CLOUD: tileCursorRect = IntRect(6 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BR_CLOUD: tileCursorRect = IntRect(6 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BLUE: tileCursorRect = IntRect(7 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BL_HILL: tileCursorRect = IntRect(7 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_UM_HILL: tileCursorRect = IntRect(8 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BM_HILL: tileCursorRect = IntRect(8 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_GREEN: tileCursorRect = IntRect(9 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BR_HILL: tileCursorRect = IntRect(9 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_UL_PIPE: tileCursorRect = IntRect(10 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BL_PIPE: tileCursorRect = IntRect(10 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_UR_PIPE: tileCursorRect = IntRect(11 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_BR_PIPE: tileCursorRect = IntRect(11 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_U_FLAG: tileCursorRect = IntRect(12 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_B_FLAG: tileCursorRect = IntRect(12 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_U_EMPTYCASTLE: tileCursorRect = IntRect(13 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_R_WINDOWCASTLE: tileCursorRect = IntRect(13 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_U_DOORCASTLE: tileCursorRect = IntRect(14 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_B_DOORCASTLE: tileCursorRect = IntRect(14 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_U_CASTLE: tileCursorRect = IntRect(15 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			case T_L_WINDOWCASTLE: tileCursorRect = IntRect(15 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
			
			default: break;
		}

		if (sfMouse_isButtonPressed(sfMouseLeft))
		{
			int y = getfWorldMousePos(_window->renderWindow).y / BLOCK_SIZE / BLOCK_SCALE;
			int x = getfWorldMousePos(_window->renderWindow).x / BLOCK_SIZE / BLOCK_SCALE;
			if (x < 0 || y < 0 || y >= NB_BLOCKS_Y ) {
				printf("DEPASSEMENT DE TABLEAU ");
				//ToggleFullscreen(_window);
				return;
			}
			b[y][x].type = currentTile;
			b[y][x].isSolid = sfFalse;
			b[y][x].timer = 0.f;
			b[y][x].pos = vector2f(x * BLOCK_SCALE * BLOCK_SIZE, y * BLOCK_SCALE * BLOCK_SIZE);
			int j = y;
			int i = x;

			//for (int j = 0; j < NB_BLOCKS_Y; j++)
			//{
			//	for (int i = 0; i < NB_BLOCKS_X; i++)
			//	{
					switch (b[j][i].type)
					{
					case T_STAIR: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_CASTLEBLOCK: b[j][i].rect = IntRect(0 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_GROUND: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_L_BUSH: b[j][i].rect = IntRect(1 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BLOCK: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_M_BUSH: b[j][i].rect = IntRect(2 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_QUESTION: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_R_BUSH: b[j][i].rect = IntRect(3 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_UL_CLOUD: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BL_CLOUD: b[j][i].rect = IntRect(4 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_UM_CLOUD: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BM_CLOUD: b[j][i].rect = IntRect(5 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_UR_CLOUD: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BR_CLOUD: b[j][i].rect = IntRect(6 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BLUE: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BL_HILL: b[j][i].rect = IntRect(7 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_UM_HILL: b[j][i].rect = IntRect(8 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BM_HILL: b[j][i].rect = IntRect(8 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_GREEN: b[j][i].rect = IntRect(9 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BR_HILL: b[j][i].rect = IntRect(9 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_UL_PIPE: b[j][i].rect = IntRect(10 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BL_PIPE: b[j][i].rect = IntRect(10 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_UR_PIPE: b[j][i].rect = IntRect(11 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_BR_PIPE: b[j][i].rect = IntRect(11 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_U_FLAG: b[j][i].rect = IntRect(12 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_B_FLAG: b[j][i].rect = IntRect(12 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_U_EMPTYCASTLE: b[j][i].rect = IntRect(13 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_R_WINDOWCASTLE: b[j][i].rect = IntRect(13 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_U_DOORCASTLE: b[j][i].rect = IntRect(14 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_B_DOORCASTLE: b[j][i].rect = IntRect(14 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_U_CASTLE: b[j][i].rect = IntRect(15 * BLOCK_SIZE, 0 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;
					case T_L_WINDOWCASTLE: b[j][i].rect = IntRect(15 * BLOCK_SIZE, 1 * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE); break;

					default: break;
					//}
				//}
			}
		}
	}
	else
	{
		sfText_setString(txtSpace, "Select a tile");
		if (!sfKeyboard_isKeyPressed(sfKeySpace)) {
			canHud = sfTrue;
		}
		if (sfKeyboard_isKeyPressed(sfKeySpace) && canHud)
		{
			isEditorHUD = !isEditorHUD;
			canHud = sfFalse;
		}

		if (canPlaceTile && !sfMouse_isButtonPressed(sfMouseLeft)) {
			canPlaceTile = sfFalse;
			isEditorHUD = sfFalse;
		}

		if (sfMouse_isButtonPressed(sfMouseLeft))
		{
			timer = 0.f;
			canPlaceTile = sfTrue;
			float mousePosX = getfMousePos(_window->renderWindow).x;
			float mousePosY = getfMousePos(_window->renderWindow).y;



			if ((mousePosX > 0.f * EDITOR_SCALE && mousePosX < 32.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_STAIR;
			else if ((mousePosX > 0.f * EDITOR_SCALE && mousePosX < 32.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_CASTLEBLOCK;
			else if ((mousePosX > 32.f * EDITOR_SCALE && mousePosX < 64.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_GROUND;
			else if ((mousePosX > 32.f * EDITOR_SCALE && mousePosX < 64.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_L_BUSH;
			else if ((mousePosX > 64.f * EDITOR_SCALE && mousePosX < 96.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_BLOCK;
			else if ((mousePosX > 64.f * EDITOR_SCALE && mousePosX < 96.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_M_BUSH;
			else if ((mousePosX > 96.f * EDITOR_SCALE && mousePosX < 128.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_QUESTION;
			else if ((mousePosX > 96.f * EDITOR_SCALE && mousePosX < 128.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_R_BUSH;

			else if ((mousePosX > 128.f * EDITOR_SCALE && mousePosX < 160.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_UL_CLOUD;
			else if ((mousePosX > 128.f * EDITOR_SCALE && mousePosX < 160.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BL_CLOUD;
			else if ((mousePosX > 160.f * EDITOR_SCALE && mousePosX < 192.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_UM_CLOUD;
			else if ((mousePosX > 160.f * EDITOR_SCALE && mousePosX < 192.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BM_CLOUD;
			else if ((mousePosX > 192.f * EDITOR_SCALE && mousePosX < 224.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_UR_CLOUD;
			else if ((mousePosX > 192.f * EDITOR_SCALE && mousePosX < 224.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BR_CLOUD;
			else if ((mousePosX > 224.f * EDITOR_SCALE && mousePosX < 256.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_BLUE;
			else if ((mousePosX > 224.f * EDITOR_SCALE && mousePosX < 256.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BL_HILL;

			else if ((mousePosX > 256.f * EDITOR_SCALE && mousePosX < 288.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_UM_HILL;
			else if ((mousePosX > 256.f * EDITOR_SCALE && mousePosX < 288.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BM_HILL;
			else if ((mousePosX > 288.f * EDITOR_SCALE && mousePosX < 320.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_GREEN;
			else if ((mousePosX > 288.f * EDITOR_SCALE && mousePosX < 320.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BR_HILL;
			else if ((mousePosX > 320.f * EDITOR_SCALE && mousePosX < 352.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_UL_PIPE;
			else if ((mousePosX > 320.f * EDITOR_SCALE && mousePosX < 352.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BL_PIPE;
			else if ((mousePosX > 352.f * EDITOR_SCALE && mousePosX < 384.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_UR_PIPE;
			else if ((mousePosX > 352.f * EDITOR_SCALE && mousePosX < 384.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_BR_PIPE;

			else if ((mousePosX > 384.f * EDITOR_SCALE && mousePosX < 416.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_U_FLAG;
			else if ((mousePosX > 384.f * EDITOR_SCALE && mousePosX < 416.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_B_FLAG;
			else if ((mousePosX > 416.f * EDITOR_SCALE && mousePosX < 448.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_U_EMPTYCASTLE;
			else if ((mousePosX > 416.f * EDITOR_SCALE && mousePosX < 448.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_R_WINDOWCASTLE;
			else if ((mousePosX > 448.f * EDITOR_SCALE && mousePosX < 480.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_U_DOORCASTLE;
			else if ((mousePosX > 448.f * EDITOR_SCALE && mousePosX < 480.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_B_DOORCASTLE;
			else if ((mousePosX > 480.f * EDITOR_SCALE && mousePosX < 512.f * EDITOR_SCALE) && (mousePosY > 0.f * EDITOR_SCALE && mousePosY < 32.f * EDITOR_SCALE))
				currentTile = T_U_CASTLE;
			else if ((mousePosX > 480.f * EDITOR_SCALE && mousePosX < 512.f * EDITOR_SCALE) && (mousePosY > 32.f * EDITOR_SCALE && mousePosY < 64.f * EDITOR_SCALE))
				currentTile = T_L_WINDOWCASTLE;


			else
				canPlaceTile = sfFalse;
		}
	}

	// to remove improve
	//if (sfKeyboard_isKeyPressed(sfKeyEscape) && timer > 0.2f)
	//{
	//	isEditor = sfFalse;
	//	timer = 0.f;
	//}

		//if (getPause())
		//	updatePause(_window);

		/// CHANGE MAPS
	int key = 0;
	if (sfKeyboard_isKeyPressed(sfKeyNumpad1)) key = 1;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad2)) key = 2;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad3)) key = 3;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad4)) key = 4;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad5)) key = 5;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad6)) key = 6;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad7)) key = 7;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad8)) key = 8;
	else if (sfKeyboard_isKeyPressed(sfKeyNumpad9)) key = 9;
	if (key > 0 && saveTimer > 0.5f) {
		saveTimer = 0.f;
		if (sfKeyboard_isKeyPressed(sfKeyC)) {
			copyMap(key);

			switch (nbMap)
			{
			case 1: sfText_setString(txtNbMap, "Copied into : 1 (M to save)"); break;
			case 2: sfText_setString(txtNbMap, "Copied into : 2 (M to save)"); break;
			case 3: sfText_setString(txtNbMap, "Copied into : 3 (M to save)"); break;
			case 4: sfText_setString(txtNbMap, "Copied into : 4 (M to save)"); break;
			case 5: sfText_setString(txtNbMap, "Copied into : 5 (M to save)"); break;
			case 6: sfText_setString(txtNbMap, "Copied into : 6 (M to save)"); break;
			case 7: sfText_setString(txtNbMap, "Copied into : 7 (M to save)"); break;
			case 8: sfText_setString(txtNbMap, "Copied into : 8 (M to save)"); break;
			case 9: sfText_setString(txtNbMap, "Copied into : 9 (M to save)"); break;
			default:
				break;
			}
		}
		else {
			loadMap(key);
			nbMap = key;

			switch (nbMap)
			{
			case 1: sfText_setString(txtNbMap, "Current Map : 1 (M to save)"); break;
			case 2: sfText_setString(txtNbMap, "Current Map : 2 (M to save)"); break;
			case 3: sfText_setString(txtNbMap, "Current Map : 3 (M to save)"); break;
			case 4: sfText_setString(txtNbMap, "Current Map : 4 (M to save)"); break;
			case 5: sfText_setString(txtNbMap, "Current Map : 5 (M to save)"); break;
			case 6: sfText_setString(txtNbMap, "Current Map : 6 (M to save)"); break;
			case 7: sfText_setString(txtNbMap, "Current Map : 7 (M to save)"); break;
			case 8: sfText_setString(txtNbMap, "Current Map : 8 (M to save)"); break;
			case 9: sfText_setString(txtNbMap, "Current Map : 9 (M to save)"); break;
			default:
				break;
			}
		}
	}

	if (sfKeyboard_isKeyPressed(sfKeyM) && saveTimer > 0.5f) {
		saveTimer = 0.f;
		saveMap(nbMap);
		sfText_setString(txtNbMap, "Map saved", nbMap);
	}

	if ((isButtonPressed(0, START) || sfKeyboard_isKeyPressed(sfKeyEscape)) && timer > 0.2f) {
		timer = 0.f;
		togglePause();
	}

	if (sfKeyboard_isKeyPressed(sfKeyQ)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x - 1000.f * dt, GetViewPosition(mainView).y));
	if (sfKeyboard_isKeyPressed(sfKeyD)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x + 1000.f * dt, GetViewPosition(mainView).y));
	if (sfKeyboard_isKeyPressed(sfKeyS)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x, GetViewPosition(mainView).y + 1000.f * dt));
	if (sfKeyboard_isKeyPressed(sfKeyZ)) SetViewPosition(mainView, vector2f(GetViewPosition(mainView).x, GetViewPosition(mainView).y - 1000.f * dt));
}

void displayEditor(Window* _window)
{
	/// TILE CURSOR
	sfSprite_setScale(tileCursor, vector2f(2.f, 2.f));

	sfSprite_setTexture(tileCursor, tileSetTexture, sfFalse);

	sfSprite_setTextureRect(tileCursor, tileCursorRect);
	
	sfVector2f pos = vector2f(getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y);
	sfSprite_setPosition(tileCursor, vector2f(getfWorldMousePos(_window->renderWindow).x, getfWorldMousePos(_window->renderWindow).y));
	sfRenderTexture_drawSprite(_window->renderTexture, tileCursor, NULL);

	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));
	sfRenderTexture_drawText(_window->renderTexture, txtMap, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtSpace, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtNbMap, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtMove, NULL);
	sfRenderTexture_drawText(_window->renderTexture, txtCopy, NULL);
	if (isEditorHUD)
	{
		sfRenderTexture_drawRectangleShape(_window->renderTexture, rectCursor, NULL);

		sfSprite_setTexture(hudEditor, tileSetTexture, sfTrue);
		sfSprite_setPosition(hudEditor, vector2f(0.f, 0.f));
		sfSprite_setScale(hudEditor, vector2f(4.f, 4.f));
		sfRenderTexture_drawSprite(_window->renderTexture, hudEditor, NULL);

	}
	sfRenderTexture_setView(_window->renderTexture, mainView->view);

	//if (getPause())
	//	displayPause(_window);
}


void saveMap(int _nbMap)
{
	FILE* file;
	switch (_nbMap)
	{
		case 1:
			file = fopen(SAVE_PATH"map1.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file); 
				int m = 4;
			}
			break;
		case 2:
			file = fopen(SAVE_PATH"map2.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 3:
			file = fopen(SAVE_PATH"map3.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 4:
			file = fopen(SAVE_PATH"map4.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 5:
			file = fopen(SAVE_PATH"map5.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 6:
			file = fopen(SAVE_PATH"map6.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 7:
			file = fopen(SAVE_PATH"map7.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 8:
			file = fopen(SAVE_PATH"map8.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 9:
			file = fopen(SAVE_PATH"map9.bin", "wb");
			if (file != NULL) {
				fwrite(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		default:
			break;
	}
}

void copyMap(int _MapToCopy)
{
	FILE* file;
	switch (_MapToCopy)
	{
		case 1:
			file = fopen(SAVE_PATH"map1.bin", "rb");
			if (file == NULL) {
				defaultMap();
				file = fopen(SAVE_PATH"map1.bin", "wb");
				fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			else {
				fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 2:
			file = fopen(SAVE_PATH"map2.bin", "rb");
			if (file == NULL) {
				defaultMap();
				file = fopen(SAVE_PATH"map2.bin", "wb");
				fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			else {
				fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 3:
			file = fopen(SAVE_PATH"map3.bin", "rb");
			if (file == NULL) {
				defaultMap();
				file = fopen(SAVE_PATH"map3.bin", "wb");
				fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			else {
				fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 4:
			file = fopen(SAVE_PATH"map4.bin", "rb");
			if (file == NULL) {
				defaultMap();
				file = fopen(SAVE_PATH"map4.bin", "wb");
				fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			else {
				fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 5:
			file = fopen(SAVE_PATH"map5.bin", "rb");
			if (file == NULL) {
				defaultMap();
				file = fopen(SAVE_PATH"map5.bin", "wb");
				fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			else {
				fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		case 6:
			file = fopen(SAVE_PATH"map6.bin", "rb");
			if (file == NULL) {
				defaultMap();
				file = fopen(SAVE_PATH"map6.bin", "wb");
				fwrite(b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			else {
				fread(&b, sizeof(Blocks), NB_BLOCKS_X * NB_BLOCKS_Y, file);
				fclose(file);
			}
			break;
		default:
			break;
	}
}

void deinitEditor()
{
	sfSprite_destroy(tileCursor);
	sfSprite_destroy(hudEditor);
	sfRectangleShape_destroy(rectCursor);
	sfSprite_destroy(otherSprite);
	sfText_destroy(txtSpace);
	sfText_destroy(txtMap);
	sfText_destroy(txtNbMap);
	sfText_destroy(txtMove);
	sfText_destroy(txtCopy);
}