#ifndef IH_PLAY_STATE_H
#define IH_PLAY_STATE_H

#include "IH.h"
#include "IHPlayer.h"
#include "OryxGameState.h"
#include "IHIsland.h"
#include "IHOcean.h"
#include "FPSCamera.h"

namespace IH
{
	class PlayState : public GameState
	{
	public:

		PlayState();

		virtual void init();
		virtual void update(Real delta);
		virtual void deinit();

	private:
	
		FPSCamera* mCamera;

		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;
		LuaSubsystem* mScript;
		BulletSubsystem* mBullet;

		IHPlayer* mPlayer;
		Real mJumpTimeout;

	};
}

#endif
