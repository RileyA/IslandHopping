#include "IHGenerator.h"
#include "OryxMessageAny.h"
#include "OryxBucket.h"

namespace IH
{
	Generator::Generator(unsigned int mSeed, int steps, Real stepSize)
		:Object(),mStepSize(stepSize),mGenerateSteps(steps),mOffset(0.f),mRand(mSeed),
			mGeneratedTo(0)
	{
		createSlot("playerPos", this, &Generator::playerMoved);
		if(!Engine::getPtr()->getBucket("Islands"))
			Engine::getPtr()->createBucket("Islands")->setTemporary(true);
		mIslandTypes.push_back(new IslandSchematic());
	}
	//-----------------------------------------------------------------------
	
	Generator::~Generator()
	{

	}
	//-----------------------------------------------------------------------
	
	void Generator::init()
	{
	}
	//-----------------------------------------------------------------------
	
	void Generator::deinit()
	{
		// islands are Oryx::Objects, so they get deleted by the engine
		mIslands.clear();
		mSpareIslands.clear();
		for(int i = 0; i < mIslandGroupings.size(); ++i)
			delete mIslandGroupings[i];
		mIslandGroupings.clear();
		for(int i = 0; i < mIslandTypes.size(); ++i)
			delete mIslandTypes[i];
		mIslandTypes.clear();
	}
	//-----------------------------------------------------------------------
	
	void Generator::update(Real delta)
	{
		// ...
	}
	//-----------------------------------------------------------------------
	
	void Generator::generateTo(Vector3 position)
	{
		// clear old islands first
		for(int i = 0; i < mIslands.size(); ++i)
		{
			if(!mIslands[i]->mActive)
			{
				mSpareIslands.push_back(mIslands[i]);
				mIslands.erase(mIslands.begin()+i);
				--i;
			}
			else if(mIslands[i]->mMesh->getPosition().z > position.z + mStepSize * 5)
			{
				std::cout<<"hiding one!\n";
				mIslands[i]->hide();
				mSpareIslands.push_back(mIslands[i]);
				mIslands.erase(mIslands.begin()+i);
				--i;
			}
		}

		int step = -1 * std::min(0,static_cast<int>((position.z + mOffset) / mStepSize));
		generate(step + mGenerateSteps);
	}
	//-----------------------------------------------------------------------

	void Generator::generate(int dist)
	{
		while(dist > mGeneratedTo)
		{
			// make some islands!
			makeIsland(mIslandTypes[0], Vector3(Rand::get().genFloat(2,7)
				,-2,-mStepSize * mGeneratedTo - mOffset));
			makeIsland(mIslandTypes[0], Vector3(Rand::get().genFloat(-2,-7)
				,-2,-mStepSize * mGeneratedTo - mOffset));
			//mOffset += 1 * mGeneratedTo;
			++mGeneratedTo;
		}
	}
	//-----------------------------------------------------------------------

	void Generator::playerMoved(const Message& position)
	{
		if(const Vector3* pos = unpackMsg<Vector3>(position))
		{
			mPlayerPos = *pos;
			generateTo(mPlayerPos);
		}
	}
	//-----------------------------------------------------------------------
	
	Island* Generator::makeIsland(IslandSchematic* type, Vector3 pos, Real roll)
	{
		for(int i = 0; i < mSpareIslands.size(); ++i)
		{
			if(mSpareIslands[i]->mSchematic == type)
			{
				mIslands.push_back(mSpareIslands[i]);
				mIslands.back()->respawn(pos, roll);
				mSpareIslands.erase(mSpareIslands.begin() + i);
				std::cout<<"REUSED ONE!!!\n";
				return mIslands.back();
			}
		}

		mIslands.push_back(new Island(type, pos, roll, mRand.gen()));
		return mIslands.back();
	}
	//-----------------------------------------------------------------------
}
