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
		mScript = dynamic_cast<LuaSubsystem*>(mEngine->getSubsystem("LuaSubsystem"));
		mBullet = dynamic_cast<BulletSubsystem*>(mEngine->getSubsystem("BulletSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), true);
		mBullet->startSimulation();

		mCamera = new FPSCamera();
		//mCamera->unlockMotion();
		mPlayer = new IHPlayer("Player");//new CharPrimitive(mBullet, Vector3(0,15,0), 0.375f*1.5f, 0.75f*1.5f);

		mGfx->setBackgroundColor(Colour(0.f,0.f,0.f));

		Ocean* oc = new Ocean();

		Generator* g = new Generator(0, 10, 16.f);
		g->init();
		g->generate(5);

		mPlayer->getSignal("moved")->addListener(g->getSlot("playerPos"));
		//mCamera->getSignal("moved")->addListener(g->getSlot("playerPos"));
		mPlayer->getSignal("moved")->addListener(oc->getSlot("moved"));
		//mCamera->getSignal("moved")->addListener(oc->getSlot("moved"));
		createSignal("playerMove")->addListener(mPlayer->getSlot("move"));
		createSignal("jump")->addListener(mPlayer->getSlot("jump"));
		//
		mPlayer->getSignal("moved")->addListener(mCamera->getSlot("move"));

	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		if(mInput->wasKeyPressed("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
		if(mInput->wasKeyPressed("KC_SPACE")||mInput->wasButtonPressed("MB_Left"))
			getSignal("jump")->send(Vector3::ZERO);
		Vector3 move =  mCamera->getDirection();
		/*move += mCamera->getDirection() * (mInput->isKeyDown("KC_W")
			- mInput->isKeyDown("KC_S"));
		move += mCamera->mCamera->getAbsoluteRight() * (mInput->isKeyDown("KC_D")
			- mInput->isKeyDown("KC_A"));*/
		move.normalize();
		if(move.y > 0.f)
			move.y = 0.f;
		//Plane pl = Plane(Vector3(0,1,0),0.f);
		//move = pl.projectVector(move) + move * 0.25f;
		move *= 17;
		getSignal("playerMove")->send(move);
		//mCamera->mPosNode->setPosition(mPlayer->getPosition());
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
