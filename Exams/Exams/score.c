#include "score.h"
#include "List.h"
#include "viewManager.h"
#include "CustomMath.h"
#include "map.h"

#define GD_SCORE STD_LIST_GETDATA(scoreList, Score, i)

typedef struct Score {
	int scoreValue;
	sfVector2f pos;
	float timer;
}Score;

sfText* scoreText;

stdList* scoreList;

char buffer[30];

void addScore(int _scoreValue, sfVector2f _pos, float _timer)
{
	Score tmp;
	tmp.scoreValue = _scoreValue;
	tmp.pos = _pos;
	tmp.timer = _timer;

	STD_LIST_PUSHBACK(scoreList, tmp);
}

void initScore()
{
	scoreText = sfText_create();
	sfText_setFont(scoreText, getDefaultFont());
	sfText_setCharacterSize(scoreText, 20);
	sfText_setFillColor(scoreText, sfWhite);

	scoreList = STD_LIST_CREATE(Score, 0);
}

void createScore(int _score, sfVector2f _pos)
{
	sfVector2f pos = AddVectors(_pos, vector2f(-GetViewPosition(mainView).x + 960.f, -BLOCK_SCALE * BLOCK_SIZE));
	addScore(_score, pos, 0.f);
}

void updateScore(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < scoreList->size(scoreList); i++)
	{
		GD_SCORE->timer += dt;

		GD_SCORE->pos.y -= 200.f * dt;

		if (GD_SCORE->timer >= 0.8f) {
			scoreList->erase(&scoreList, i);
			continue;
		}
	}
}

void displayScore(Window* _window)
{
	sfRenderTexture_setView(_window->renderTexture, sfRenderTexture_getDefaultView(_window->renderTexture));

	for (int i = 0; i < scoreList->size(scoreList); i++)
	{
		sprintf(buffer, "%d", GD_SCORE->scoreValue);
		sfText_setString(scoreText, buffer);
		sfText_setPosition(scoreText, GD_SCORE->pos);
		sfFloatRect tmpRect;
		tmpRect = sfText_getLocalBounds(scoreText);
		sfText_setOrigin(scoreText, TEXT_ORIGIN);
		sfRenderTexture_drawSprite(_window->renderTexture, scoreText, NULL);
	}

	sfRenderTexture_setView(_window->renderTexture, mainView->view);
}

void eraseAllScore()
{
	scoreList->clear(&scoreList);
}
