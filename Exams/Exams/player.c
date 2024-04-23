#include "player.h"
#include "textureManager.h"
#include "map.h"
#include "CustomMath.h"
#include "gamepadx.h"

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

#define STOP_FALL 1575
#define WALK_FALL 1800
#define RUN_FALL 2025
#define STOP_FALL_A /*450*/ 450.f * 0.8f
#define WALK_FALL_A /*421.875f*/ 421.875f * 0.8f
#define RUN_FALL_A /*562.5f*/ 562.5f * 0.8f

#define MAX_FALL 270

typedef enum playerState {
	P_IDLE,
	P_WAL,
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
}Player;
Player p[2];

sfSprite* playerSprite;
sfCircleShape* cr;
sfRectangleShape* rec;

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
		p[i].rect = IntRect(0, 32, 18, 16);
		p[i].origin = vector2f(9.f, 16.f);
		p[i].scale = vector2f(BLOCK_SCALE, BLOCK_SCALE);
		p[i].pos = vector2f(100.f, 200.f);
		p[i].velocity = VECTOR2F_NULL;
		p[i].releaseTimer = 0.f;
		p[i].state = P_IDLE;
		p[i].fallAcc = 562.5f;
		p[i].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
		p[i].canJump = sfFalse;
		p[i].nextPos = p[i].pos;
		p[i].power = P_SMALL;
		p[i].invincibilityTimer = 0.f;

	}
}

void updatePlayer(Window* _window)
{
	float dt = getDeltaTime();

	for (int i = 0; i < 1; i++)
	{
		float lStickXPos = getStickPos(i, sfTrue, sfTrue);

		//sfKeyCode keyLeft;
		//sfKeyCode keyRight;
		//if (i == 0) {
		//	keyLeft = sfKeyQ;
		//	keyRight = sfKeyD;
		//}
		//else {
		//	keyLeft = sfKeyLeft;
		//	keyRight = sfKeyRight;
		//}
		//
		//if (lStickXPos > 10.f || sfKeyboard_isKeyPressed(keyRight)) {
		//	p[i].velocity.x = MAX(p[i].velocity.x, 0.f);
		//	p[i].velocity.x += PLAYER_SPEED * dt;
		//	p[i].releaseTimer = 0.f;
		//	p[i].scale.x = BLOCK_SCALE;
		//}
		//else if (lStickXPos < -10.f || sfKeyboard_isKeyPressed(keyLeft)) {
		//	p[i].velocity.x = MIN(p[i].velocity.x, 0.f);
		//	p[i].velocity.x -= PLAYER_SPEED * dt;
		//	p[i].releaseTimer = 0.f;
		//	p[i].scale.x = -BLOCK_SCALE;
		//}
		//else {
		//	p[i].releaseTimer += dt;
		//	p[i].releaseTimer = MIN(p[i].releaseTimer, 1.f);
		//	p[i].velocity.x = Lerp(p[i].velocity.x, 0.f, p[i].releaseTimer);
		//}

		if (!isButtonPressed(i, A) || isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds))
			p[i].canJump = sfTrue;
		else
			//p[i].canJump = sfFalse;

		if (isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
		//if (isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity)) {
			p[i].state = P_IDLE;
		}

		if (p[i].state != P_JUMP) { // not jumping
			// ground physics
			if (fabsf(p[i].velocity.x) < MIN_WALK) {  // slower than a walk // starting, stopping or turning around
				p[i].velocity.x = 0;
				p[i].state = P_IDLE;
				if (lStickXPos < 0.f/* && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-MIN_WALK, 0.f))) {
					p[i].scale.x = -BLOCK_SCALE;
					p[i].velocity.x -= MIN_WALK;
				}
				if (lStickXPos > 0.f /*&& !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfFalse, vector2f(MIN_WALK, 0.f))) {
					p[i].scale.x = BLOCK_SCALE;
					p[i].velocity.x += MIN_WALK;
				}
			}
			else if (fabsf(p[i].velocity.x) >= MIN_WALK) {  // faster than a walk // accelerating or decelerating
				if (1) {
					if (lStickXPos > 0.f/* && !p[i].game.left && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(ACC_RUN * dt, 0.f))) {
						p[i].scale.x = BLOCK_SCALE;
						if (isButtonPressed(i, B)) {
							p[i].velocity.x += ACC_RUN * dt;
						}
						else {
							p[i].velocity.x += ACC_WALK * dt;
						}
					}
					else if (lStickXPos < 0.f/* && !p[i].game.right && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-DEC_SKID * dt, 0.f))) {
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
				if (0) {
					if (lStickXPos < 0.f/* && !p[i].game.right && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-ACC_RUN * dt, 0.f))) {
						p[i].scale.x = -BLOCK_SCALE;
						if (isButtonPressed(i, B)) {
							p[i].velocity.x -= ACC_RUN * dt;
						}
						else {
							p[i].velocity.x -= ACC_WALK * dt;
						}
						
					}
					else if (lStickXPos > 0.f/* && !p[i].game.left && !p[i].game.down*/ && !isCollision2(p[i].bounds, sfTrue, sfFalse, vector2f(DEC_SKID * dt, 0.f))) {
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
			//if (p[i].canJump && isButtonPressed(i, A) && isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity)) { // jump
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
			if (lStickXPos > 0.f/* && !p[i].game.left*/ && !isCollision2(p[i].bounds, sfTrue, sfFalse, vector2f(ACC_RUN * dt, 0.f))) {
				p[i].scale.x = BLOCK_SCALE;
				if (fabsf(p[i].velocity.x) > MAX_WALK) {
					p[i].velocity.x += ACC_RUN * dt;
				}
				else p[i].velocity.x += ACC_WALK * dt;
			}
			else if (lStickXPos < 0.f/* && !p[i].game.right*/ && !isCollision2(p[i].bounds, sfTrue, sfTrue, vector2f(-ACC_RUN * dt, 0.f))) {
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

		if (isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds))
		//if (isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity))
			p[i].state = P_IDLE;

		if (isCollision3(p[i].bounds, &p[i].velocity))
			printf("3");

		// max speed calculation
		if (p[i].velocity.y >= MAX_FALL) p[i].velocity.y = MAX_FALL;
		if (p[i].velocity.y <= -MAX_FALL) p[i].velocity.y = -MAX_FALL;

		if (p[i].velocity.x >= MAX_RUN) p[i].velocity.x = MAX_RUN;
		if (p[i].velocity.x <= -MAX_RUN) p[i].velocity.x = -MAX_RUN;
		if (p[i].velocity.x >= MAX_WALK && !isButtonPressed(i, B)) p[i].velocity.x = MAX_WALK;
		if (p[i].velocity.x <= -MAX_WALK && !isButtonPressed(i, B)) p[i].velocity.x = -MAX_WALK;


		//p[i].velocity.x = MAX(p[i].velocity.x, -MAX_PLAYER_SPEED);
		//p[i].velocity.x = MIN(p[i].velocity.x, MAX_PLAYER_SPEED);

		p[i].pos = AddVectors(p[i].pos, MultiplyVector(p[i].velocity, BLOCK_SCALE * dt));


		// Invincibility
		p[i].invincibilityTimer -= dt;
		if (p[i].invincibilityTimer > 0.f) {
			// TODO color
		}
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
		sfRenderTexture_drawSprite(_window->renderTexture, playerSprite, NULL);

		p[i].bounds = sfSprite_getGlobalBounds(playerSprite);



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
		sfRectangleShape_setFillColor(rec, color(0, 255, 255, 100));
		sfRectangleShape_setPosition(rec, vector2f(tmpRect4.left, tmpRect4.top));
		sfRectangleShape_setSize(rec, vector2f(tmpRect4.width, tmpRect4.height));
		sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
		
		sfRectangleShape_setFillColor(rec, color(255, 255, 255, 100));
		sfRectangleShape_setPosition(rec, vector2f(tmpRect5.left, tmpRect5.top));
		sfRectangleShape_setSize(rec, vector2f(tmpRect5.width, tmpRect5.height));
		sfRenderTexture_drawRectangleShape(_window->renderTexture, rec, NULL);
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
		p[_id].rect = IntRect(0, 32, 18, 16);
		p[_id].origin = vector2f(8.f, 16.f);
		break;
	case P_BIG:
		p[_id].rect = IntRect(0, 0, 18, 32);
		p[_id].origin = vector2f(8.f, 32.f);
		break;
	case P_FIRETHROWER:
		p[_id].rect = IntRect(0, 53, 18, 32);
		p[_id].origin = vector2f(8.f, 32.f);
		break;
	default:
		break;
	}
}

void setPlayerPower(int _id, playerPower _power)
{
	if (_power <= p[_id].power)
		return;

	p[_id].power = _power;

	changePower(_id);
}

void DamagePlayer(int _id)
{
	if (p[_id].invincibilityTimer > 0.f)
		return;

	p[_id].invincibilityTimer = 3.f;

	if (p[_id].power > P_DEAD)
		p[_id].power--;

	if (p[_id].power <= P_DEAD) {
		// dead
		printf("dead");
	}
	else {
		changePower(_id);
	}
}
