#include "player.h"
#include "textureManager.h"
#include "map.h"
#include "CustomMath.h"
#include "gamepadx.h"
#include "fireballs.h"
#include "viewManager.h"
#include "menu.h"
#include "game.h"
#include "hud.h"
#include "items.h"
#include "enemies.h"
#include "score.h"
#include "soundManager.h"

#define PLAYER_SPEED 1400.f
#define MAX_PLAYER_SPEED 400.f

#define MIN_WALK 4.453125f
#define MAX_WALK 93.75f
#define MAX_RUN 153.75f
#define ACC_WALK 133.59375f
#define ACC_RUN 200.390625f
#define DEC_REL /*182.8125f*/ 1182.8125f
#define DEC_SKID /*365.625f*/ 1365.625f
#define MIN_SKID /*33.75f*/ 133.75f

#define STOP_FALL 1575.f
#define WALK_FALL 1800.f
#define RUN_FALL 2025.f
#define STOP_FALL_A /*450*/ 450.f * 0.8f
#define WALK_FALL_A /*421.875f*/ 421.875f * 0.8f
#define RUN_FALL_A /*562.5f*/ 562.5f * 0.8f

#define MAX_FALL 270.f
#define MAX_JUMP 270.f


#define STAR_TIMER_DURATION 10.f

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
	sfBool isFlying;
	sfBool canPressA;
	float flagTimer;
	sfBool flagScore;
	sfBool canPlayDieMusic;
	sfBool canPlayClearStageMusic;
	sfBool canPlayFlagPoleSFX;
	sfBool canPlayJumpSFX;
	sfBool canReplayMusic;
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
		resetPlayer(i);
	}

	greatestViewPos = vector2f(960.f, 540.f);

	playerTurn = 0;
	wantedPlayerTurn = 0;

}

void updatePlayer(Window* _window)
{
	if (startTimer > 0.f)
		return;

	float dt = getDeltaTime();

	sfVector2f firstPlayerPos = vector2f(960.f, 540.f);

	int nbPlayersOnFlag = 0;
	int nbPlayersOnFlagNeeded = 0;

	for (int i = 0; i < 2; i++)
	{
		if (i != playerTurn)
			continue;


		float lStickXPos = getStickPos(0, sfTrue, sfTrue);
		float lStickYPos = getStickPos(0, sfTrue, sfFalse);

		nbPlayersOnFlagNeeded++;

		if (!p[i].isAtFlag) {

			if (gameTime <= 0.4f) {
				p[i].power = P_DEAD;
			}

			if (p[i].starTimer > 0.f) {
				StopASound("gameMusic");
				StopASound("lowTimerMusic");
				StopASound("undergroundMusic");
				StopASound("lowUndergroundMusic");
			}
			else if (p[i].canReplayMusic) {
				p[i].canReplayMusic = sfFalse;
				StopASound("gameMusic");
				StopASound("lowTimerMusic");
				StopASound("undergroundMusic");
				StopASound("lowUndergroundMusic");
				if (nbMap == 2) {
					if (gameTime <= 100.f) {
						PlayASound("lowUndergroundMusic", sfTrue);
					}
					else {
						PlayASound("undergroundMusic", sfTrue);
					}
				}
				else {
					if (gameTime <= 100.f) {
						PlayASound("lowTimerMusic", sfTrue);
					}
					else {
						PlayASound("gameMusic", sfTrue);
					}
				}
			}

			if (p[i].power <= P_DEAD) {
				
				if (p[i].canPlayDieMusic) {
					p[i].canPlayDieMusic = sfFalse;
					StopASound("gameMusic");
					StopASound("lowTimerMusic");
					StopASound("undergroundMusic");
					StopASound("lowUndergroundMusic");
					StopASound("starMusic");
					PlayASound("dieMusic", sfFalse);
				}

				p[i].deathTimer += dt;
				p[i].rect = IntRect(96, 32, 16, 16);
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
					startTimer = 6.f;
					hud[i].lives--;
					eraseAllFireballs();
					eraseAllItems();
					eraseAllEnemies();
					resetPlayer(i);

					if (nbTotalPlayers == 2 && i == 0) {
						wantedPlayerTurn = 1;
						playerTurn = 0;
					}
					else {
						wantedPlayerTurn = 0;
						playerTurn = 1;
					}

					if (hud[0].lives <= 0) {
						hud[0].hasGameOver = sfTrue;
						wantedPlayerTurn = 1;
						playerTurn = 0;
					}
					if (hud[1].lives <= 0) {
						hud[1].hasGameOver = sfTrue;
						wantedPlayerTurn = 0;
						playerTurn = 1;
					}
					greatestViewPos = vector2f(960.f, 540.f);
				}
				continue;
			}

			playerState lastState = p[i].state;

			p[i].canGoInYPipe = sfFalse;
			p[i].canGoInXPipe = sfFalse;

			if (!p[i].canJump) {
				p[i].isFlying = sfFalse;
			}

			if (p[i].pos.y > 1080.f + p[i].rect.height * BLOCK_SCALE) {
				p[i].power = P_DEAD;
				p[i].pos.y = 1400.f;
				continue;
			}

			if (!isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
				p[i].isFlying = sfTrue;
			}

			if (!isButtonPressed(0, A) && (p[i].isFlying || p[i].canPressA))
				p[i].canJump = sfTrue;

			if (isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
				p[i].canPressA = sfTrue;
				p[i].canPlayJumpSFX = sfTrue;
			}

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
							if (isButtonPressed(0, B)) {
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
							if (isButtonPressed(0, B)) {
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

				if (p[i].canJump && isButtonPressed(0, A) && isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) { // jump
					//if (p[i].canJump && isButtonPressed(i, A) && isCollision2(p[i].bounds, sfFalse, sfFalse, p[i].velocity, i)) { // jump
					if (p[i].canPlayJumpSFX) {
						if (p[i].power >= P_BIG)
							PlayASound("bigJumpSFX", sfFalse);
						else
							PlayASound("jumpSFX", sfFalse);
						p[i].canPlayJumpSFX = sfFalse;
					}

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
				if (p[i].velocity.y < 0 && isButtonPressed(0, A)) { // holding A while jumping jumps higher
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
			if (p[i].velocity.x >= MAX_WALK && !isButtonPressed(0, B)) p[i].velocity.x = MAX_WALK;
			if (p[i].velocity.x <= -MAX_WALK && !isButtonPressed(0, B)) p[i].velocity.x = -MAX_WALK;


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
				if (isButtonPressed(0, B) && p[i].firethrowerTimer > 0.4f && fabsf(p[i].velocity.x) < 10.f/* && getNbFireballs() < 2*/) { // if debug
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
				PlayASound("pipeSFX", sfFalse);
				StopASound("gameMusic");
				StopASound("lowTimerMusic");
				if (gameTime <= 100.f) {
					PlayASound("lowUndergroundMusic", sfTrue);
				}
				else {
					PlayASound("undergroundMusic", sfTrue);
				}

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
				PlayASound("pipeSFX", sfFalse);
				StopASound("undergroundMusic");
				StopASound("lowUndergroundMusic");
				if (gameTime <= 100.f) {
					PlayASound("lowTimerMusic", sfTrue);
				}
				else {
					PlayASound("gameMusic", sfTrue);
				}

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

			if (!isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
				p[i].rect.left = 80;
			}

			if (p[i].power == P_FIRETHROWER && p[i].firethrowerTimer < 0.2f) {
				p[i].rect = IntRect(256, 53, 16, 32);
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

			if (p[i].canPlayFlagPoleSFX) {
				p[i].canPlayFlagPoleSFX = sfFalse;
				StopASound("gameMusic");
				StopASound("lowTimerMusic");
				StopASound("starMusic");
				PlayASound("flagpoleSFX", sfFalse);
			}
			
			isAtFinish = sfTrue;
			p[i].scale.x = BLOCK_SCALE;
			p[i].color = color(255, 255, 255, 255);

			if (p[i].flagScore) {
				p[i].flagScore = sfFalse;
				if (p[i].pos.y <= 4.f * BLOCK_SCALE * BLOCK_SIZE) createScore(5000, AddVectors(p[i].pos, vector2f(100.f, 0.f)));
				else if (p[i].pos.y <= 6.5f * BLOCK_SCALE * BLOCK_SIZE) createScore(2000, AddVectors(p[i].pos, vector2f(100.f, 0.f)));
				else if (p[i].pos.y <= 8.5f * BLOCK_SCALE * BLOCK_SIZE) createScore(800, AddVectors(p[i].pos, vector2f(100.f, 0.f)));
				else if (p[i].pos.y <= 11.f * BLOCK_SCALE * BLOCK_SIZE) createScore(400, AddVectors(p[i].pos, vector2f(100.f, 0.f)));
				else createScore(100, AddVectors(p[i].pos, vector2f(100.f, 0.f)));
			}

			if (p[i].finishState == 0) {
				p[i].pos.x = 14278.f;
				p[i].velocity.y = 400.f;
				if (!isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
					p[i].pos.y += dt * p[i].velocity.y;
				}
				else {
					p[i].finishState = 1;
				}
			}
			else if (p[i].finishState == 1) {
				p[i].flagTimer += dt;
				p[i].scale.x = -BLOCK_SCALE;
				p[i].pos.x = 14315.f;
					if (p[i].flagTimer > 0.5f) {
						p[i].flagTimer = 0.f;
						p[i].finishState = 2;
					}
			}
			else if (p[i].finishState == 2) {

				if (p[i].canPlayClearStageMusic) {
					p[i].canPlayClearStageMusic = sfFalse;
					PlayASound("clearStageMusic", sfFalse);
				}

				p[i].rect.left = 16;
				p[i].velocity = vector2f(300.f, 270.f);
				if (!isGrounded(p[i].pos, &p[i].velocity, p[i].origin, p[i].bounds)) {
					p[i].pos.y += p[i].velocity.y * dt;
				}
				p[i].pos.x += p[i].velocity.x * dt;

				if (p[i].pos.x > 14725.f) {
					p[i].finishState = 3;
				}
			}

			else if (p[i].finishState == 3) {
				p[i].color.a = 0;
				p[i].scale.x = BLOCK_SCALE;
				p[i].flagTimer += dt;
				if (p[i].flagTimer >= 0.001f) {
					if (gameTime > 1.f) {
						gameTime -= 1.f;
						p[i].flagTimer = 0.f;
						hud[i].score += 50;
					}
					else {
						gameTime = 0.f;
						p[i].flagTimer = 0.f;
						p[i].finishState = 4;
					}
					hud[i].score = min(hud[i].score, 999999);
				}
			}
			else if (p[i].finishState == 4) {
				p[i].color.a = 0;
				p[i].flagTimer += dt;
				if (p[i].flagTimer > 1.f) {
					p[i].flagTimer = 0.f;
					saveLeaderboard();
					if (nbTotalPlayers > 1) {
						greatestViewPos = vector2f(960.f, 540.f);

						hud[i].lives = 0;
						hud[i].hasGameOver = sfTrue;
						hud[i].neverShowAgain = sfTrue;
						startTimer = 6.f;

						if (nbTotalPlayers == 2 && i == 0) {
							wantedPlayerTurn = 1;
							playerTurn = 0;
						}
						else {
							wantedPlayerTurn = 0;
							playerTurn = 1;
						}

						if (hud[0].lives <= 0) {
							hud[0].hasGameOver = sfTrue;
							wantedPlayerTurn = 1;
							playerTurn = 0;
						}
						if (hud[1].lives <= 0) {
							hud[1].hasGameOver = sfTrue;
							wantedPlayerTurn = 0;
							playerTurn = 1;
						}

						int shouldGoToMenu = 0;
						for (int nbr = 0; nbr < nbTotalPlayers; nbr++)
						{
							if (hud[nbr].neverShowAgain)
								shouldGoToMenu++;
						}
						if (shouldGoToMenu >= nbTotalPlayers) {
							changeState(_window, MENU);
						}

						eraseAllFireballs();
						eraseAllItems();
						eraseAllEnemies();
						resetPlayer(i);
					}
					else {
						changeState(_window, MENU);
					}
				}
			}
		}
	}


	// View
	if (nbMap == 2) {
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
	if (startTimer > 0.f)
		return;

	for (int i = 0; i < 2; i++)
	{
		if (i != playerTurn)
			continue;

		if (i == 1) sfSprite_setTextureRect(playerSprite, IntRect(p[i].rect.left, p[i].rect.top + 108, p[i].rect.width, p[i].rect.height));
		else sfSprite_setTextureRect(playerSprite, p[i].rect);
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


	if (p[_id].power > P_DEAD) {
		if (p[_id].power > P_SMALL) {
			p[_id].power = P_SMALL;
		}
		else {
			p[_id].power = P_DEAD;
		}
	}
	if (p[_id].power > P_DEAD) {
		p[_id].invincibilityTimer = 3.f;
		PlayASound("pipeSFX", sfFalse);
		changePower(_id);
	}
	else {
		p[_id].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	}
}

sfVector2f getGreatestViewPos()
{
	return greatestViewPos;
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
	p[_id].canJump = sfFalse;
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

void resetPlayer(int _id)
{
	p[_id].rect = IntRect(0, 32, 16, 16);
	p[_id].origin = vector2f(8.f, 16.f);
	p[_id].scale = vector2f(BLOCK_SCALE, BLOCK_SCALE);
	p[_id].pos = vector2f(3.f * BLOCK_SCALE * BLOCK_SIZE, 13.f * BLOCK_SCALE * BLOCK_SIZE);
	p[_id].velocity = VECTOR2F_NULL;
	p[_id].releaseTimer = 0.f;
	p[_id].state = P_IDLE;
	p[_id].fallAcc = 562.5f;
	p[_id].bounds = FlRect(0.f, 0.f, 0.f, 0.f);
	p[_id].canJump = sfTrue;
	p[_id].nextPos = p[_id].pos;
	p[_id].power = P_SMALL;
	p[_id].invincibilityTimer = 0.f;
	p[_id].firethrowerTimer = 1.f;
	p[_id].canGoInYPipe = sfFalse;
	p[_id].canGoInXPipe = sfFalse;
	p[_id].wasReJumping = sfFalse;
	p[_id].color = color(255, 255, 255, 255);
	p[_id].starTimer = 0.f;
	p[_id].isAtFlag = sfFalse;
	p[_id].finishState = 0;
	p[_id].deathTimer = 0.f;
	p[_id].isFlying = sfFalse;
	p[_id].canPressA = sfTrue;
	p[_id].flagTimer = 0.f;
	p[_id].flagScore = sfTrue;
	p[_id].canPlayDieMusic = sfTrue;
	p[_id].canPlayClearStageMusic = sfTrue;
	p[_id].canPlayFlagPoleSFX = sfTrue;
	p[_id].canPlayJumpSFX = sfTrue;
	p[_id].canReplayMusic = sfFalse;

	isAtFinish = sfFalse;
	greatestViewPos = vector2f(960.f, 540.f);
}

sfBool isPlayerAlive(int _id)
{
	if (p[_id].power <= P_DEAD)
		return sfFalse;

	return sfTrue;
}

void setPlayerCanReplayMusic(int _id, sfBool _canHe)
{
	p[_id].canReplayMusic = _canHe;
}
