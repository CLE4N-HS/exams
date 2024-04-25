#include "player.h"
#include "textureManager.h"
#include "map.h"
#include "CustomMath.h"
#include "gamepadx.h"
#include "fireballs.h"
#include "viewManager.h"

#define PLAYER_SPEED 1400.f
#define MAX_PLAYER_SPEED 400.f

#define MIN_WALK 4.453125f
#define MAX_WALK 93.75f
#define MAX_RUN 153.75f
#define ACC_WALK 133.59375f
#define ACC_RUN 200.390625f
#define DEC_REL /*182.8125f*/ 1182.8125f
#define DEC_SKID 365.625f
#define MIN_SKID 33.75f

#define STOP_FALL 1575.f
#define WALK_FALL 1800.f
#define RUN_FALL 2025.f
#define STOP_FALL_A /*450*/ 450.f * 0.8f
#define WALK_FALL_A /*421.875f*/ 421.875f * 0.8f
#define RUN_FALL_A /*562.5f*/ 562.5f * 0.8f

#define MAX_FALL 270.f
#define MAX_JUMP 270.f


#define STAR_TIMER_DURATION 10.f
#define FINISH_XPOS 14259.f

typedef enum playerState {
	P_IDLE,
	P_WALK,
	P_RUN,
	P_SKID,
	P_JUMP
}playerState;

typedef struct Player {
	sfVector2f pos;
	sfVector2f origin;
	sfVector2f scale;
	sfIntRect rect;
	sfFloatRect bounds;
	sfVector2f velocity;
	float releaseTimer;
	playerState state;
	float fallAcc;
	sfBool canJump;
	sfVector2f nextPos;
	playerPower power;
	float invincibilityTimer;
	float firethrowerTimer;
	sfBool canGoInYPipe;
	sfBool canGoInXPipe;
	sfBool wasReJumping;
	sfColor color;
	float starTimer;
	sfBool isAtFlag;
	int finishState;
	float deathTimer;
	int nbCoins;
}Player;
Player p[2];

sfSprite* playerSprite;
sfCircleShape* cr;
sfRectangleShape* rec;

sfVector2f greatestViewPos;

void initPlayer()
{
	playerSprite = sfSprite_create();
	sfSprite_setTexture(playerSprite, GetTexture("mario"), sfFalse);

	cr = sfCircleShape_create();
	sfCircleShape_setRadius(cr, 2.f);
	sfCircleShape_setFillColor(cr, sfBlue);

	rec = sfRectangleShape_create();

	for (int i = 0; i < 2; i++)
	{
		p[i].rect = IntRect(0, 32, 16, 16);
		p[i].origin = vector2f(8.f, 16.f);
		p[i].scale = vector2f(BLOCK_SCALE, BLOCK_SCALE);
		p[i].pos = vector2f(3.f * BLOCK_SCALE * BLOCK_SIZE, 13.f * BLOCK_SCALE * BLOCK_SIZE);
		p[i].velocity = VECTOR2F_NULL;
		p[i].releaseTimer = 0.f;
		p[i].state = P_IDLE;
		p[i].fallAcc = 562.5f;
		p[i].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
		p[i].canJump = sfFalse;
		p[i].nextPos = p[i].pos;
		p[i].power = P_SMALL;
		p[i].invincibilityTimer = 0.f;
		p[i].firethrowerTimer = 1.f;
		p[i].canGoInYPipe = sfFalse;
		p[i].canGoInXPipe = sfFalse;
		p[i].wasReJumping = sfFalse;
		p[i].color = color(255, 255, 255, 255);
		p[i].starTimer = 0.f;
		p[i].isAtFlag = sfFalse;
		p[i].finishState = 0;
		p[i].deathTimer = 0.f;
		p[i].nbCoins = 0;

	}

	greatestViewPos = vector2f(960.f, 540.f);
}

void updatePlayer(Window* _window)
{
	float dt = getDeltaTime();

	sfVector2f firstPlayerPos = vector2f(960.f, 540.f);

	int nbPlayersOnFlag = 0;
	int nbPlayersOnFlagNeeded = 0; // TODO nbplayer dynamic

	for (int i = 0; i < 1; i++)
	{
		float lStickXPos = getStickPos(i, sfTrue, sfTrue);
		float lStickYPos = getStickPos(i, sfTrue, sfFalse);

		nbPlayersOnFlagNeeded++;

		if (!p[i].isAtFlag) {

			if (p[i].power <= P_DEAD) {
				p[i].deathTimer += dt;
				p[i].rect.left = 96;
				if (p[i].pos.y < 1300.f) {
					if (p[i].deathTimer < 0.5f) {

					}
					else if (p[i].deathTimer < 1.f) {
						p[i].pos.y -= sinf(p[i].deathTimer) * dt * 800.f;
					}
					else {
						p[i].pos.y += p[i].deathTimer /** p[i].deathTimer*/ * dt * 300.f;
					}
				}
				else {
					p[i].deathTimer = 0.f;
					p[i].invincibilityTimer = 0.f;

					p[i].power = P_SMALL;
					p[i].pos.y = 100.f;
				}
				continue;
			}

			playerState lastState = p[i].state;

			p[i].canGoInYPipe = sfFalse;
			p[i].canGoInXPipe = sfFalse;

			if (!isButtonPressed(i, A) || isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds))
				p[i].canJump = sfTrue;
			else
				//p[i].canJump = sfFalse;

			//if (isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
				if (isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity, i)) {
					p[i].state = P_IDLE;
					p[i].wasReJumping = sfFalse;
				}

			if (p[i].state != P_JUMP) { // not jumping
				// ground physics
				if (fabsf(p[i].velocity.x) < MIN_WALK) {  // slower than a walk // starting, stopping or turning around
					p[i].velocity.x = 0;
					p[i].state = P_IDLE;
					if (lStickXPos < 0.f/* && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-MIN_WALK, 0.f), i)) {
						p[i].scale.x = -BLOCK_SCALE;
						p[i].velocity.x -= MIN_WALK;
					}
					if (lStickXPos > 0.f /*&& !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfFalse, vector2f(MIN_WALK, 0.f), i)) {
						p[i].scale.x = BLOCK_SCALE;
						p[i].velocity.x += MIN_WALK;
					}
				}
				else if (fabsf(p[i].velocity.x) >= MIN_WALK) {  // faster than a walk // accelerating or decelerating
					//if (1) {
					if (lStickXPos >= 0.f) {
						if (lStickXPos > 0.f/* && !p[i].game.left && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(ACC_RUN * dt, 0.f), i)) {
							p[i].scale.x = BLOCK_SCALE;
							if (isButtonPressed(i, B)) {
								p[i].velocity.x += ACC_RUN * dt;
							}
							else {
								p[i].velocity.x += ACC_WALK * dt;
							}
						}
						else if (lStickXPos < 0.f/* && !p[i].game.right && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-DEC_SKID * dt, 0.f), i)) {
							p[i].scale.x = -BLOCK_SCALE;
							p[i].velocity.x -= DEC_SKID * dt;
							p[i].state = P_SKID;
						}
						else {
							if (p[i].velocity.x > 0.f)
								p[i].velocity.x -= DEC_REL * dt;
							else
								p[i].velocity.x += DEC_REL * dt;

						}
					}
					//if (0) {
					else {
						if (lStickXPos < 0.f/* && !p[i].game.right && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-ACC_RUN * dt, 0.f), i)) {
							p[i].scale.x = -BLOCK_SCALE;
							if (isButtonPressed(i, B)) {
								p[i].velocity.x -= ACC_RUN * dt;
							}
							else {
								p[i].velocity.x -= ACC_WALK * dt;
							}

						}
						else if (lStickXPos > 0.f/* && !p[i].game.left && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfFalse, vector2f(DEC_SKID * dt, 0.f), i)) {
							p[i].scale.x = BLOCK_SCALE;
							p[i].velocity.x += DEC_SKID * dt;
							p[i].state = P_SKID;
						}
						else {
							p[i].velocity.x += DEC_REL * dt;
						}
					}
				}

				p[i].velocity.y += p[i].fallAcc * dt;

				if (p[i].canJump && isButtonPressed(i, A) && isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) { // jump
					//if (p[i].canJump && isButtonPressed(i, A) && isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity, i)) { // jump
					if (fabsf(p[i].velocity.x) < 16.f) {
						p[i].velocity.y = -240.f;
						p[i].fallAcc = STOP_FALL;
					}
					else if (fabsf(p[i].velocity.x) < 40.f) {
						p[i].velocity.y = -240.f;
						p[i].fallAcc = WALK_FALL;
					}
					else {
						p[i].velocity.y = -300.f;
						p[i].fallAcc = RUN_FALL;
					}
					p[i].state = P_JUMP;
					p[i].canJump = sfFalse;
				}



			}
			else {
				// air physics
				// vertical physics
				if (p[i].velocity.y < 0 && isButtonPressed(i, A)) { // holding A while jumping jumps higher
					if (p[i].fallAcc == STOP_FALL) p[i].velocity.y -= (STOP_FALL - STOP_FALL_A) * dt;
					if (p[i].fallAcc == WALK_FALL) p[i].velocity.y -= (WALK_FALL - WALK_FALL_A) * dt;
					if (p[i].fallAcc == RUN_FALL) p[i].velocity.y -= (RUN_FALL - RUN_FALL_A) * dt;
				}

				// horizontal physics
				if (lStickXPos > 0.f/* && !p[i].game.left*/ && !isCollision2(p[i].bounds, sfTrue, sfFalse, vector2f(ACC_RUN * dt, 0.f), i)) {
					p[i].scale.x = BLOCK_SCALE;
					if (fabsf(p[i].velocity.x) > MAX_WALK) {
						p[i].velocity.x += ACC_RUN * dt;
					}
					else p[i].velocity.x += ACC_WALK * dt;
				}
				else if (lStickXPos < 0.f/* && !p[i].game.right*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-ACC_RUN * dt, 0.f), i)) {
					p[i].scale.x = -BLOCK_SCALE;
					if (fabsf(p[i].velocity.x) > MAX_WALK) {
						p[i].velocity.x -= ACC_RUN * dt;
					}
					else p[i].velocity.x -= ACC_WALK * dt;
				}
				else {
					// do nothing
				}
			}

			p[i].velocity.y += p[i].fallAcc * dt;

			if (p[i].state == P_SKID)
				lastState = P_SKID;

			//if (isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity))
			if (isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
				p[i].state = P_IDLE;
			}

			if (isCollision3(p[i].bounds, &p[i].velocity, i)) {}

			// max speed calculation
			if (p[i].velocity.y >= MAX_FALL) p[i].velocity.y = MAX_FALL;
			if (p[i].velocity.y <= -MAX_FALL && !p[i].wasReJumping) p[i].velocity.y = -MAX_FALL;
			if (p[i].velocity.y <= -MAX_JUMP) p[i].velocity.y = -MAX_JUMP;

			if (p[i].velocity.x >= MAX_RUN) p[i].velocity.x = MAX_RUN;
			if (p[i].velocity.x <= -MAX_RUN) p[i].velocity.x = -MAX_RUN;
			if (p[i].velocity.x >= MAX_WALK && !isButtonPressed(i, B)) p[i].velocity.x = MAX_WALK;
			if (p[i].velocity.x <= -MAX_WALK && !isButtonPressed(i, B)) p[i].velocity.x = -MAX_WALK;


			//p[i].velocity.x = MAX(p[i].velocity.x, -MAX_PLAYER_SPEED);
			//p[i].velocity.x = MIN(p[i].velocity.x, MAX_PLAYER_SPEED);

			if (isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity, i)) {
				p[i].velocity.y = -1.f;
			}


			p[i].pos = AddVectors(p[i].pos, MultiplyVector(p[i].velocity, BLOCK_SCALE * dt));

			if (p[i].pos.x + p[i].velocity.x * BLOCK_SCALE * dt < greatestViewPos.x - 960.f + p[i].origin.x * fabsf(p[i].scale.x)) {
				p[i].pos.x = greatestViewPos.x - 960.f + p[i].origin.x * fabsf(p[i].scale.x);
				p[i].velocity.x = 0.f;
			}



			// Fireballs
			if (p[i].power == P_FIRETHROWER) {
				p[i].firethrowerTimer += dt;
				if (isButtonPressed(i, B) && p[i].firethrowerTimer > 0.4f && fabsf(p[i].velocity.x) < 10.f/* && getNbFireballs() < 2*/) { // if debug
					p[i].firethrowerTimer = 0.f;
					sfBool isLeftSide = sfFalse;
					if (p[i].scale.x < 0.f)
						isLeftSide = sfTrue;
					createFireball(p[i].pos, isLeftSide);

				}
			}

			// Star
			p[i].starTimer -= dt;
			// Invincibility
			p[i].invincibilityTimer -= dt;

			if (p[i].starTimer > 0.f) {
				p[i].color.r = (sfUint8)(fabsf(sinf(p[i].starTimer * 5.f)) * 127.f + 127.f);
				p[i].color.g = (sfUint8)(fabsf(sinf(p[i].starTimer * 5.f)) * 127.f + 127.f);
				p[i].color.b = 0;
			}
			else if (p[i].invincibilityTimer > 0.f) {
				p[i].color = color(255, 255, 255, (sfUint8)(fabsf(sinf(p[i].invincibilityTimer * 5.f)) * 127.f + 127.f));
			}
			else {
				p[i].color = color(255, 255, 255, 255);
			}

			// View Setup
			if (nbMap != 2 && p[i].pos.x > firstPlayerPos.x)
				firstPlayerPos.x = p[i].pos.x;

			// Pipes
			if (p[i].canGoInYPipe && lStickYPos < -50.f && p[i].pos.x >= 3800.f && p[i].pos.x <= 4500.f) {
				p[i].canGoInYPipe = sfFalse;
				nbMap = 2;
				loadMap(2);
				for (int j = 0; j < 2; j++)
				{
					p[j].pos = vector2f(2.f * BLOCK_SCALE * BLOCK_SIZE + p[i].origin.x * BLOCK_SCALE, 3.f * BLOCK_SCALE * BLOCK_SIZE);
					p[j].velocity = VECTOR2F_NULL;
				}
				greatestViewPos = vector2f(8.5f * BLOCK_SCALE * BLOCK_SIZE, 540.f);
			}
			if (p[i].canGoInXPipe && isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds) && lStickXPos > 50.f) {
				p[i].canGoInYPipe = sfFalse;
				nbMap = 1;
				loadMap(1);
				for (int j = 0; j < 2; j++)
				{
					p[j].pos = vector2f(163.5f * BLOCK_SCALE * BLOCK_SIZE + p[i].origin.x * BLOCK_SCALE, 11.f * BLOCK_SCALE * BLOCK_SIZE);
					p[j].velocity = VECTOR2F_NULL;
				}
				greatestViewPos = vector2f(168.5f * BLOCK_SCALE * BLOCK_SIZE, 540.f);
			}


			if (fabsf(p[i].velocity.x) > ACC_WALK) {

				p[i].rect.left = 48;
			}
			else if (fabsf(p[i].velocity.x) > 1.f) {
				p[i].rect.left = 16;
			}
			else {
				p[i].rect.left = 0;
			}

			if ((lStickXPos > 0.f && p[i].velocity.x < 0.f) || (lStickXPos < 0.f && p[i].velocity.x > 0.f)) {
				p[i].rect.left = 64;
			}

			if (p[i].power == P_FIRETHROWER && p[i].firethrowerTimer < 0.2f) {
				p[i].rect = IntRect(256, 53, 16, 32);
			}

			if (sfKeyboard_isKeyPressed(sfKeyP)) {
				p[i].pos.x = FINISH_XPOS - 400.f;
			}

			if (p[i].pos.x >= FINISH_XPOS) {
				p[i].isAtFlag = sfTrue;
			}
		}
		else {
			nbPlayersOnFlag++;

			p[i].rect.left = 128;

			if (p[i].finishState < 1) {
				p[i].pos.x = 14278.f;
				p[i].pos.y = MAX(p[i].pos.y, 3.f * BLOCK_SCALE * BLOCK_SIZE);
			}
		}

		// Finish anim
		if (nbPlayersOnFlag >= nbPlayersOnFlagNeeded) {
			if (p[i].finishState == 0) {
				p[i].velocity.y = 100.f;
				if (!isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
					p[i].pos.y += dt * p[i].velocity.y;
				}
				else {
					p[i].finishState = 1;
				}
			}
			else if (p[i].finishState == 1) {
				p[i].rect.left = 16;
				p[i].velocity = vector2f(100.f, 270.f);
				if (!isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
					p[i].pos.y += p[i].velocity.y * dt;
				}
				p[i].pos.x += p[i].velocity.x * dt;

				if (p[i].pos.x > 14725.f) {
					p[i].finishState = 2;
				}
			}
			else if (p[i].finishState == 2) {
				p[i].color.a = 0;
			}
		}

	}


	// View
	if (nbMap == 2) { // TODO maxPos
		SetViewPosition(mainView, vector2f(8.5f * BLOCK_SCALE * BLOCK_SIZE, 540.f));
	}
	else {
		if (firstPlayerPos.x > greatestViewPos.x) {
			greatestViewPos.x = firstPlayerPos.x;
		}
		if (greatestViewPos.x > 14293.f) {
			greatestViewPos.x = 14293.f;
		}
		SetViewPosition(mainView, greatestViewPos);
	}
}

void displayPlayer(Window* _window)
{
	for (int i = 0; i < 2; i++)
	{
		sfSprite_setTextureRect(playerSprite, p[i].rect);
		sfSprite_setOrigin(playerSprite, p[i].origin);
		sfSprite_setPosition(playerSprite, p[i].pos);
		sfSprite_setScale(playerSprite, p[i].scale);
		sfSprite_setColor(playerSprite, p[i].color);
		sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

		if (p[i].power <= P_DEAD) p[i].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
		else p[i].bounds = sfSprite_getGlobalBounds(playerSprite);



		//sfCircleShape_setPosition(cr, p[i].pos);
		//sfRenderTexture_drawCircleShape(_window->renderTexture, cr, NULL);
		//
		//sfRectangleShape_setFillColor(rec, color(255, 0, 0, 100));
		//sfRectangleShape_setPosition(rec, vector2f(p[i].bounds.left, p[i].bounds.top));
		//sfRectangleShape_setSize(rec, vector2f(p[i].bounds.width, p[i].bounds.height));
		////sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
		//
		//sfRectangleShape_setFillColor(rec, color(0, 255, 0, 100));
		//sfRectangleShape_setPosition(rec, vector2f(tmpRect.left, tmpRect.top));
		//sfRectangleShape_setSize(rec, vector2f(tmpRect.width, tmpRect.height));
		//sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
		//
		//sfRectangleShape_setFillColor(rec, color(0, 255, 255, 100));
		//sfRectangleShape_setPosition(rec, vector2f(tmpRect2.left, tmpRect2.top));
		//sfRectangleShape_setSize(rec, vector2f(tmpRect2.width, tmpRect2.height));
		//sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
		//
		//sfRectangleShape_setFillColor(rec, color(255, 255, 255, 100));
		//sfRectangleShape_setPosition(rec, vector2f(tmpRect3.left, tmpRect3.top));
		//sfRectangleShape_setSize(rec, vector2f(tmpRect3.width, tmpRect3.height));
		//sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
		//
		//sfRectangleShape_setFillColor(rec, color(0, 255, 255, 100));
		//sfRectangleShape_setPosition(rec, vector2f(tmpRect4.left, tmpRect4.top));
		//sfRectangleShape_setSize(rec, vector2f(tmpRect4.width, tmpRect4.height));
		//sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
		//
		//sfRectangleShape_setFillColor(rec, color(255, 255, 255, 100));
		//sfRectangleShape_setPosition(rec, vector2f(tmpRect5.left, tmpRect5.top));
		//sfRectangleShape_setSize(rec, vector2f(tmpRect5.width, tmpRect5.height));
		//sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
	}
}

sfFloatRect* pgetPlayerBounds(int _id)
{
	return &p[_id].bounds;
}

void changePower(int _id)
{
	switch (p[_id].power)
	{
	case P_SMALL:
		p[_id].rect = IntRect(0, 32, 16, 16);
		p[_id].origin = vector2f(8.f, 16.f);
		break;
	case P_BIG:
		p[_id].rect = IntRect(0, 0, 16, 32);
		p[_id].origin = vector2f(8.f, 32.f);
		break;
	case P_FIRETHROWER:
		p[_id].rect = IntRect(0, 53, 16, 32);
		p[_id].origin = vector2f(8.f, 32.f);
		break;
	default:
		break;
	}
}

void setPlayerPower(int _id, playerPower _power)
{
	printf("%d\n", p[_id].power);
	if (_power <= p[_id].power)
		return;
	if (p[_id].power >= P_FIRETHROWER)
		return;

	//if (p[_id].power == P_SMALL) {
	//	p[_id].power = P_BIG;
	//}
	//if (p[_id].power == P_BIG) {
	//	p[_id].power = P_FIRETHROWER
	//}


	//p[_id].power = _power;
	p[_id].power++;

	changePower(_id);
}

void DamagePlayer(int _id)
{
	if (p[_id].invincibilityTimer > 0.f)
		return;

	p[_id].invincibilityTimer = 3.f;

	if (p[_id].power > P_DEAD) {
		p[_id].power--;
		p[_id].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	}
	//else {
		changePower(_id);
	//}
}

sfVector2f getGreatestViewPos()
{
	return greatestViewPos;
}

sfBool isOffView(sfVector2f _pos)
{
	// pas mal
	return sfFalse;
}

void setPlayerPossiblePipe(int _id, sfBool _canEnter, sfBool _YPipe)
{
	if (_YPipe)
		p[_id].canGoInYPipe = _canEnter;
	else
		p[_id].canGoInXPipe = _canEnter;
}

sfBool isPlayerBig(int _id)
{
	if (p[_id].power >= P_BIG)
		return sfTrue;

	return sfFalse;
}

sfVector2f getPlayerPos(int _id)
{
	return p[_id].pos;
}

void setPlayerPos(int _id, sfVector2f _pos)
{
	p[_id].pos = _pos;
}

void setPlayerVelocity(int _id, float _x, float _y)
{
	p[_id].velocity = vector2f(_x, _x);
}

void MakePlayerJump(int _id)
{
	p[_id].canJump = sfTrue;
	p[_id].state = P_IDLE;
	p[_id].velocity.y = -MAX_JUMP;
	p[_id].wasReJumping = sfTrue;
}

void setPlayerStarPower(int _id)
{
	p[_id].starTimer = STAR_TIMER_DURATION;
}

sfBool PlayerHasStar(int _id)
{
	if (p[_id].starTimer > 0.f)
		return sfTrue;

	return sfFalse;
}
