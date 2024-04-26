#include "tools.h"

sfTime sftime;
sfClock* sfclock;
sfFont* defaultFont;


void initTools()
{
	sfclock = sfClock_create();
	srand(time(NULL));
	defaultFont = sfFont_createFromFile(FONT_PATH"marioFont.ttf");
}

sfFont* getDefaultFont()
{
	return defaultFont;
}

sfVector2f vector2f(float _x, float _y)
{
	sfVector2f tmp = { _x, _y };
	return tmp;
}

int getFrameX(sfIntRect _rect)
{
	if (_rect.width <= 0)
		return 0;
	return _rect.left / _rect.width;
}

int getFrameY(sfIntRect _rect)
{
	if (_rect.height <= 0)
		return 0;
	return _rect.top / _rect.height;
}

void Animator(sfIntRect* _rect, float* _timer, int _maxFrameX, int _maxFrameY, float _maxTimer, float _resetTimer)
{
	if (*_timer >= _maxTimer) {
		_rect->left += _rect->width;
		if (_rect->left >= _rect->width * _maxFrameX) {
			_rect->left = 0;
			//_rect->top += _rect->height;
			//if (_rect->height >= _rect->top * _maxFrameY) {
			//	_rect->top = 0;
			//}
		}
		*_timer = _resetTimer;
	}
}

sfFloatRect FlRect(float _left, float _top, float _width, float _height)
{
	sfFloatRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

sfIntRect IntRect(int _left, int _top, int _width, int _height)
{
	sfIntRect rectTmp = { _left, _top,  _width, _height };
	return rectTmp;
}

sfColor color(sfUint8 r, sfUint8 g, sfUint8 b, sfUint8 a)
{
	return sfColor_fromRGBA(r, g, b, a);
}

sfColor color_lerp(sfColor v0, sfColor v1, float t)
{
	sfColor tmp;
	tmp.r = (1 - t) * v0.r + t * v1.r;
	tmp.g = (1 - t) * v0.g + t * v1.g;
	tmp.b = (1 - t) * v0.b + t * v1.b;
	tmp.a = (1 - t) * v0.a + t * v1.a;
	return tmp;
}

void restartClock()
{
	sftime = sfClock_restart(sfclock);
}

float getDeltaTime()
{
	float tmp = sfTime_asSeconds(sftime);
	if (tmp > 0.1) tmp = 0.0f;
	return tmp * timeScale;
}

float getUnscaledDeltaTime()
{
	float tmp = sfTime_asSeconds(sftime);
	if (tmp > 0.1) tmp = 0.0f;
	return tmp;
}

sfSprite* CreateSprite(char* _path)
{
	sfTexture* tmp = sfTexture_createFromFile(_path, NULL);
	sfSprite* spTmp = sfSprite_create();
	sfSprite_setTexture(spTmp, tmp, sfTrue);
	return spTmp;
}

sfText* CreateText(char* _path)
{
	sfFont* tmp = sfFont_createFromFile(_path);
	sfText* txtTmp = sfText_create();
	sfText_setFont(txtTmp, tmp);
	return txtTmp;
}

float FrameRate()
{
	return 1.0f / getDeltaTime();
}

void displayFrameRate(sfRenderWindow* _window, sfText* _text)
{
	static float displayTimer = 0.0f;
	static char frameRate[25];

	displayTimer += getDeltaTime();

	if (displayTimer > 0.1f)
	{
		sprintf_s(frameRate, 25, "FPS : %.2f", FrameRate());
		displayTimer = 0.0f;
	}

	sfText_setString(_text, frameRate);
	sfRenderWindow_drawText(_window, _text, NULL);
}

void screenshot(sfRenderWindow* _window)
{
	char hours[9] = { 0 }, date[9] = { 0 }, tmph[7] = { 0 }, tmpd[7] = { 0 };
	_strtime(hours);
	_strdate(date);
	sfVector2u windowSize;


	windowSize = sfRenderWindow_getSize(_window);
	sfTexture* texture = sfTexture_create(windowSize.x, windowSize.y);
	sfTexture_updateFromRenderWindow(texture, _window, 0, 0);
	sfImage* screenshot = sfTexture_copyToImage(texture);
	char filename[100];
	int j = 0;
	for (int i = 0; i < 9; i++) if (hours[i] != ':') tmph[j++] = hours[i];
	j = 0;
	for (int i = 0; i < 9; i++) if (date[i] != '/') tmpd[j++] = date[i];

	sprintf(filename, "../Ressources/Screenshots/Screenshot-%s-%s.jpg", tmpd, tmph);
	sfImage_saveToFile(screenshot, filename);
}