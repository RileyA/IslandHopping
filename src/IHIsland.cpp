#include "IHIsland.h"

namespace IH
{
	Island::Island(IslandSchematic* schematic, Vector3 position, Real roll, long long seed)
		:mSchematic(schematic),mGen(seed),mPosition(position)
	{
		OgreSubsystem* ogre = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>();
		BulletSubsystem* bullet = Engine::getPtr()->getSubsystem("BulletSubsystem")->castType<BulletSubsystem>();
		mMesh = ogre->createMesh(mSchematic->mesh);
		mMesh->setPosition(position);
		mMesh->roll(position.x);
		for(int i=0;i<2;++i)
			mMesh->setMaterialName(mMesh->getMaterial(i).cloneMe(),i);
		mMesh->getMaterial(1).setTexture(0,Colour(
			Rand::get().genFloat(0., 1.f),Rand::get().genFloat(0., 1.f),Rand::get().genFloat(0., 1.f)));
		ogre->getRootSceneNode()->addChild(mMesh);
		
		if(PhysicsShape* cached = bullet->getShape(mSchematic->mesh))
		{
			mCollide = bullet->createStatic(cached, position);
		}
		else
		{
			MeshData d;
			mMesh->getMeshData(d, false, false);
			mCollide = bullet->createStatic(bullet->createTrimeshShape(d, mSchematic->mesh), position);
		}
		mCollide->setOrientation(mMesh->getOrientation());
		bob = mGen.genFloat(0.125f,0.275f);
		bob_offset = mGen.genFloat(0.1f,2.f);
		mActive = true;
	}
	//-----------------------------------------------------------------------
	
	Island::~Island()
	{
		OgreSubsystem* ogre = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>();
		ogre->destroySceneNode(mMesh);
	}
	//-----------------------------------------------------------------------

	void Island::update(Real delta)
	{
		if(mActive)
		{
			mMesh->setPosition(mPosition + Vector3(0,1,0) * sin(TimeManager::getPtr()->getTimeDecimal()*1.25f+bob_offset) * bob);
		}
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
		mPosition = position;
		mMesh->setOrientation(Quaternion::IDENTITY);
		mMesh->roll(roll);
		mCollide->setPosition(position);
		mCollide->setOrientation(mMesh->getOrientation());
	}
/bin/bash: s: command not found
	
	void Island::hide()
	{
		mActive = false;
		mMesh->setVisible(false);
		mCollide->setPosition(Vector3(0,-1000,0));
	}
	//-----------------------------------------------------------------------
}
