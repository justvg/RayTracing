#if !defined HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

class hitable_list : public hitable
{
	public:
		hitable **List;
		int ListSize;

		hitable_list() {}
		hitable_list(hitable **L, int N) { List = L; ListSize = N; }
		virtual bool Hit(ray &Ray, float tMin, float tMax, hit_record &Rec);
};

bool hitable_list::Hit(ray &Ray, float tMin, float tMax, hit_record &Rec)
{
	hit_record TempRec;
	bool HitAnything = false;
	double ClosestT = tMax;
	for (int I = 0; I < ListSize; I++)
	{
		if (List[I]->Hit(Ray, tMin, ClosestT, TempRec))
		{
			HitAnything = true;
			ClosestT = TempRec.t;
			Rec = TempRec;
		}
	}
	return(HitAnything);
}

#endif