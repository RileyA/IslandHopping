#ifndef IH_LOBBYSTATE_H
#define IH_LOBBYSTATE_H


#include "IH.h"
#include "OryxGameState.h"

namespace IH
{
	class LobbyState : public GameState
	{
	public:

		LobbyState();
		virtual ~LobbyState();

		void init();
		void update(Real delta);
		void deinit();

		void clickedStart(const Message& m);
		void clickedBack(const Message& m);

		void addButton(Vector2 pos, String caption, String slot);

	private:

		OgreSubsystem* mGfx;
		OISSubsystem* mInput;
		ALSubsystem* mAudio;
		LuaSubsystem* mScript;
		GUIScreen* mScreen;
	
	};
}

#endif
