#ifndef IH_PLAY_STATE_H
#define IH_PLAY_STATE_H

#include "IH.h"
#include "OryxGameState.h"

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
	
		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;
		ChaiscriptSubsystem* mScript;
		BulletSubsystem* mPhys;

	};
}

#endif
