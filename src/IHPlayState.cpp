#include "IHPlayState.h"
#include "OryxEventHandler.h"
#include "OryxMessageAny.h"

namespace IH
{
	PlayState::PlayState(){}
	//-----------------------------------------------------------------------
	
	void PlayState::init()
	{
		mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
		mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), true);

		mCamera = new FPSCamera();

		mGfx->setBackgroundColor(Colour(0.2f,0.2f,0.2f));
		mIslandTypes.push_back(new IslandSchematic());
		mIslands.push_back(new Island(mIslandTypes.back(),Vector3(0,-2,-7)));
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
