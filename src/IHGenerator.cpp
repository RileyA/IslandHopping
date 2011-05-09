#include "IHGenerator.h"
#include "OryxMessageAny.h"

namespace IH
{
	Generator::Generator(unsigned int mSeed, int steps, Real stepSize)
		:Object(),mStepSize(stepSize),mGenerateSteps(steps),mOffset(0.f),mRand(mSeed),
			mGeneratedTo(0)
	{
		createSlot("playerPos", this, &Generator::playerMoved);
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
		for(int i = 0; i < mIslands.size(); ++i)
			delete mIslands[i];
		mIslands.clear();
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
		int step = -1 * std::max(0,static_cast<int>((position.z - mOffset) / mStepSize));
		std::cout<<"generatin' "<<step<<"\n";
		generate(step + mGenerateSteps);
	}
	//-----------------------------------------------------------------------

	void Generator::generate(int dist)
	{
		while(dist > mGeneratedTo)
		{
			// make some islands!
			mIslands.push_back(new Island(mIslandTypes[0], Vector3(0,-2,-mStepSize * mGeneratedTo - mOffset)));

			mOffset += 0.1 + mOffset;
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
}
