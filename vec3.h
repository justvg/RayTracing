#include <math.h>
#include <fstream>

class vec3
{
	public:
		union
		{
			struct
			{
				float x, y, z;
			};
			struct
			{
				float r, g, b;
			};
			float E[3];
		};

		vec3() {};
		vec3(float X, float Y, float Z) { x = X; y = Y; z = Z; }

		inline const vec3& operator+() { return (*this); }
		inline vec3 operator-() { return (vec3(-x, -y, -z)); }
		inline float operator[](int I) const { return (E[I]); }
		inline float& operator[](int I) { return (E[I]); }

		inline vec3& operator+=(vec3 &V2);
		inline vec3& operator-=(vec3 &V2);
		inline vec3& operator*=(vec3 &V2);
		inline vec3& operator/=(vec3 &V2);
		inline vec3& operator*=(float A);
		inline vec3& operator/=(float A);

		inline float Length()
		{
			float Result = sqrtf(x*x + y*y + z*z);
			return (Result);
		}
		inline float LengthSquared()
		{
			float Result = x*x + y*y + z*z;
			return (Result);
		}
		inline void Normalize();
};

inline std::ifstream& operator>>(std::ifstream &In, vec3 &A)
{
	In >> A.x >> A.y >> A.z;
	return(In);
}

inline std::ofstream& operator<<(std::ofstream &Out, vec3 &A)
{
	Out << A.x << " " << A.y << " " << A.z;
	return(Out);
}

inline void vec3::Normalize()
{
	float InvLength = 1.0f / Length();
	x *= InvLength;
	y *= InvLength;
	z *= InvLength;
}

inline vec3 operator+(vec3 &V1, vec3 &V2)
{
	vec3 Result(V1.x + V2.x, V1.y + V2.y, V1.z + V2.z);
	return (Result);
}

inline vec3 operator-(vec3 &V1, vec3 &V2)
{
	vec3 Result(V1.x - V2.x, V1.y - V2.y, V1.z - V2.z);
	return (Result);
}

inline vec3 operator*(vec3 &V1, vec3 &V2)
{
	vec3 Result(V1.x * V2.x, V1.y * V2.y, V1.z * V2.z);
	return (Result);
}

inline vec3 operator/(vec3 &V1, vec3 &V2)
{
	vec3 Result(V1.x / V2.x, V1.y / V2.y, V1.z / V2.z);
	return (Result);
}

inline vec3 operator*(float A, vec3 &V)
{
	vec3 Result(A*V.x, A*V.y, A*V.z);
	return(Result);
}

inline vec3 operator/(vec3 &V, float A)
{
	vec3 Result(V.x/A, V.y/A, V.z/A);
	return(Result);
}

inline vec3 operator*(vec3 &V, float A)
{
	vec3 Result(A*V.x, A*V.y, A*V.z);
	return(Result);
}

inline float Dot(vec3 &V1, vec3 &V2)
{
	float Result = V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;
	return (Result);
}

inline vec3 Cross(vec3 &V1, vec3 &V2)
{
	vec3 Result;
	Result.x = V1.y*V2.z - V1.z*V2.y;
	Result.y = V1.z*V2.x - V1.x*V2.z;
	Result.z = V1.x*V2.y - V1.y*V2.x;

	return(Result);
}

inline vec3& vec3::operator+=(vec3 &V)
{
	x += V.x;
	y += V.y;
	z += V.z;
	return(*this);
}

inline vec3& vec3::operator-=(vec3 &V)
{
	x -= V.x;
	y -= V.y;
	z -= V.z;
	return(*this);
}

inline vec3& vec3::operator*=(vec3 &V)
{
	x *= V.x;
	y *= V.y;
	z *= V.z;
	return(*this);
}

inline vec3& vec3::operator/=(vec3 &V)
{
	x /= V.x;
	y /= V.y;
	z /= V.z;
	return(*this);
}

inline vec3& vec3::operator*=(float A)
{
	x *= A;
	y *= A;
	z *= A;
	return(*this);
}

inline vec3& vec3::operator/=(float A)
{
	float InvA = 1.0f / A;

	x *= InvA;
	y *= InvA;
	z *= InvA;
	return(*this);
}

inline vec3 Normalize(vec3 V)
{
	vec3 Result = V / V.Length();
	return(Result);
}
