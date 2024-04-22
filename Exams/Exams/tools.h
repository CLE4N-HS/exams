#pragma once

#include "SFML/Graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <windows.h>

#define TEXTURE_PATH "..\\Ressources\\Textures\\"
#define FONT_PATH "..\\Ressources\\Fonts\\"
#define MAP_PATH "..\\Ressources\\Maps\\"
#define TILESET_PATH "..\\Ressources\\Tilesets\\"

typedef struct vector4f vector4f;
struct vector4f
{
	float r;
	float g;
	float b;
	float a;
};

float timeScale;

//-----------------------------------------------------------
//	Fonction d'initialisation des tools
//	à mettre obligatoirement dans votre init 
//	pour utiliser les fonctions de cette bibliothèque
//-----------------------------------------------------------
void initTools();

//-----------------------------------------------------------
//	Fonction getDefaultFont ( )
//  Renvoi la police par défaut
//-----------------------------------------------------------
sfFont* getDefaultFont();

//-----------------------------------------------------------
//	Fonction vector2f ( float _x, float _y )
//  Renvoi un sfVector2f en partant de 2 float
//	_x float
//	_y float
//-----------------------------------------------------------
sfVector2f vector2f(float _x, float _y);

//------------------------------------------------------------------------------
//	Fonction FlRect ( float _left, float _top, float _width, float _height )
//  Renvoi un sfFloatRect en partant de 4 float
//	_left float
//	_top float
//	_width float
//	_height float
//------------------------------------------------------------------------------
sfFloatRect FlRect(float _left, float _top, float _width, float _height);

//-----------------------------------------------------------
//	Fonction vector2i ( int _left, int _top, int _width, int _height )
//  Renvoi un sfIntRect en partant de 4 int
//	_left int
//	_top int
// 	_width int
//	_height int
//-----------------------------------------------------------
sfIntRect IntRect(int _left, int _top, int _width, int _height);

//-----------------------------------------------------------
//	Fonction vector2i ( sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a )
//  Renvoi un sfColor en partant de 4 int
//	r rouge	: valeur entre 0 et 255
//	g vert	: valeur entre 0 et 255
// 	b bleu	: valeur entre 0 et 255
//	a alpha	: valeur entre 0 et 255
//-----------------------------------------------------------
sfColor color(sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a);

//-----------------------------------------------------------
//	Fonction lerp ( sfColor )
//	v0 couleur de départ
//	v1 couleur de fin
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
sfColor color_lerp(sfColor v0, sfColor v1, float t);

//-----------------------------------------------------------
//	Fonction restartClock()
//	fonction à mettre dès le début de votre update
//	pour gèrer le temps
//-----------------------------------------------------------
void restartClock();

//-----------------------------------------------------------
//	Fonction GetDeltaTime()
//	Renvoi un float du temps passé entre 2 ticks
//  Tient compte du timeScale
//-----------------------------------------------------------
float getDeltaTime();

//-----------------------------------------------------------
//	Fonction GetDeltaTime()
//	Renvoi un float du temps passé entre 2 ticks
//  Ne tient pas compte du timeScale
//-----------------------------------------------------------
float getUnscaledDeltaTime();

//-----------------------------------------------------------
//	Fonction CreateSprite(char* _path)
//	Crée un sprite à partir d'une texture
//	_path le chemin vers la texture 
// 
//	renvoi un sfSprite*
//-----------------------------------------------------------
sfSprite* CreateSprite(char* _path);

//-----------------------------------------------------------
//	Fonction CreateText(char* _path)
//	Crée un texte à partir d'une font
//	_path le chemin vers la font 
// 
//	renoi un sfText*
//-----------------------------------------------------------
sfText* CreateText(char* _path);

//-----------------------------------------------------------
//	Fonction FrameRate 
//-----------------------------------------------------------
float FrameRate();

//-----------------------------------------------------------
//	Fonction displayFrameRate
//-----------------------------------------------------------
void displayFrameRate(sfRenderWindow* _window, sfText* _text);

//-----------------------------------------------------------
//	Fonction screenshot
//-----------------------------------------------------------
void screenshot(sfRenderWindow* _window);

/// <summary>
/// Gets the frame in X of a Sprite (linked with Animator())
/// </summary>
/// <param name="_rect">The rect of the Sprite</param>
/// <returns>The current number of frames in X of the sprite (starting at 1) </returns>
int getFrameX(sfIntRect _rect);

/// <summary>
/// Gets the frame in Y of a Sprite (linked with Animator())
/// </summary>
/// <param name="_rect">The rect of the Sprite</param>
/// <returns>The current number of frames in Y of the sprite (starting at 1) </returns>
int getFrameY(sfIntRect _rect);


/// <summary>
/// Animates a Sprite, modifies directly the rect and timer, only works with Sprites that have the same width and height all along.
/// After a row the rect will be reset at 0 in X.
/// The time between each frame must be the same too.
/// This function doesn't increment the timer
/// </summary>
/// <param name="_rect">The rect of the Sprite</param>
/// <param name="_timer">The timer of your animation</param>
/// <param name="_maxFrameX">The last frame in X, (starting at 1)</param>
/// <param name="_maxFrameY">The last frame in Y, (starting at 1) (default value -> 1 means no changes in Y)</param>
/// <param name="_maxTimer">The time between each frame</param>
/// <param name="_resetTimer">The reset value of the timer (default value -> 0.f)</param>
void Animator(sfIntRect* _rect, float* _timer, int _maxFrameX, int _maxFrameY, float _maxTimer, float _resetTimer);
