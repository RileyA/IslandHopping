#include "IHLobbyState.h"
#include "IHPlayState.h"
#include "OgreSubsystem/GUIButton.h"

namespace IH
{
	LobbyState::LobbyState()
		:GameState()
	{
		
	}

	LobbyState::~LobbyState()
	{
		
	}

	void LobbyState::init()
	{
		mGfx = dynamic_cast<OgreSubsystem*>(mEngine->getSubsystem("OgreSubsystem"));
		mAudio = dynamic_cast<ALSubsystem*>(mEngine->getSubsystem("ALSubsystem"));
		mInput = dynamic_cast<OISSubsystem*>(mEngine->getSubsystem("OISSubsystem"));
		mScript = dynamic_cast<LuaSubsystem*>(mEngine->getSubsystem("LuaSubsystem"));
		mInput->initInput(mGfx->getWindowHandle(), true);
		mGfx->setBackgroundColor(Colour(0.3f,0.5f,0.7f));

		GUI* gui = mGfx->getGUI();
		GUIScreen* scrn = mGfx->getGUI()->createScreen(mGfx->getMainViewport(),"TechDemo","Test");
		mScreen = scrn;
		scrn->getRootElement(0)->setAspectRatio(4,3);
		mGfx->getGUI()->setOverlayEnabled("HUD", false);

		// set up cursor
		GUIRectangle* rect = new GUIRectangle(scrn->getRootElement(8),"cursor","Mouse");
		rect->setScale(Vector2(16.f/1024.f,23.f/768.f));
		scrn->setCursor(rect);

		// hook up the input and the GUI
		EventHandler::getDestination("OISSubsystem")->getSignal("mouseMovedAbs")
			->addListener(scrn->getSlot("moveCursor"));
		EventHandler::getDestination("OISSubsystem")->getSignal("pressed_MB_Left")
			->addListener(scrn->getSlot("click"));
		EventHandler::getDestination("OISSubsystem")->getSignal("released_MB_Left")
			->addListener(scrn->getSlot("release"));

		StaticText* txt = new StaticText(scrn->getRootElement(0),
			"Title",0,20,Vector2(0.4f,0.2f),Vector2(0.2,1.f/20.f),"New Game");
		createSlot("play", this, &LobbyState::clickedStart);
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setAlign(GA_CENTER);

		rect = new GUIRectangle(scrn->getRootElement(0),"button","logo");
		rect->setScale(Vector2(327.f/1024.f,80.f/768.f));
		rect->setPosition(Vector2(-0.01f,0.9f));

		// labels
		txt = new StaticText(scrn->getRootElement(0),"modelabel",0,20,Vector2(0.4f,0.27f),
			Vector2(0.2,1.f/20.f),"Mode: ");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setScale(Vector2(327.f/1024.f,32.f/768.f));
		txt->setPosition(Vector2(0.2705f,0.492f));
		txt = new StaticText(scrn->getRootElement(0),"difflabel",0,20,Vector2(0.4f,0.27f),
			Vector2(0.2,1.f/20.f),"Difficulty: ");
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setScale(Vector2(327.f/1024.f,32.f/768.f));
		txt->setPosition(Vector2(0.2705f,0.552f));

		addButton(Vector2(412.f/1024.f, 0.4f), "Go!", "play");
		addButton(Vector2(412.f/1024.f, 0.48f), "< Classic >", "play");
		addButton(Vector2(412.f/1024.f, 0.54f), "< Easy >", "play");
		addButton(Vector2(412.f/1024.f, 0.62f), "Back", "play");



	}

	void LobbyState::update(Real delta)
	{
		if(mInput->isKeyDown("KC_ESCAPE"))
		{
			sendMessage(MessageAny<String>("end state"),"Engine");
		}
		if(mInput->wasKeyPressed("KC_HOME"))
			mInput->toggleMouseGrab();

	}

	void LobbyState::deinit()
	{

	}


	void LobbyState::clickedStart(const Message& m)
	{
		mEngine->addState(new PlayState());
		mEngine->endCurrentState();
	}


	void LobbyState::addButton(Vector2 pos, String caption, String slot)
	{
		GUIButton* rect = new GUIButton(mScreen->getRootElement(0), 0,"btnup");
		rect->setUpSprite("btnup");
		rect->setHoverSprite("btnhover");
		rect->setDownSprite("btndown");
		rect->setPosition(pos);
		rect->setScale(Vector2(200.f/1024.f, 50.f/768.f));

		StaticText* txt = new StaticText(mScreen->getRootElement(1),
			caption,0,20,Vector2(0.4f,0.2f),Vector2(0.2,1.f/20.f),caption);
		txt->setScale(Vector2(200.f/1024.f, 50.f/768.f));
		txt->setPosition(pos + Vector2(0,0.0125f));
		txt->setColour(Colour(1.f,1.f,1.f));
		txt->setAlign(GA_CENTER);
	}
}
