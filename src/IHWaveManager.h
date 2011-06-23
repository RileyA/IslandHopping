#ifndef IH_WAVEMANAGER_H
#define IH_WAVEMANAGER_H

#include "IH.h"
#include "OryxObject.h"

namespace IH
{
	class WaveManager : public Object
	{
	public:

		WaveManager(Real turbulence);
		virtual ~WaveManager();

		void update(Real delta);
		
		inline Real getDisplacement(Vector2 pt)
		{
			Real disp = 0.f;
			for(int i = 0; i < mWaves.size(); ++i)
			{
				// distance from wave line to this pt
				Real dist = fabs(((pt - mWaves[i].origin).dotProduct(mWaves[i].pos) / mWaves[i].length) - mWaves[i].length);
				if(dist < mWaves[i].radius)
				{
					Real prop = 1-dist/mWaves[i].radius;
					Real p2 = prop * prop;
					disp += p2 < 0.85f ? p2 * mWaves[i].strength : mWaves[i].strength;// - mWaves[i].strength;
				}
			}
			return disp;
		}

		inline Real getDisplacement(Vector2 pt, Vector3& normal)
		{
			Real disp = 0.f;
			Real scale = fabs(pt.x);
			scale = scale < 50.f ? 1.f : 1-std::min(1.f,(scale-50.f)/13.f);
			for(int i = 0; i < mWaves.size(); ++i)
			{
				// distance from wave line to this pt
				Real dist = ((pt - mWaves[i].origin).dotProduct(mWaves[i].pos) / mWaves[i].length) - mWaves[i].length;
				Real df = fabs(dist);
				if(df < mWaves[i].radius)
				{
					Real prop = 1-df/mWaves[i].radius;
					Real str = (prop)*(prop) * mWaves[i].strength;// - mWaves[i].strength;
					disp += str * 0.95f;
					Real sign = (dist < 0) ? 1 : -1;
					if(prop > 0.8f)
					{
						Real temp = (prop - 0.9f)/0.1f;// scale to 0-1
						normal += (mWaves[i].direction * 0.4f * sign * (1-temp) + 
							Vector3(0,0.6f + (1-temp)*0.4f,0)) * 
							mWaves[i].strength * prop;
					}
					else
					{
						normal += (mWaves[i].direction * (prop*sign/2) + Vector3(0,1-prop/2,0)) * 
							mWaves[i].strength * prop;
					}
				}
			}
			return disp * scale;
		}

		inline Real getDisplacementFast(Vector2 pt, Vector3& normal)
		{
			Real disp = 0.f;
			Real scale = fabs(pt.x);
			scale = scale < 50.f ? 1.f : 1-std::min(1.f,(scale-50.f)/13.f);
			for(int i = 0; i < mWaves.size(); ++i)
			{
				// distance from wave line to this pt
				Real dist = ((pt - mWaves[i].origin).dotProduct(mWaves[i].pos) / mWaves[i].length) - mWaves[i].length;
				Real df = fabs(dist);
				if(df < mWaves[i].radius)
				{
					Real prop = 1-df/mWaves[i].radius;
					Real str = (prop)*(prop) * mWaves[i].strength;// - mWaves[i].strength;
					disp += str;// * 0.85f;
					Real sign = (dist < 0) ? 1 : -1;
					if(prop > 0.8f)
					{
						Real temp = (prop - 0.9f)/0.1f;// scale to 0-1
						normal += (mWaves[i].direction * 0.4f * sign * (1-temp) + 
							Vector3(0,0.6f + (1-temp)*0.4f,0)) * 
							mWaves[i].strength * prop;
					}
					else
					{
						normal += (mWaves[i].direction * (prop*sign/2) + Vector3(0,1-prop/2,0)) * 
							mWaves[i].strength * prop;
					}
					//normal += (mWaves[i].direction * (prop*sign/2) + Vector3(0,1-prop/2,0)) * 
					//	mWaves[i].strength * prop;
				}
			}
			return disp*scale;
		}

		void pushWave(Vector2 pos, Vector2 origin, Real radius, Real strength)
		{
			mWaves.push_back(Wave());
			mWaves.back().pos = pos;
			mWaves.back().origin = origin;
			mWaves.back().sqlength = pos.squaredLength();
			mWaves.back().length = sqrt(mWaves.back().sqlength);
			mWaves.back().radius = radius;
			mWaves.back().strength = strength;
			Vector2 copy = pos / mWaves.back().length;
			//mWaves.back().direction = Vector3(-copy.y, 0, copy.x);
			mWaves.back().direction = Vector3(copy.x, 0, -copy.y);
			// there be magic here
			//mWaves.back().direction2 = Vector3(-copy.y, 0, copy.x);
		}

		void playerMoved(const Message& m);

	private:

		// a magical cylindrical wave
		struct Wave
		{
			Vector2 pos;
			Vector2 origin;
			Vector3 direction;
			//Vector3 direction2;
			Real length;
			Real sqlength;
			Real radius;
			Real strength;
		};

		std::vector<Wave> mWaves;
		Real mTurbulence;
		Vector2 mPlayerPos2d;
		Vector3 mPlayerPos;
		Real mTimer;
	};
}

#endif
