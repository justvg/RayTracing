#if !defined CAMERAH
#define CAMERAH

#include "ray.h"

#define PI 3.14159265358979323846f

vec3 RandomInUnitDisk()
{
	vec3 P;
	do
	{
		P = 2.0f*vec3(RandomFloats(Generator), RandomFloats(Generator), 0.0f) - vec3(1.0f, 1.0f, 0.0f);
	} while(Dot(P, P) >= 1.0f);
	return(P);
}

class camera
{
	public:
		vec3 Origin;
		vec3 LowerLeftCorner;
		vec3 Horizontal;
		vec3 Vertical;
		vec3 Forward, Right, Up;
		float LensRadius;

		camera(vec3 LookFrom, vec3 LookAt, vec3 TempUp, float VertFoV, float Aspect, float Aperture, float FocusDist)
		{
			LensRadius = Aperture / 2.0f;
			float Theta = VertFoV*PI/180.0f;
			float HalfHeight = tanf(Theta/2.0f);
			float HalfWidth = HalfHeight * Aspect;
			Origin = LookFrom;
			Forward = Normalize(LookFrom - LookAt);
			Right = Normalize(Cross(TempUp, Forward));
			Up = Cross(Forward, Right);
			LowerLeftCorner = Origin - HalfWidth*FocusDist*Right - HalfHeight*FocusDist*Up - FocusDist*Forward;
			Horizontal = 2.0f*HalfWidth*FocusDist*Right;
			Vertical = 2.0f*HalfHeight*FocusDist*Up;
		}

		ray GetRay(float U, float V)
		{
			vec3 RandPosInLins = LensRadius*RandomInUnitDisk();
			vec3 Offset = Right*RandPosInLins.x + Up*RandPosInLins.y;
			return (ray(Origin + Offset, LowerLeftCorner + U*Horizontal + V*Vertical - Origin - Offset));
		}
};

#endif