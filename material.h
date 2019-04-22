#if !defined MATERIALH
#define MATERIALH

#include "hitable.h"

std::uniform_real_distribution<float> RandomFloats(0.0f, 1.0f);
std::default_random_engine Generator;

class material
{
	public:
		virtual bool Scatter(ray &RayIn, hit_record &Rec, vec3 &Attenuation, ray &Scattered) = 0;
};

vec3 RandomInUnitSphere()
{
	vec3 P;
	do
	{
		P = 2.0f*vec3(RandomFloats(Generator), RandomFloats(Generator), RandomFloats(Generator)) - vec3(1.0f, 1.0f, 1.0f);
	} while (P.LengthSquared() >= 1.0f);
	return(P);
}

class lambertian : public material
{
	public:
		vec3 Albedo;

		lambertian(vec3 &A) : Albedo(A) {}
		virtual bool Scatter(ray &RayIn, hit_record &Rec, vec3 &Attenuation, ray &Scattered)
		{
			vec3 Target = Rec.P + Rec.Normal + RandomInUnitSphere();
			Scattered = ray(Rec.P, Target - Rec.P);
			Attenuation = Albedo;
			return (true);
		}
};

vec3 Reflect(vec3 &V, vec3 &Normal)
{
	vec3 Result = V - 2.0f*Dot(V, Normal)*Normal;
	return(Result);
}

class metal : public material
{
	public:
		vec3 Albedo;
		float Fuzz;

		metal(vec3 &A, float F) : Albedo(A) 
		{
			Fuzz = (F < 1.0f) ? F : 1.0f;
		}
		virtual bool Scatter(ray &RayIn, hit_record &Rec, vec3 &Attenuation, ray &Scattered)
		{
			vec3 Reflected = Reflect(Normalize(RayIn.Dir), Rec.Normal);
			Scattered = ray(Rec.P, Reflected + Fuzz*RandomInUnitSphere());
			Attenuation = Albedo;
			return (Dot(Scattered.Dir, Rec.Normal) > 0);
		}
};

bool Refract(vec3 V, vec3 &N, float RefrIdxRatio, vec3 &Refracted)
{
	V = Normalize(V);
	float dt = Dot(V, N);
	float Discriminant = 1.0f - RefrIdxRatio*RefrIdxRatio*(1.0f - dt*dt);
	if (Discriminant > 0.0f)
	{
		Refracted = RefrIdxRatio*(V - N*dt) - N*sqrtf(Discriminant);
		return(true);
	}
	
	return(false);
}

float Schlick(float Cosine, float RefrIdx)
{
	float R0 = (1.0f - RefrIdx) / (1.0f + RefrIdx);
	R0 = R0*R0;
	float Result = R0 + (1.0f - R0)*pow((1.0f-Cosine), 5.0f);
	return(Result);
}

class dielectric : public material
{
	public:
		float RefrIdx;

		dielectric(float RI) : RefrIdx(RI) {}
		virtual bool Scatter(ray &RayIn, hit_record &Rec, vec3 &Attenuation, ray &Scattered)
		{
			vec3 OutwardNormal;
			vec3 Reflected = Reflect(RayIn.Dir, Rec.Normal);
			float RefrIdxRatio;
			Attenuation = vec3(1.0f, 1.0f, 1.0f);
			vec3 Refracted;
			float ReflectProb;
			float Cosine;
			if (Dot(RayIn.Dir, Rec.Normal) > 0.0f)
			{
				OutwardNormal = -Rec.Normal;
				// NOTE: RefrIdx / AirRefrIdx; AirRefrIndx = 1.0f;
				RefrIdxRatio = RefrIdx;
				Cosine = RefrIdx * Dot(RayIn.Dir, Rec.Normal) / RayIn.Dir.Length();
			}
			else
			{
				OutwardNormal = Rec.Normal;
				RefrIdxRatio = 1.0f / RefrIdx;
				Cosine = -Dot(RayIn.Dir, Rec.Normal) / RayIn.Dir.Length();
			}

			if (Refract(RayIn.Dir, OutwardNormal, RefrIdxRatio, Refracted))
			{
				ReflectProb = Schlick(Cosine, RefrIdx);
			}
			else
			{
				ReflectProb = 1.0f;
			}

			if (RandomFloats(Generator) < ReflectProb)
			{
				Scattered = ray(Rec.P, Reflected);
			}
			else
			{
				Scattered = ray(Rec.P, Refracted);
			}

			return(true);
		}

};

#endif