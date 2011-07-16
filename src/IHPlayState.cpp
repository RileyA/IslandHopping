#include "IHPlayState.h"
#include "IHScoreHandler.h"
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

		mGfx->setBackgroundColor(Colour(0.f,0.f,0.f));
		mGfx->getGUI()->setOverlayEnabled("HUD", true);
		mGfx->getGUI()->loadFont("UI");
		GUIScreen* scrn = mGfx->getGUI()->createScreen(mGfx->getMainViewport(),"TechDemo","Test");

		mCamera = new FPSCamera();
		mCamera->lockMotion();
		mCamera->mCamera->setFOV(55.f);
		ScoreHandler* scorehandler = new ScoreHandler();
		mPlayer = new IHPlayer("Player");
		Ocean* oc = new Ocean();
		Generator* g = new Generator(0, 10, 10.f);
		g->init();
		g->generate(5);

		mPlayer->getSignal("moved")->addListener(g->getSlot("playerPos"));
		mPlayer->getSignal("moved")->addListener(oc->getSlot("moved"));
		mPlayer->getSignal("moved")->addListener(mCamera->getSlot("move"));
		mPlayer->getSignal("moved")->addListener(scorehandler->getSlot("move"));
		createSignal("playerMove")->addListener(mPlayer->getSlot("move"));
		createSignal("jump")->addListener(mPlayer->getSlot("jump"));
		mJumpTimeout = 0.5f;
		//mCamera->getSignal("moved")->addListener(g->getSlot("playerPos"));
		//mCamera->getSignal("moved")->addListener(oc->getSlot("moved"));
	}
	//-----------------------------------------------------------------------
	
	void PlayState::update(Real delta)
	{
		mJumpTimeout-=delta;
		if(mInput->wasKeyPressed("KC_ESCAPE"))
			sendMessage(MessageAny<String>("kill"),"Engine");
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
		if((mInput->wasKeyPressed("KC_SPACE")||mInput->wasButtonPressed("MB_Left")) && mJumpTimeout < 0.f)
			getSignal("jump")->send(Vector3::ZERO);
		if(mInput->wasButtonPressed("MB_Right"))
			mGfx->takeScreenshot();
		Vector3 move =  mCamera->getDirection();
		/*move += mCamera->getDirection() * (mInput->isKeyDown("KC_W")
			- mInput->isKeyDown("KC_S"));
		move += mCamera->mCamera->getAbsoluteRight() * (mInput->isKeyDown("KC_D")
			- mInput->isKeyDown("KC_A"));*/

		if(move.y > 0.f)
			move.y = 0.f;
		else
			move.y *= 0.8f;
		move.normalize();
		//Plane pl = Plane(Vector3(0,1,0),0.f);
		//move = pl.projectVector(move) + move * 0.25f;
		//move *= 9;
		getSignal("playerMove")->send(move);
		//mCamera->mPosNode->setPosition(mPlayer->getPosition());
		
	}
	//-----------------------------------------------------------------------
	
	void PlayState::deinit()
	{
	}
}
