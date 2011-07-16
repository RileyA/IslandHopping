#include "IHMenuState.h"
#include "IHPlayState.h"
#include "IHLobbyState.h"
#include "OgreSubsystem/Camera.h"
#include "OryxEventHandler.h"
#include "OryxMessageAny.h"
#include "Oyster.h"

namespace IH
{
	MenuState::MenuState(){}
	//-----------------------------------------------------------------------
	
	void MenuState::init()
	{
		mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
		mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), true);
		mGfx->getGUI()->setOverlayEnabled("HUD", false);

		mOyster = new Oyster::Oyster(1024,768);

		mOyster->createAtlas("atlas1", "TechDemo.oyster");
		Oyster::Batch* b = mOyster->createBatch("batch1", "atlas1");
		b->createLayer(1)->createRectangle(0, 0, 50, 50)->setColor(255,0,0,128);
		b->update();

		ScreenMesh* s = mGfx->createScreenMesh("UITEST"); //sleep(1); 
		s->buildMesh(b->getMesh().getVertexCount(), b->getMesh().getFaceCount(), b->getMesh().getVertices(), b->getMesh().getTexcoords(), 
			b->getMesh().getColors(), b->getMesh().getIndices());

		GUI* gui = mGfx->getGUI();
		GUIScreen* scrn = mGfx->getGUI()->createScreen(mGfx->getMainViewport(),"TechDemo","Test");

		mGfx->setBackgroundColor(Colour(0.3f,0.5f,0.7f));

		// set up menu
		// TODO: make an xml (or some format) gui format
		// also long-term TODO: replace Gorilla with a custom solution

		scrn->getRootElement(0)->setAspectRatio(4,3);
		StaticText* txt = new StaticText(scrn->getRootElement(0),"Title",0,20,Vector2(0.4f,0.2f),Vector2(0.2,1.f/20.f),"Island Hopping");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setAlign(GA_CENTER);
		//txt->setHoverColour(Colour(1.f,0.f,0.f));

		GUIRectangle* rect = new GUIRectangle(scrn->getRootElement(1),"cursor","Mouse");
		rect->setScale(Vector2(16.f/1024.f,23.f/768.f));
		scrn->setCursor(rect);

		rect = new GUIRectangle(scrn->getRootElement(0),"button","logo");
		rect->setScale(Vector2(327.f/1024.f,80.f/768.f));
		rect->setPosition(Vector2(-0.01f,0.9f));

		// play button
		txt = new StaticText(scrn->getRootElement(0),"PlayBtn",0,20,Vector2(0.4f,0.2f),
			Vector2(0.2,1.f/20.f),"New Game");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setHoverColour(Colour(0.7f,0.8f,0.9f));
		txt->setAlign(GA_CENTER);
		txt->setScale(Vector2(327.f/1024.f,32.f/768.f));
		txt->setPosition(Vector2(0.3405f,0.432f));
		txt->getSignal("clicked")->addListener(createSlot("clickedPlay",
			this,&MenuState::clickedPlay));
		// high score button
		txt = new StaticText(scrn->getRootElement(0),"ScoreBtn",0,20,Vector2(0.4f,0.2f),
			Vector2(0.2,1.f/20.f),"High Scores");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setHoverColour(Colour(0.7f,0.8f,0.9f));
		txt->setAlign(GA_CENTER);
		txt->setScale(Vector2(327.f/1024.f,32.f/768.f));
		txt->setPosition(Vector2(0.3405f,0.502f));
		txt->getSignal("clicked")->addListener(createSlot("clickedHS",
			this,&MenuState::clickedHS));
		// settings button
		txt = new StaticText(scrn->getRootElement(0),"SettingsBtn",0,20,Vector2(0.4f,0.2f),
			Vector2(0.2,1.f/20.f),"Settings");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setHoverColour(Colour(0.7f,0.8f,0.9f));
		txt->setAlign(GA_CENTER);
		txt->setScale(Vector2(327.f/1024.f,32.f/768.f));
		txt->setPosition(Vector2(0.3405f,0.572f));
		txt->getSignal("clicked")->addListener(createSlot("clickedSettings",
			this,&MenuState::clickedSettings));
		// quit button
		txt = new StaticText(scrn->getRootElement(0),"QuitBtn",0,20,Vector2(0.4f,0.2f),
			Vector2(0.2,1.f/20.f),"Quit");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setHoverColour(Colour(0.7f,0.8f,0.9f));
		txt->setAlign(GA_CENTER);
		txt->setScale(Vector2(327.f/1024.f,32.f/768.f));
		txt->setPosition(Vector2(0.3405f,0.642f));
		txt->getSignal("clicked")->addListener(createSlot("clickedQuit",
			this,&MenuState::clickedQuit));

		EventHandler::getDestination("OISSubsystem")->getSignal("mouseMovedAbs")
			->addListener(scrn->getSlot("moveCursor"));
		EventHandler::getDestination("OISSubsystem")->getSignal("pressed_MB_Left")
			->addListener(scrn->getSlot("click"));
		EventHandler::getDestination("OISSubsystem")->getSignal("released_MB_Left")
			->addListener(scrn->getSlot("release"));
	}
	//-----------------------------------------------------------------------
	
	void MenuState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
		{
			//mEngine->addState(new PlayState());	
			sendMessage(MessageAny<String>("end state"),"Engine");
			//sendMessage(MessageAny<String>("kill"),"Engine");
		}
		if(mInput->isKeyDown("KC_Q"))
		{
			mEngine->addState(new PlayState());	
			sendMessage(MessageAny<String>("end state"),"Engine");
		}
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();
	}
	//-----------------------------------------------------------------------
	
	void MenuState::deinit()
	{

	}
	//-----------------------------------------------------------------------

	void MenuState::clickedPlay(const Message& m)
	{
		mEngine->addState(new PlayState());	
		sendMessage(MessageAny<String>("end state"),"Engine");
	}
	//-----------------------------------------------------------------------

	void MenuState::clickedHS(const Message& m)
	{
		std::cout<<"High Scores\n";
	}
	//-----------------------------------------------------------------------
	
	void MenuState::clickedSettings(const Message& m)
	{
		std::cout<<"Settings\n";
	}
	//-----------------------------------------------------------------------
	
	void MenuState::clickedQuit(const Message& m)
	{
		mEngine->shutdown();
	}
	//-----------------------------------------------------------------------
}
