#include <fstream>
#include <random>
#include "material.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

vec3 Color(ray &Ray, hitable *World, int FuncDepth)
{
	hit_record Rec;
	if (World->Hit(Ray, 0.001f, FLT_MAX, Rec))
	{
		ray Scattered;
		vec3 Attenuation;
		if ((FuncDepth < 50) && (Rec.MatPtr->Scatter(Ray, Rec, Attenuation, Scattered)))
		{
			vec3 Result = Attenuation*Color(Scattered, World, FuncDepth+1);
			return(Result);
		}
		else
		{
			vec3 Result = vec3(0.0f, 0.0f, 0.0f);
			return(Result);
		}
	}

	vec3 UnitDir = Normalize(Ray.B);
	float t = (UnitDir.y + 1.0f) * 0.5f;
	vec3 Result = (1.0f - t)*vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
	return (Result);
}

hitable *RandomScene()
{
	int N = 500;
	hitable **List = new hitable*[N+1];
	List[0] = new sphere(vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int I = 1;
	for (int A = -11; A < 11; A++)
	{
		for (int B = -11; B < 11; B++)
		{
			float ChooseMat = RandomFloats(Generator);
			vec3 Center(A+0.9f*RandomFloats(Generator), 0.2f, B+0.9f*RandomFloats(Generator));
			if ((Center - vec3(4.0f, 0.2f, 0.0f)).Length() > 0.9f)
			{
				if (ChooseMat < 0.8f)
				{
					List[I++] = new sphere(Center, 0.2f, new lambertian(vec3(RandomFloats(Generator)*RandomFloats(Generator),
																			 RandomFloats(Generator)*RandomFloats(Generator),
																			 RandomFloats(Generator)*RandomFloats(Generator))));
				}
				else if (ChooseMat < 0.95f)
				{
					List[I++] = new sphere(Center, 0.2f, new metal(vec3(0.5f*(1.0f+RandomFloats(Generator)),
																		0.5f*(1.0f + RandomFloats(Generator)),
																		0.5f*(1.0f + RandomFloats(Generator))),
																		0.5f*RandomFloats(Generator)));
				}
				else
				{
					List[I++] = new sphere(Center, 0.2f, new dielectric(1.5f));
				}
			}
		}
	}

	List[I++] = new sphere(vec3(0.0f, 1.0f, 0.0f), 1.0f, new dielectric(1.5f));
	List[I++] = new sphere(vec3(-4.0f, 1.0f, 0.0f), 1.0f, new lambertian(vec3(0.4f, 0.2f, 0.1f)));
	List[I++] = new sphere(vec3(4.0f, 1.0f, 0.0f), 1.0f, new metal(vec3(0.7f, 0.6f, 0.5f), 0.0f));

	hitable *Result = new hitable_list(List, I);
	return(Result);
}

int main(void)
{
	int nX = 1200;
	int nY = 800;
	int nS = 10;
	std::ofstream File("Output.ppm");
	File << "P3\n" << nX << " " << nY << "\n255\n";
	
	vec3 LookFrom(13.0f, 2.0f, 3.0f);
	vec3 LookAt(0.0f, 0.0f, 0.0f);
	float DistToFocus = 10.0f;
	float Aperture = 0.1f;
	camera Cam(LookFrom, LookAt, vec3(0.0f, 1.0f, 0.0f), 20.0f, (float)nX/(float)nY, Aperture, DistToFocus);
	hitable *World = RandomScene();

	for (int J = nY - 1; J >= 0; J--)
	{
		for (int I = 0; I < nX; I++)
		{
			vec3 Col(0.0f, 0.0f, 0.0f);
			for (int S = 0; S < nS; S++)
			{
				float U = ((float)I + RandomFloats(Generator)) / (float)nX;
				float V = ((float)J + RandomFloats(Generator)) / (float)nY;
				ray Ray = Cam.GetRay(U, V);
				Col += Color(Ray, World, 0);
			}
			Col /= (float)nS;
			Col = vec3(sqrtf(Col.x), sqrtf(Col.y), sqrtf(Col.z));
			int iR = (int)(255.99f*Col.r);
			int iG = (int)(255.99f*Col.g);
			int iB = (int)(255.99f*Col.b);

			File << iR << " " << iG << " " << iB << "  ";
		}
		File << "\n";
	}
	File.close();

	return(0);
}