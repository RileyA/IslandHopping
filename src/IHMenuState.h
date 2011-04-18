#ifndef IH_MENU_STATE_H
#define IH_MENU_STATE_H

#include "IH.h"
#include "OryxGameState.h"

namespace IH
{
	class MenuState : public GameState
	{
	public:

		MenuState();

		virtual void init();
		virtual void update(Real delta);
		virtual void deinit();

		void clickedPlay(const Message& m);

	private:
	
		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;

	};
}

#endif
