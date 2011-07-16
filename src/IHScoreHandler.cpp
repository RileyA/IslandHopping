#ifndef IH_SCORE_HANDLER
#define IH_SCORE_HANDLER

#include "IHScoreHandler.h"
#include "IHWaveManager.h"
#include "IHPlayState.h"
#include "IHLobbyState.h"
#include "IHMenuState.h"

namespace IH
{
	ScoreHandler::ScoreHandler()
		:Object("Default","ScoreHandler")// name it here, we'll assume one exists...
		,mScore(0)
		,mCombo(0)
		,mDistance(0)
	{
		createSlot("scored", this, &ScoreHandler::score);
		createSlot("move", this, &ScoreHandler::move);
		Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
			"ScoreText", "Score:0");
		Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
			"DistanceText", "Distance:0");
		Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
			"IslesText", "Combo:0");
		Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
			"TimeText", "Time:00:00:00");
		Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
			"MultiText", "Multiplier:0x");
		mStart = TimeManager::getPtr()->getTimeDecimal();
	}

	void ScoreHandler::update(Real delta)
	{
		Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()
			->getGUI()->setOverlayText("TimeText",String("Time:") 
			+ TimeManager::getPtr()->getTimestampMilli(mStart));
	}

	void ScoreHandler::score(const Message& message)
	{
		if(const size_t* id = unpackMsg<size_t>(message))
		{
			String fx = "../media/audio/hit_green.wav";
			if(*id == 0)
			{
				mScore += 500;
				mScore += 250 * std::max(mCombo,0);
				if(mCombo < 0)
					mCombo = 1;
				else
					++mCombo;
			}
			else if(*id == 1)
			{
				mScore -= 500;
				mScore += 250 * std::min(mCombo,0);
				fx = "../media/audio/hit_01.wav";
				if(mCombo > 0)
				{
					mCombo = -1;
					dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D(
						"../media/audio/hit_02.wav");
				}
				else
					--mCombo;
			}
			else if(*id == 2)
			{
				mScore += 50;
				fx = "../media/audio/hit_grey.wav";
				 if(mCombo > 0)
					dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D(
						"../media/audio/hit_02.wav");
				mCombo = 0;
			}

			// play a sound effect
			dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D(fx);

			// update HUD
			Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
				"ScoreText", "Score:"+StringUtils::toString(mScore));
			Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
				"IslesText", "Combo:"+StringUtils::toString(abs(mCombo)));
			Colour combocol = Colour(0.7,0.8,0.9);
			if(mCombo > 0)
				combocol = Colour(0.6f,0.9f,0.775f);
			else if(mCombo < 0)
				combocol = Colour(0.9f,0.4f,0.3f);
			Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayTextColour(
				"IslesText", combocol);
		
			// TODO Add score pops
		}
	}
	//-----------------------------------------------------------------------
	
	void ScoreHandler::move(const Message& message)
	{
		if(const Vector3* pos = unpackMsg<Vector3>(message))
		{
			mDistance = std::max(static_cast<unsigned long long>(
				(pos->z < 0 ? -pos->z : 0)), mDistance);
			Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>()->getGUI()->setOverlayText(
				"DistanceText", "Distance:"+StringUtils::toString(abs(mDistance)));
			Vector2 waterPlanePos = Vector2(pos->x, pos->z);
			if(pos->y+4 < static_cast<WaveManager*>(Engine::getPtr()->getObject("WaveMgr"))->getDisplacement(waterPlanePos))
			{
				// failure!
				// just start a new state for the moment
				//Engine::getPtr()->addState(new PlayState());
				//Engine::getPtr()->addState(new LobbyState());
				Engine::getPtr()->addState(new MenuState());
				Engine::getPtr()->endCurrentState();
			}
		}
	}
}

#endif
