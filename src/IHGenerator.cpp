#include "IHGenerator.h"
#include "OryxMessageAny.h"

namespace IH
{
	Generator::Generator(unsigned int mSeed, int steps, Real stepSize)
		:mStepSize(stepSize),mGenerateSteps(steps),mOffset(0.f),mRand(mSeed),
			mGeneratedTo(0)
	{}
	//-----------------------------------------------------------------------
	
	Generator::~Generator()
	{

	}
	//-----------------------------------------------------------------------
	
	void Generator::init()
	{
		createSlot("playerPos", this, &Generator::playerMoved);
	}
	//-----------------------------------------------------------------------
	
	void Generator::deinit()
	{

	}
	//-----------------------------------------------------------------------
	
	void Generator::update(Real delta)
	{
	}
	//-----------------------------------------------------------------------
	
	void Generator::generateTo(Vector3 position)
	{
		int step = position.z / mStepSize;
		generate(step + mGenerateSteps);
	}
	//-----------------------------------------------------------------------

	void Generator::generate(int dist)
	{
		while(dist > mGeneratedTo)
		{
			// make some islands!
			++mGeneratedTo;
		}
	}
	//-----------------------------------------------------------------------

	void Generator::playerMoved(const Message& position)
	{
		if(const MessageAny<Vector3>* pos = message_cast<Vector3>(position))
		{
			mPlayerPos = pos->data;
			generateTo(mPlayerPos);
		}
	}
	//-----------------------------------------------------------------------
}
