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
		mScript = dynamic_cast<ChaiscriptSubsystem*>(mEngine->getSubsystem("ChaiscriptSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mPhys = dynamic_cast<BulletSubsystem*>(mEngine->getSubsystem("BulletSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), false);

		mGfx->setBackgroundColor(Colour(1.f,1.f,1.f));
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
	}
	//-----------------------------------------------------------------------
	
	void PlayState::deinit()
	{

	}
	//-----------------------------------------------------------------------
}
