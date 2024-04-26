#include "textureManager.h"

void Texture_Onload(State _state)
{
	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->type == SINGLE)
		{
			if (tempRessources->state == _state)
			{
				Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
				strcpy(tempTexture->name, tempRessources->name);
				tempTexture->type = tempRessources->type;
				tempTexture->state = tempRessources->state;

				tempTexture->sftexture = sfTexture_createFromFile(tempRessources->path, NULL);
				tempTexture->compteur = 0;
				AddTexture(tempTexture);
			}
		}
		if (tempRessources->type == SPRITESHEET)
		{
			if (tempRessources->state == _state)
			{
				FILE* file;
				char tmpligne[200];
				char tmp[50];
				char tmpname[20], tmppath[50];
				sfIntRect tmprect;
				StateTexture statetex = TEXNULL;
				sfTexture* tmptexture = NULL;
				sfBool firstPass = sfTrue;

				if (!(file = fopen(tempRessources->path, "r")))
				{
					printf("Unable to access file : %s", tempRessources->path);
					exit(1);
				}
				while (fgets(tmpligne, 200, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
				{
					sscanf(tmpligne, "%s ", tmp);

					if ((strcmp(tmp, "[end]")) == 0)
					{
						statetex = TEXNULL;
					}
					if (statetex == TEXNULL)
					{
						if ((strcmp(tmp, "[path]")) == 0)
						{
							statetex = PATH;
						}
						else if ((strcmp(tmp, "[config]")) == 0)
						{
							statetex = CONFIG;
						}
					}
					else if (statetex == PATH)
					{
						sscanf(tmpligne, "%s ", tmppath);
					}
					else if (statetex == CONFIG)
					{
						sscanf(tmpligne, "%s %d %d %d %d", tmpname, &tmprect.left, &tmprect.top, &tmprect.width, &tmprect.height);

						Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
						strcpy(tempTexture->name, tmpname);
						tempTexture->spSheetRec = tmprect;
						tempTexture->type = tempRessources->type;
						tempTexture->state = tempRessources->state;
						if (firstPass)
						{
							if (!(tempTexture->sftexture = sfTexture_createFromFile(tmppath, NULL)))
								exit(1);
							firstPass = sfFalse;
							tmptexture = tempTexture->sftexture;
						}
						else
						{
							tempTexture->sftexture = tmptexture;
						}
						tempTexture->compteur = 0;
						AddTexture(tempTexture);
					}

				}
				firstPass = sfTrue;
				fclose(file);
			}
		}
		if (tempRessources->type == ANIMATION)
		{
			if (tempRessources->state == _state)
			{
				FILE* file;
				char tmpligne[200];
				char tmp[50];
				char tmpname[20], tmppath[50];
				sfIntRect tmprect;
				StateTexture statetex = TEXNULL;
				sfTexture* tmptexture = NULL;
				sfBool firstPass = sfTrue;
				float tmptime;
				int tmpNbFrame, tmpRow;

				if (!(file = fopen(tempRessources->path, "r")))
				{
					printf("Unable to access file : %s", tempRessources->path);
					exit(1);
				}
				while (fgets(tmpligne, 200, file) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
				{
					sscanf(tmpligne, "%s ", tmp);

					if ((strcmp(tmp, "[end]")) == 0)
					{
						statetex = TEXNULL;
					}
					if (statetex == TEXNULL)
					{
						if ((strcmp(tmp, "[path]")) == 0)
						{
							statetex = PATH;
						}
						else if ((strcmp(tmp, "[config]")) == 0)
						{
							statetex = CONFIG;
						}
					}
					else if (statetex == PATH)
					{
						sscanf(tmpligne, "%s ", tmppath);
					}
					else if (statetex == CONFIG)
					{
						sscanf(tmpligne, "%s %d %d %d %d %d %f %d", tmpname, &tmprect.left, &tmprect.top, &tmprect.width, &tmprect.height, &tmpNbFrame, &tmptime, &tmpRow);

						Texture* tempTexture = (Texture*)calloc(1, sizeof(Texture));
						strcpy(tempTexture->name, tmpname);
						tempTexture->spSheetRec = tmprect;
						tempTexture->type = tempRessources->type;
						tempTexture->state = tempRessources->state;
						if (firstPass)
						{
							if (!(tempTexture->sftexture = sfTexture_createFromFile(tmppath, NULL)))
								exit(1);
							firstPass = sfFalse;
							tmptexture = tempTexture->sftexture;
						}
						else
						{
							tempTexture->sftexture = tmptexture;
						}
						tempTexture->compteur = 0;
						tempTexture->NbFrame = tmpNbFrame;
						tempTexture->frameTime = tmptime;
						tempTexture->row = tmpRow;
						AddTexture(tempTexture);
					}
				}
				firstPass = sfTrue;
				fclose(file);
			}
		}
		tempRessources = tempRessources->pNext;
	}
}


sfTexture* GetTexture(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture->sftexture;
		}
		tempTexture = tempTexture->pNext;
	}
	return NULL;
}

int GetNbFrame(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture->NbFrame;
		}
		tempTexture = tempTexture->pNext;
	}
	return 0;
}

float GetFrameTime(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture->frameTime;
		}
		tempTexture = tempTexture->pNext;
	}
	return 0.0f;
}

sfIntRect GetRect(char* _name)
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if ((strcmp(_name, tempTexture->name)) == 0)
		{
			return tempTexture->spSheetRec;
		}
		tempTexture = tempTexture->pNext;
	}
	return IntRect(0, 0, 0, 0);
}

void AddTexture(Texture* _texture)
{
	_texture->pNext = textureBegin;
	textureBegin = _texture;
}

Texture* RemoveTexture(Texture* _texture)
{
	if (_texture == textureBegin)
	{
		Texture* tempTexture = _texture->pNext;
		textureBegin = tempTexture;
		//sfTexture_destroy(tempTexture->sftexture);
		sfTexture_destroy(_texture->sftexture);
		free(_texture);
		return tempTexture;
	}
	else
	{
		Texture* tempTexture = textureBegin;
		while (tempTexture->pNext != _texture)
		{
			tempTexture = tempTexture->pNext;
		}
		tempTexture->pNext = _texture->pNext;
		//sfTexture_destroy(tempTexture->sftexture);
		sfTexture_destroy(_texture->sftexture);
		free(_texture);
		return tempTexture->pNext;
	}
}

// Enléve toutes les textures de la chaine
void RemoveAllTexture()
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		Texture* todelete = tempTexture;
		tempTexture = tempTexture->pNext;
		sfTexture_destroy(tempTexture->sftexture);
		free(todelete);
	}
	textureBegin = 0;
}

// Enléve toutes les textures de la chaine sauf celles qui sont indispensables dans tous les states
void RemoveAllTextureButALL()
{
	Texture* tempTexture = textureBegin;
	while (tempTexture != NULL)
	{
		if (tempTexture->state != ALL)
		{
			if (tempTexture == textureBegin)
			{
				Texture* tempTexture2 = tempTexture->pNext;
				textureBegin = tempTexture2;
				sfTexture_destroy(tempTexture->sftexture);
				free(tempTexture);
				tempTexture = tempTexture2;
			}
			else
			{
				Texture* toDelete = tempTexture;
				tempTexture = tempTexture->pNext;
				sfTexture_destroy(toDelete->sftexture);
				free(toDelete);
			}
		}
		else
		{
			tempTexture = tempTexture->pNext;
		}
	}
}