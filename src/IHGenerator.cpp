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
		//mIslandTypes.push_back(new IslandSchematic("mini_island.mesh",Colour(0.4f,0.8f,0.5f),0));
		mMeshes.push_back("mini_island.mesh");
		mMeshes.push_back("standard_island.mesh");
		mMeshes.push_back("big_island.mesh");
		mMeshes.push_back("long_island.mesh");
		mIslandIds[0] = Colour(0.4f,0.8f,0.5f);
		mIslandIds[1] = Colour(0.8f,0.3f,0.4f);
		mIslandIds[2] = Colour(0.4f,0.4f,0.4f);
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
			makeIsland(mMeshes[mRand.gen(0,mMeshes.size()-1)], mRand.gen(0,2), Vector3(mRand.genFloat(-5,5)
				,-2,-mStepSize * mGeneratedTo - mOffset));
			makeIsland(mMeshes[mRand.gen(0,mMeshes.size()-1)], mRand.gen(0,2), Vector3(mRand.genFloat(-10,-20)
				,-2,-mStepSize * mGeneratedTo - mOffset));
			makeIsland(mMeshes[mRand.gen(0,mMeshes.size()-1)], mRand.gen(0,2), Vector3(mRand.genFloat(10,20)
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
	
	Island* Generator::makeIsland(String mesh, size_t id, Vector3 pos, Real roll)
	{
		for(int i = 0; i < mSpareIslands.size(); ++i)
		{
			if(mSpareIslands[i]->mMeshName == mesh)
			{
				mIslands.push_back(mSpareIslands[i]);
				mIslands.back()->respawn(pos, id, roll);
				mSpareIslands.erase(mSpareIslands.begin() + i);
				return mIslands.back();
			}
		}

		mIslands.push_back(new Island(mesh, id, pos, roll, mRand.gen(), this));
		return mIslands.back();
	}
	//-----------------------------------------------------------------------
}
