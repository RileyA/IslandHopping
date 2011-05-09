#include "IHPlayState.h"
#include "OryxEventHandler.h"
#include "OryxMessageAny.h"
#include "IHGenerator.h"

namespace IH
{
	PlayState::PlayState(){}
	//-----------------------------------------------------------------------
	
	void PlayState::init()
	{
		mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
		mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), false);

		mCamera = new FPSCamera();

		mGfx->setBackgroundColor(Colour(0.12f,0.12f,0.12f));
		/*mIslandTypes.push_back(new IslandSchematic());

		for(int i=-1;i<2;++i)
		{
			mIslands.push_back(new Island(mIslandTypes.back(),Vector3(i*5,-2,-7),i*30.f));
			mIslands.push_back(new Island(mIslandTypes.back(),Vector3(i*5,-2,-12),i*30.f));
			mIslands.push_back(new Island(mIslandTypes.back(),Vector3(i*5,-2,-17),i*30.f));
			mIslands.push_back(new Island(mIslandTypes.back(),Vector3(i*5,-2,-22),i*30.f));
		}*/

		Generator* g = new Generator(0, 5, 10.f);
		g->init();
		g->generate(5);
		mCamera->getSignal("moved")->addListener(g->getSlot("playerPos"));
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(mInput->wasKeyPressed("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
	}
	//-----------------------------------------------------------------------
	
	void PlayState::deinit()
	{
		for(int i=0;i<mIslandTypes.size();++i)
			delete mIslandTypes[i];
		mIslandTypes.clear();

		for(int i=0;i<mIslands.size();++i)
			delete mIslands[i];
		mIslands.clear();
	}
	//-----------------------------------------------------------------------
}
