#pragma once
#include "tools.h"

#define EPSILON (float)1e-6
#define DEG2RAD 0.0174533
#define PI 3.1415926536f

#define MAX(a,b) a >= b ? a : b
#define MIN(a,b) a <= b ? a : b

#define VECTOR2F_NULL vector2f(0.f, 0.f)

#define GRAVITATIONAL_FORCE 1.6e-8f


typedef struct {
	sfVector2f point;
	sfVector2f normal;
	float sqrDist;
}Contact;

typedef enum Shape Shape;
enum Shape {
	NO_SHAPE = 0,
	CIRCLE,
	BOX
};

typedef struct bodies bodies;
struct bodies
{
	bodies* pNext;
	Shape* shape;
	sfVector2f pos;
	sfVector2f velocity;
	float mass;
	float bounciness;
	float drag;
	sfColor color;
	float radius;
	sfVector2f origin;
	sfBool isKinematic;
	sfBool isRedirected;

	sfVector2f size;
	sfFloatRect rect;
	sfFloatRect aabb;
	float angle;
	float angularVelocity;
	float momentOfInertia;
	float angularDrag;
};
bodies* debList;

//-----------------------------------------------------------
//	Fonction vector2i ( int _x, int _y )
//  Renvoi un sfVector2i en partant de 2 int
//	_x int
//	_y int
//-----------------------------------------------------------
sfVector2i vector2i(int _x, int _y);

//-----------------------------------------------------------
//	Fonction vector3f ( float _x, float _y, float _z )
//  Renvoi un sfVector3f en partant de 3 float
//	_x float
//	_y float
//	_z float
//-----------------------------------------------------------
sfVector3f vector3f(float _x, float _y, float _z);

float Sign(float a);

//-----------------------------------------------------------
//	Fonction iRand ( int _min, int _max )
//	_min valeur minimum de random
//	_max valeur maximum de random
// 
//	si _max < _min, _min sera toujours renvoyé
// 
//	renvoi un int compris entre _min et _max
//-----------------------------------------------------------
int iRand(int _min, int _max);

//-----------------------------------------------------------
//	Fonction rand_int ( int _min, int _max )
//	_min valeur minimum de random
//	_max valeur maximum de random
// 
//	renvoi un int compris entre _min et _max
//-----------------------------------------------------------
int rand_int(int _min, int _max);

//-----------------------------------------------------------
//	Fonction rand_float ( float _min, float _max )
//	_min valeur minimum de random
//	_max valeur maximum de random
// 
//	renvoi un double compris entre _min et _max
//-----------------------------------------------------------
double rand_float(float _min, float _max);

//-----------------------------------------------------------
//	Fonction lerp ( float )
//	_a point de départ
//	_b destination
//	_t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
float Lerp(float _a, float _b, float _t);

//-----------------------------------------------------------
//	Fonction lerp ( vector2f )
//	_a point de départ
//	_b destination
//	_t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
sfVector2f LerpVector(sfVector2f _a, sfVector2f _b, float _t);

//-----------------------------------------------------------
//	Fonction lerp ( vector3f )
//	v0 point de départ
//	v1 destination
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
sfVector3f vec3f_lerp(sfVector3f v0, sfVector3f v1, float t);

//-----------------------------------------------------------
//	Fonction lerp ( vector4f )
//	v0 point de départ
//	v1 destination
//	t valeur entre 0.0f et 1.0f ( seul variable qui varie )
//-----------------------------------------------------------
vector4f vec4f_lerp(vector4f v0, vector4f v1, float t);

//--------------------------------------------------------------------------------------------
//	Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2)
//	Vérifie si 2 cercles sont en collision
//	_pos1	position du 1er cercle
//	_pos2	position du 2eme cercle
//	_rayon1	rayon du 1er cercle
//	_rayon2	rayon du 2eme cercle
// 
// renvoi un sfBool, sfTrue si il y a collision, sfFalse si il n'y a pas collision
//--------------------------------------------------------------------------------------------
sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2);

//--------------------------------------------------------------------------------------------
//	Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2)
//	Vérifie si 2 rectangles sont en collision
//	_box1	position, largeur et hauteur du 1er rectangle
//	_box2	position, largeur et hauteur du 2eme rectangle
// 
// renvoi un sfBool, sfTrue si il y a collision, sfFalse si il n'y a pas collision
//--------------------------------------------------------------------------------------------
sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2);

sfVector2f PolarCoords(sfVector2f v, float r, float theta);

/// <summary>
/// Create a vector using 2 other vectors
/// </summary>
/// <param name="_v1">Start point</param>
/// <param name="_v2">End point</param>
/// <returns>1 third vector2f</returns>
sfVector2f CreateVector(sfVector2f _v1, sfVector2f _v2);

/// <summary>
/// Add two vectors together
/// </summary>
/// <param name="_v1"></param>
/// <param name="_v2"></param>
/// <returns>1 third vector2f</returns>
sfVector2f AddVectors(sfVector2f _v1, sfVector2f _v2);

/// <summary>
/// Substract two vectors -> the First minus Second vector
/// </summary>
/// <param name="_v1">The first number</param>
/// <param name="_v2">The second number</param>
/// <returns>A third vector2f</returns>
sfVector2f SubstractVectors(sfVector2f _v1, sfVector2f _v2);

/// <summary>
/// Multiply a vector by a multiplier
/// </summary>
/// <param name="_vector">A vector</param>
/// <param name="_multiply">The multiplier</param>
/// <returns>The Vector muliplied by the float</returns>
sfVector2f MultiplyVector(sfVector2f _vector, float _multiplier);

/// <summary>
/// Dot Product of two Vectors
/// </summary>
/// <param name="_v1">First vector</param>
/// <param name="_v2">Second vector</param>
/// <returns>Dot Product in float between -1 and 1</returns>
float DotProduct(sfVector2f _v1, sfVector2f _v2);

/// <summary>
/// Magnitude of a vector using sqrtf
/// </summary>
/// <param name="_vector">A Vector</param>
/// <returns>Magnitude (distance) in float</returns>
float GetMagnitude(sfVector2f _vector);

/// <summary>
/// Magnitude of a vector without using sqrtf
/// </summary>
/// <param name="_vector">A Vector</param>
/// <returns>Square of Magnitude (distance) in float</returns>
float GetSqrMagnitude(sfVector2f _vector);

/// <summary>
/// Normalize a Vector
/// </summary>
/// <param name="_vector">A vector</param>
/// <returns>A vector between {-1,-1} and {1,1}</returns>
sfVector2f Normalize(sfVector2f _vector);

/// <summary>
/// Get the angle between two vectors
/// </summary>
/// <param name="_v1">First vector</param>
/// <param name="_v2">Second vector</param>
/// <returns>The angle in degrees</returns>
float GetAngleBetweenVectors(sfVector2f _v1, sfVector2f _v2);

/// <summary>
/// Check if a lookingObject can see the targetedObject with a max distance and an FOV
/// </summary>
/// <param name="_lookingObject">The looking object</param>
/// <param name="_forwardVector">The forward vector of the looking object</param>
/// <param name="_targetObject">The targeted object</param>
/// <param name="_maxRadius">The maximum distance visible</param>
/// <param name="_fieldOfView">The FOV of the looking object</param>
/// <returns>If the targeted object is in the FOV of the looking object</returns>
sfBool IsInFOV(sfVector2f _lookingObject, sfVector2f _forwardVector, sfVector2f _targetObject, float _maxRadius, float _fieldOfView);

/// <summary>
/// Adds force to the velocity of a body (One time call)
/// </summary>
/// <param name="rb">The body</param>
/// <param name="_force">How much force</param>
/// <param name="usingMass">sfTrue to divide by the mass, sfFalse otherwise</param>
void AddForce(bodies* rb, sfVector2f _force, sfBool usingMass);

/// <summary>
/// Adds force to the velocity of a body at a certain point (One time call)
/// </summary>
/// <param name="rb">The body</param>
/// <param name="_force">How much force</param>
/// <param name="_pos">The point in global position</param>
void AddForceAtPosition(bodies* rb, sfVector2f _force, sfVector2f _pos);

/// <summary>
/// Reflects a body, doesn't modify the values (Call it with AddForce())
/// </summary>
/// <param name="_i">The previous velocity of the body</param>
/// <param name="_n">Where to reflect (a vector between -1.f and 1.f)</param>
/// <param name="_bounciness">The bounciness of the body</param>
/// <returns>The new velocity after the reflection</returns>
sfVector2f Reflect(sfVector2f _i, sfVector2f _n, float _bounciness);

/// <summary>
/// Attracts a body to a point
/// </summary>
/// <param name="rb">The body</param>
/// <param name="_center">The point that attracts</param>
/// <param name="_power">How much power</param>
/// <param name="_range">From what distance to attract</param>
void Attract(bodies* rb, sfVector2f _center, float _power, float _range);

/// <summary>
/// Predicts the force to apply
/// </summary>
/// <param name="_pos">Current position of the body</param>
/// <param name="_attractPos">The body that attracts</param>
/// <param name="_mass">Using the mass of the body (1.f to ignore it)</param>
/// <param name="_attractMass">The mass of the body that attracts (1.f to ignore it)</param>
/// <param name="_time">The time where the force will be apply</param>
/// <returns></returns>
sfVector2f getForceToApply(sfVector2f _pos, sfVector2f _attractPos, float _mass, float _attractMass, float _time);

/// <summary>
/// Tells if a point is in the segment of a shape
/// </summary>
/// <param name="pointTest">The point you want to test in global coordinates</param>
/// <param name="p1">The first point of the segment in local coordinates</param>
/// <param name="p2">The second point of the segment in local coordinates</param>
/// <param name="contact">The Contact structure</param>
/// <returns>sfTrue if the point is in the segment, sfFalse otherwise</returns>
sfBool PointSegmentDistance(sfVector2f pointTest, sfVector2f p1, sfVector2f p2, Contact* contact);

/// <summary>
/// Tells if a Circle is in collision with an other circle
/// </summary>
/// <param name="pointA">The position of the first circle in global coordinates</param>
/// <param name="radiusA">The radius of the first circle</param>
/// <param name="pointB">The position of the second circle in global coordinates</param>
/// <param name="radiusB">The radius of the second circle</param>
/// <param name="contact">The Contact structure</param>
/// <returns>sfTrue if there's a collision, sfFalse otherwise</returns>
sfBool CircleCircleCollision(sfVector2f pointA, float radiusA, sfVector2f pointB, float radiusB, Contact* contact);

/// <summary>
/// Tells if a Circle is in collision with a Box shape
/// </summary>
/// <param name="point">The position of the circle in global coordinates</param>
/// <param name="radius">The radius of the circle</param>
/// <param name="rectPoints">All points of the Box shape</param>
/// <param name="contact">The Contact structure</param>
/// <returns>sfTrue if there's a collision, sfFalse otherwise</returns>
sfBool CircleBoxCollision(sfVector2f point, float radius, sfVector2f* rectPoints, Contact* contact);

/// <summary>
/// Tells if a Box shape is in collision with an other Box (doesn't work currently)
/// </summary>
/// <param name="pointsA">All points of the first Box shape</param>
/// <param name="pointsB">All points of the second Box</param>
/// <param name="contact">The Contact structure</param>
/// <returns>sfTrue if there's a collision, sfFalse otherwise</returns>
sfBool BoxBoxCollision(sfVector2f* pointsA, sfVector2f* pointsB, Contact* contact);

