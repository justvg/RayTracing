#if !defined HITABLEH
#define HITABLEH

#include "ray.h"

class material;

struct hit_record
{
	float t;
	vec3 P;
	vec3 Normal;
	material *MatPtr;
};

class hitable
{
	public:
		virtual bool Hit(ray &Ray, float tMin, float tMax, hit_record &Rec) = 0;
};

#endif