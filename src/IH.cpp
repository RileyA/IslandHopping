#include "IH.h"
#include "IHMenuState.h"
#include "IHPlayState.h"

int main(int argc, char** argv)
{
	Logger::getPtr();
	TimeManager::getPtr();

	// create subsystems:
	ALSubsystem aSys = ALSubsystem();
	OgreSubsystem ogre = OgreSubsystem(1024,768,false);
	OISSubsystem ois = OISSubsystem();
	BulletSubsystem bull = BulletSubsystem();
	LuaSubsystem lua = LuaSubsystem();
	//ENetSubsystem enet = ENetSubsystem();

	// allocate engine and add subsystems
	Engine* eng = new Engine();
	eng->addSubsystem(&aSys);
	eng->addSubsystem(&ogre);
	eng->addSubsystem(&ois);
	eng->addSubsystem(&bull);
	eng->addSubsystem(&lua);
	//eng->addSubsystem(&enet);

	// initialize the engine
	eng->init();

	// add game state
	eng->addState(new IH::MenuState());

	// start up the engine
	eng->start();

	// delete the engine object
	delete eng;

	return 0;
}
