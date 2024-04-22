#include "CustomMath.h"

sfVector2i vector2i(int _x, int _y)
{
    sfVector2i tmp = { _x, _y };
    return tmp;
}

sfVector3f vector3f(float _x, float _y, float _z)
{
    sfVector3f tmp = { _x, _y, _z };
    return tmp;
}

float Sign(float a)
{
    if (a < 0.f) return -1.f;
    return 1.f;
}

int iRand(int _min, int _max)
{
    if (_max > _min)
        return rand() % (_max - _min + 1) + _min;
    else
        return _min;
}

int rand_int(int _min, int _max)
{
    return rand() % (_max - _min) + _min;
}

double rand_float(float _min, float _max)
{
    return ((rand() / (double)RAND_MAX) * ((double)_max - (double)_min) + (double)_min);
}

sfVector2f getfWorldMousePos(sfRenderWindow* _window)
{
    sfVector2i iMousePos = sfMouse_getPositionRenderWindow(_window);
    sfVector2f fWorldMousePos;
    fWorldMousePos.x = (float)iMousePos.x;
    fWorldMousePos.y = (float)iMousePos.y;
    fWorldMousePos = sfRenderWindow_mapPixelToCoords(_window, iMousePos, getView());
    return fWorldMousePos;
}

sfVector2f getfMousePos(sfRenderWindow* _window)
{
    sfVector2i iMousePos = sfMouse_getPositionRenderWindow(_window);
    sfVector2f fWorldMousePos;
    fWorldMousePos.x = (float)iMousePos.x;
    fWorldMousePos.y = (float)iMousePos.y;
    return fWorldMousePos;
}

sfBool lClick(sfRenderWindow* _window)
{
    if (sfMouse_isButtonPressed(sfMouseLeft) && sfRenderWindow_hasFocus(_window)) return sfTrue;
    return sfFalse;
}

sfBool rClick(sfRenderWindow* _window)
{
    if (sfMouse_isButtonPressed(sfMouseRight) && sfRenderWindow_hasFocus(_window)) return sfTrue;
    return sfFalse;
}

sfBool middleClick(sfRenderWindow* _window)
{
    if (sfMouse_isButtonPressed(sfMouseMiddle) && sfRenderWindow_hasFocus(_window)) return sfTrue;
    return sfFalse;
}

float Lerp(float _a, float _b, float _t)
{
    return (_b - _a) * _t + _a;
}

sfVector2f LerpVector(sfVector2f _a, sfVector2f _b, float _t)
{
    return vector2f(Lerp(_a.x, _b.x, _t), Lerp(_a.y, _b.y, _t));
}

sfVector3f vec3f_lerp(sfVector3f v0, sfVector3f v1, float t)
{
    sfVector3f tmp;
    tmp.x = (1 - t) * v0.x + t * v1.x;
    tmp.y = (1 - t) * v0.y + t * v1.y;
    tmp.z = (1 - t) * v0.z + t * v1.z;
    return tmp;
}

vector4f vec4f_lerp(vector4f v0, vector4f v1, float t)
{
    vector4f tmp;
    tmp.r = (1 - t) * v0.r + t * v1.r;
    tmp.g = (1 - t) * v0.g + t * v1.g;
    tmp.b = (1 - t) * v0.b + t * v1.b;
    tmp.a = (1 - t) * v0.a + t * v1.a;
    return tmp;
}

sfBool Circle_Collision(sfVector2f _pos1, sfVector2f _pos2, float _rayon1, float _rayon2)
{
    // if (sqrt(pow(_pos1.x - _pos2.x, 2) + pow(_pos1.y - _pos2.y, 2)) < _rayon1 + _rayon2) // Version non optimisé
    // Version Optimisé
    if ((_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y) < (_rayon1 + _rayon2) * (_rayon1 + _rayon2))
    {
        return sfTrue;
    }
    else return sfFalse;
}

sfBool Rectangle_Collision(sfFloatRect _box1, sfFloatRect _box2)
{
    if ((_box2.left >= _box1.left + _box1.width)      // trop à droite
        || (_box2.left + _box2.width <= _box1.left) // trop à gauche
        || (_box2.top >= _box1.top + _box1.height) // trop en bas
        || (_box2.top + _box2.height <= _box1.top))  // trop en haut
        return sfFalse;
    else
        return sfTrue;
}

sfVector2f PolarCoords(sfVector2f v, float r, float theta)
{
    return vector2f(v.x + (r * cosf(theta)), v.y + (r * sinf(theta)));
}

sfVector2f CreateVector(sfVector2f _v1, sfVector2f _v2)
{
    sfVector2f v = { (_v2.x - _v1.x) , (_v2.y - _v1.y) };
    return v;
}

sfVector2f AddVectors(sfVector2f _v1, sfVector2f _v2)
{
    sfVector2f v = { _v1.x + _v2.x, _v1.y + _v2.y };
    return v;
}

sfVector2f SubstractVectors(sfVector2f _v1, sfVector2f _v2)
{
    sfVector2f v = { _v1.x - _v2.x, _v1.y - _v2.y };
    return v;
}

sfVector2f MultiplyVector(sfVector2f _vector, float _multiplier)
{
    sfVector2f v = { _vector.x * _multiplier, _vector.y * _multiplier };
    return v;
}

float DotProduct(sfVector2f _v1, sfVector2f _v2)
{
    float d = (_v1.x * _v2.x + _v1.y * _v2.y);
    return d;
}

float GetMagnitude(sfVector2f _vector)
{
    float m = sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
    return m;
}

float GetSqrMagnitude(sfVector2f _vector)
{
    float m = (_vector.x * _vector.x + _vector.y * _vector.y);
    return m;
}

sfVector2f Normalize(sfVector2f _vector)
{
    float magnitude = GetMagnitude(_vector);
    if (magnitude < EPSILON) return _vector;
    sfVector2f v = { _vector.x / magnitude, _vector.y / magnitude };
    return v;
}

float GetAngleBetweenVectors(sfVector2f _v1, sfVector2f _v2)
{
    float a = atan2f(_v2.y, _v2.x) - atan2f(_v1.y, _v1.x);
    return a;
}

sfBool IsInFOV(sfVector2f _lookingObject, sfVector2f _forwardVector, sfVector2f _targetObject, float _maxRadius, float _fieldOfView)
{
    sfVector2f lookToTargetVector = CreateVector(_lookingObject, _targetObject);
    float sqrdistance = GetSqrMagnitude(lookToTargetVector);
    if (sqrdistance > _maxRadius * _maxRadius) return sfFalse;

    sfVector2f lookToTargetNorm = Normalize(lookToTargetVector);
    if (DotProduct(lookToTargetNorm, _forwardVector) > cosf(_fieldOfView * DEG2RAD * 0.5f)) return sfTrue;

    return sfFalse;

}

void AddForce(bodies* rb, sfVector2f _force, sfBool usingMass)
{
	float massToApply = !usingMass ? 1.f : rb->mass;
	rb->velocity = AddVectors(rb->velocity, MultiplyVector(_force, 1.f / massToApply));
}

void AddForceAtPosition(bodies* rb, sfVector2f _force, sfVector2f _pos)
{
	if (rb->isKinematic) return;

	AddForce(rb, _force, sfTrue);
	sfVector2f r = CreateVector(rb->pos, _pos);
	rb->angularVelocity += (r.x * _force.y - r.y * _force.x) / rb->momentOfInertia;
}

sfBool TestAABBOverlap(sfFloatRect a, sfFloatRect b)
{
	//float d1x = b.top + b.height - a.left + a.
	//float d1y = 
	//float d2x = 
	//float d2y = 
	//
	//if (d1x > 0.f || d1y > 0.f || d2x > 0.f || d2y > 0.f) return sfFalse;

	return sfTrue;
}

sfVector2f Reflect(sfVector2f _i, sfVector2f _n, float _bounciness)
{
	return MultiplyVector(SubstractVectors(_i, MultiplyVector(_n, 2 * DotProduct(_i, _n))), _bounciness);
}

void Attract(bodies* rb, sfVector2f _center, float _power, float _range)
{
	sfVector2f rbToCenter = CreateVector(rb->pos, _center);
	float dist = GetMagnitude(rbToCenter);
	float maxed = MAX(0.f, (_range - dist));
	//float force = MAX(0.f, ((_range, -dist) * (_range - dist)) / (_range * _range));
	float force = (maxed * maxed) / (_range * _range) * _power;
	AddForce(rb, MultiplyVector(rbToCenter, force * getDeltaTime()), sfTrue);
}

// using mass, _time to 1.f to ignore it
sfVector2f getForceToApply(sfVector2f _pos, sfVector2f _attractPos, float _mass, float _attractMass, float _time)
{
	sfVector2f _planetToSun = CreateVector(_pos, _attractPos);
	float _gravityForce = GRAVITATIONAL_FORCE * (_mass * _attractMass / GetSqrMagnitude(_planetToSun));
	sfVector2f _forceDir = Normalize(_planetToSun);
	sfVector2f _force = MultiplyVector(_forceDir, _gravityForce * _time / _mass);
	return _force;
}



sfBool PointSegmentDistance(sfVector2f pointTest, sfVector2f p1, sfVector2f p2, Contact* contact)
{
	sfVector2f contactPoint = vector2f(0.f, 0.f);

	sfVector2f p1ToP2 = CreateVector(p1, p2);
	sfVector2f p1ToPoint = CreateVector(p1, pointTest);

	float sqrDist = GetSqrMagnitude(p1ToP2);
	float d = DotProduct(p1ToP2, p1ToPoint) / sqrDist;
	Contact contactP;

	if (d <= 0)
		contactP.point = p1;
	else if (d >= 1)
		contactP.point = p2;

	else
		contactP.point = AddVectors(p1, MultiplyVector(p1ToP2, d));

	contactP.normal = vector2f(0.f, 0.f);
	contactP.sqrDist = GetSqrMagnitude(CreateVector(pointTest, contactP.point));
	*contact = contactP;
	return sfTrue;
}

sfBool CircleCircleCollision(sfVector2f pointA, float radiusA, sfVector2f pointB, float radiusB, Contact* contact)
{
	sfVector2f circleCircle = CreateVector(pointA, pointB);
	float sqrDist = GetSqrMagnitude(circleCircle);
	if (sqrDist <= (radiusA + radiusB) * (radiusA + radiusB))
	{
		Contact contactP;
		sfVector2f norm = Normalize(circleCircle);
		contactP.point = AddVectors(MultiplyVector(norm, radiusA), pointA);
		contactP.normal = norm;
		contactP.sqrDist = sqrDist;
		*contact = contactP;
		return sfTrue;
	}

	return sfFalse;
}

sfBool CircleBoxCollision(sfVector2f point, float radius, sfVector2f* rectPoints, Contact* contact)
{
	float minDistSqr = LONG_MAX;
	sfVector2f closestPoint = vector2f(0.f, 0.f);

	Contact contactP;
	for (int i = 0; i < 4; i++)
	{
		sfVector2f va = rectPoints[i];
		sfVector2f vb = rectPoints[(i + 1) % 4];

		sfBool isContact = PointSegmentDistance(point, va, vb, &contactP);
		if (!isContact)
			continue;

		if (contactP.sqrDist <= minDistSqr)
		{
			minDistSqr = contactP.sqrDist;
			closestPoint = contactP.point;
		}
	}

	float distCircle = GetSqrMagnitude(CreateVector(point, closestPoint));
	if (distCircle <= radius * radius)
	{
		contactP.sqrDist = distCircle;
		contactP.point = closestPoint;
		contactP.normal = Normalize(CreateVector(closestPoint, point));
		if (contact != NULL)
			*contact = contactP;
		return sfTrue;
	}
	contact = NULL;
	return sfFalse;
}

sfBool BoxBoxCollision(sfVector2f* pointsA, sfVector2f* pointsB, Contact* contact)
{
	sfVector2f closestPoint = vector2f(0.f, 0.f);
	float minDistSqr = LONG_MAX;

	int minIndex = 0;
	sfBool isBRect = sfFalse;

	Contact contactP;

	for (int i = 0; i < 4; i++)
	{
		sfVector2f p = pointsA[i];
		for (int j = 0; j < 4; j++)
		{
			sfVector2f va = pointsB[j];
			sfVector2f vb = pointsB[(j + 1) % 4];
			sfBool isContact = PointSegmentDistance(p, va, vb, &contactP);
			if (!isContact)
				continue;

			if (contactP.sqrDist < minDistSqr)
			{
				minDistSqr = contactP.sqrDist;
				closestPoint = contactP.point;
				minIndex = i;
				isBRect = sfFalse;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		sfVector2f p = pointsB[i];
		for (int j = 0; j < 4; j++)
		{
			sfVector2f va = pointsA[j];
			sfVector2f vb = pointsA[(j + 1) % 4];
			sfBool isContact = PointSegmentDistance(p, va, vb, &contactP);
			if (!isContact)
				continue;

			if (contactP.sqrDist < minDistSqr)
			{
				minDistSqr = contactP.sqrDist;
				closestPoint = contactP.point;
				minIndex = i;
				isBRect = sfTrue;
			}
		}
	}

	float sqrMagnitude = GetSqrMagnitude(CreateVector(closestPoint, isBRect ? pointsB[minIndex] : pointsA[minIndex]));
	if (sqrMagnitude > 0.2f)
	{
		contact = NULL;
		return sfFalse;
	}

	contactP.normal = vector2f(0.f, 0.f);
	*contact = contactP;

	return sfTrue;

}










