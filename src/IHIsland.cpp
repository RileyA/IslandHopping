#include "IHIsland.h"

namespace IH
{
	Island::Island(IslandSchematic* schematic, Vector3 position, Real roll)
		:mSchematic(schematic)
	{
		OgreSubsystem* ogre = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>();
		mMesh = ogre->createMesh(mSchematic->mesh);
		mMesh->setPosition(position);
		mMesh->roll(roll);
		for(int i=0;i<2;++i)
			mMesh->setMaterialName(mMesh->getMaterial(i).cloneMe(),i);
		mMesh->getMaterial(1).setTexture(0,Colour(
			Rand::get().genFloat(0., 1.f),Rand::get().genFloat(0., 1.f),Rand::get().genFloat(0., 1.f)));
		ogre->getRootSceneNode()->addChild(mMesh);
		mActive = true;
	}
	//-----------------------------------------------------------------------
	
	Island::~Island()
	{
		OgreSubsystem* ogre = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>();
		ogre->destroySceneNode(mMesh);
	}
	//-----------------------------------------------------------------------
	
	void Island::score()
	{
		// do some scoring stuff here
		hide();
	}
	//-----------------------------------------------------------------------
	
	void Island::respawn(Vector3 position, Real roll)
	{
		mActive = true;
		mMesh->setVisible(true);
		mMesh->setPosition(position);
		mMesh->roll(roll);
	}
	//-----------------------------------------------------------------------
	
	void Island::hide()
	{
		mActive = false;
		mMesh->setVisible(false);
	}
	//-----------------------------------------------------------------------
}
