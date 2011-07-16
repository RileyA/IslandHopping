#ifndef IH_MENU_STATE_H
#define IH_MENU_STATE_H

#include "IH.h"
#include "OryxGameState.h"

namespace Oyster
{
	class Oyster;
}

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
		void clickedHS(const Message& m);
		void clickedSettings(const Message& m);
		void clickedQuit(const Message& m);

	private:
	
		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;
		Oyster::Oyster* mOyster;

	};
}

#endif
