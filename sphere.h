#if !defined SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere : public hitable
{
	public: 
		vec3 Center;
		float Radius;
		material *Material;

		sphere() {}
		sphere(vec3 Cen, float R, material *Mat) : Center(Cen), Radius(R), Material(Mat) {};
		virtual bool Hit(ray &Ray, float tMin, float tMax, hit_record &Rec);
};

bool sphere::Hit(ray &Ray, float tMin, float tMax, hit_record &Rec)
{
	vec3 oc = Ray.Origin - Center;
	float A = Dot(Ray.Dir, Ray.Dir);
	float B = Dot(Ray.Dir, oc);
	float C = Dot(oc, oc) - Radius*Radius;
	float Discriminant = B*B - A*C;
	if(Discriminant > 0)
	{
		float Temp = (-B - sqrt(Discriminant)) / A;
		if ((Temp > tMin) && (Temp < tMax))
		{
			Rec.t = Temp;
			Rec.P = Ray.PointAtParameter(Rec.t);
			Rec.Normal = (Rec.P - Center) / Radius;
			Rec.MatPtr = Material;
			return (true);
		}
		Temp = (-B + sqrt(Discriminant)) / A;
		if ((Temp > tMin) && (Temp < tMax))
		{
			Rec.t = Temp;
			Rec.P = Ray.PointAtParameter(Rec.t);
			Rec.Normal = (Rec.P - Center) / Radius;
			Rec.MatPtr = Material;
			return (true);
		}
	}
	return(false);
}

#endif