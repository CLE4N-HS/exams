#include "fontManager.h"

void Font_Onload(State _state)
{
	ressources* tmpRessources = ResBegin;
	while (tmpRessources != NULL)
	{
		if (tmpRessources->state == _state)
		{
			if (tmpRessources->type == FONT)
			{
				Font* tmpFont = (Font*)calloc(1, sizeof(Font));

				tmpFont->font = sfFont_createFromFile(tmpRessources->path);
				strcpy(tmpFont->name, tmpRessources->name);
				tmpFont->state = tmpRessources->state;
				tmpFont->type = tmpRessources->type;

				AddFont(tmpFont);

			}
		}

		tmpRessources = tmpRessources->pNext;
	}
}

void AddFont(Font* _font)
{
	_font->pNext = FontBegin;
	FontBegin = _font;
}

Font* RemoveFont(Font* _font)
{
	if (_font == FontBegin)
	{
		Font* tmpFont = _font->pNext;
		FontBegin = tmpFont;
		sfFont_destroy(_font->font);
		free(_font);
		return tmpFont;
	}
	else
	{
		Font* tmpFont = FontBegin;
		while (tmpFont->pNext != _font)
		{
			tmpFont = tmpFont->pNext;
		}
		tmpFont->pNext = _font->pNext;
		sfFont_destroy(_font->font);
		free(_font);
		return tmpFont->pNext;
	}
}

void RemoveAllFonts()
{
	Font* tmpFont = FontBegin;

	while (tmpFont != NULL)
	{
		sfFont_destroy(tmpFont->font);
		Font* toDelete = tmpFont;
		tmpFont = tmpFont->pNext;
		free(toDelete);
	}
	FontBegin = 0;
}

void RemoveAllFontsButALL()
{
	Font* tmpFont = FontBegin;
	while (tmpFont != NULL)
	{
		if (tmpFont->state != ALL)
		{
			if (tmpFont = FontBegin)
			{
				Font* tmpNextFont = tmpFont->pNext;
				FontBegin = tmpNextFont;
				sfFont_destroy(tmpFont->font);
				free(tmpFont);
				tmpFont = tmpNextFont;
			}
			else
			{
				Font* toDelete = tmpFont;
				//tmpFont = tmpFont->pNext;
				sfFont_destroy(tmpFont->font);
				tmpFont = tmpFont->pNext;
				free(toDelete);
			}
		}
		else
			tmpFont = tmpFont->pNext;
	}
}

sfFont* GetFont(char* _name)
{
	Font* tmpFont = FontBegin;

	while (tmpFont != NULL)
	{
		if (strcmp(tmpFont->name, _name) == 0)
			return tmpFont->font;

		tmpFont = tmpFont->pNext;
	}
	return getDefaultFont();
}