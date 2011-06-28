#include "IHIsland.h"
#include "IHWaveManager.h"
#include "IHGenerator.h"

namespace IH
{
	Island::Island(String mesh, size_t id, Vector3 position, Real roll, long long seed, Generator* parent)
		:mMeshName(mesh),mId(id),mGen(seed),mPosition(position),mParent(parent)
	{
		mWaves = dynamic_cast<WaveManager*>(Engine::getPtr()->getObject("WaveMgr"));
		OgreSubsystem* ogre = Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>();
		BulletSubsystem* bullet = Engine::getPtr()->getSubsystem("BulletSubsystem")->castType<BulletSubsystem>();

		mMesh = ogre->createMesh(mesh);
		mMesh->setPosition(position);

		for(int i=0;i<2;++i)
			mMesh->setMaterialName(mMesh->getMaterial(i).cloneMe(),i);
		mMesh->getMaterial(1).setTexture(0,mParent->getIdColor(id));
		ogre->getRootSceneNode()->addChild(mMesh);
		
		if(PhysicsShape* cached = bullet->getShape(mMeshName))
		{
			mCollide = bullet->createStatic(cached, position);
		}
		else
		{
			MeshData d;
			mMesh->getMeshData(d, false, false);
			mCollide = bullet->createStatic(bullet->createTrimeshShape(d, mMeshName), position);
		}

		mCollide->setOrientation(mMesh->getOrientation());
		bob = mGen.genFloat(0.125f,0.275f);
		bob_offset = mGen.genFloat(0.1f,2.f);
		mCollide->setUserData(this);
		mActive = true;
		mScored = false;
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
			Vector3 normal = Vector3(0,0.45f,0);
			Real disp = mWaves->getDisplacement(Vector2(mPosition.x,mPosition.z),normal);
			normal.normalize();
			mMesh->setPosition(mPosition + Vector3(0,1,0) * sin(TimeManager::getPtr()->getTimeDecimal()*1.25f+bob_offset)
				* bob + Vector3(0,disp-0.75f,0));
			if(normal == Vector3::UNIT_Y)
				mMesh->setOrientation(Quaternion::IDENTITY);
			else
				mMesh->setOrientation(Vector3::UNIT_Y.getRotationTo(normal) * Quaternion::IDENTITY);

			mCollide->setOrientation(mMesh->getOrientation());
			mCollide->setPosition(mMesh->getPosition());
		}
	}
	//-----------------------------------------------------------------------
	
	void Island::score()
	{
		// do some scoring stuff here
		if(mActive && !mScored)
		{
			mScored = true;
			String fx = "../media/audio/hit_green.wav";
			if(mId == 1)
				fx = "../media/audio/hit_01.wav";
			else if(mId == 2)
				fx = "../media/audio/hit_grey.wav";

			dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D(fx);
			mMesh->getMaterial(1).setTexture(0, Colour(0.4,0.5,0.9));
			//hide();
		}
	}
	//-----------------------------------------------------------------------
	
	void Island::respawn(Vector3 position, size_t id, Real roll)
	{
		mId = id;
		mActive = true;
		mScored = false;
		mMesh->setVisible(true);
		mMesh->setPosition(position);
		mPosition = position;
		mMesh->setOrientation(Quaternion::IDENTITY);
		mMesh->roll(roll);
		mCollide->setPosition(position);
		mCollide->setOrientation(mMesh->getOrientation());
		mMesh->getMaterial(1).setTexture(0,mParent->getIdColor(mId));
	}
	//-----------------------------------------------------------------------
	
	void Island::hide()
	{
		mActive = false;
		mMesh->setVisible(false);
		mCollide->setPosition(Vector3(0,-1000,0));
	}
	//-----------------------------------------------------------------------
}
