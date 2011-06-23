#include "IHWaveManager.h"

namespace IH
{
	WaveManager::WaveManager(Real turbulence)
		:Object("Default","WaveMgr"),mTurbulence(turbulence),mPlayerPos(0,0,0)
	{
		createSlot("playerMoved", this, &WaveManager::playerMoved);
		mTimer = 0.3f;
	}
	
	WaveManager::~WaveManager()
	{
		
	}

	void WaveManager::update(Real delta)
	{
		// advances waves, etc
		for(int i = 0; i < mWaves.size(); ++i)
		{
			Vector2 d = mWaves[i].pos;
			d.normalize();
			mWaves[i].pos -= d * delta * 3.f;
			if(mWaves[i].pos.y >= 0.f)
			{
				mWaves.erase(mWaves.begin()+i);
				--i;
			}
			else
			{
				mWaves[i].length -= delta*3.f;
				mWaves[i].sqlength -= mWaves[i].length * mWaves[i].length;
			}
		}

		mTimer -=delta;
		if(mWaves.size() < 10 && mTimer < 0.f)
		{
			Vector2 plpos = Vector2(mPlayerPos.x, mPlayerPos.z);
			pushWave(Vector2(Rand::get().genFloat(-80.f,80.f),-100 +
				Rand::get().genFloat(-10.f,10.f)), plpos, 
				Rand::get().genFloat(8.f,16.f),
				Rand::get().genFloat(0.4f,3.0f));
			mTimer = Rand::get().genFloat(1.5f,2.5f);
		}
	}

	void WaveManager::playerMoved(const Message& m)
	{
		if(const Vector3* pos = unpackMsg<Vector3>(m))
		{
			mPlayerPos = *pos;
			mPlayerPos2d = Vector2((*pos).x,0);
		}
	}
}
