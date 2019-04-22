#if !defined RAYH
#define RAYH
#include "vec3.h"

class ray
{
	public:
		union
		{
			struct
			{
				vec3 A, B;
			};
			struct
			{
				vec3 Origin, Dir;
			};
		};
		
		ray() {}
		ray(vec3 &a, vec3 &b) { Origin = a; Dir = b; }
		vec3 PointAtParameter(float t) { return (A + t*B); }
};

#endif