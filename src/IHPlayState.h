#ifndef IH_PLAY_STATE_H
#define IH_PLAY_STATE_H

#include "IH.h"
#include "OryxGameState.h"
#include "IHIsland.h"
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
	
		std::vector<IslandSchematic*> mIslandTypes;
		std::vector<Island*> mIslands;

		FPSCamera* mCamera;

		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;

	};
}

#endif
