#include "IHOcean.h"

namespace IH
{
	Ocean::Ocean()
	{
		mWaveMgr = new WaveManager(1.0f);
		//mWaveMgr->pushWave(Vector2(5,0), 7, 2);
		//mWaveMgr->pushWave(Vector2(5,-10), Vector2(0,-90), 10, 3);
		//mWaveMgr->pushWave(Vector2(0,-50), Vector2(0,0), 12, 5);
		//mWaveMgr->pushWave(Vector2(-10,-130), Vector2(0,0), 10, 4);
		createSlot("moved",this,&Ocean::moved);
		mTimer = 0.3f;
		Real SCALE_X = 120.f;
		Real SCALE_Y = 120.f;
		
		Real stepX = SCALE_X / VERTS_X;
		Real stepY = SCALE_Y / VERTS_Y;
		Real startX = -SCALE_X / 2;
		Real startY = -SCALE_Y / 2 - 20.f;

		mData.addTexcoordSet();

		for(int i = 0; i < VERTS_X; ++i)
		{
			for(int j = 0; j < VERTS_Y; ++j)
			{
				Vector3 vpos;
				vpos.x = startX + stepX * i;
				vpos.y = -2.25f;//Rand::get().genFloat(-2.25,-3.f);
				vpos.z = startY + stepY * j;

				mData.vertices.push_back(vpos.x);
				mData.vertices.push_back(vpos.y);
				mData.vertices.push_back(vpos.z);

				mData.texcoords[0].push_back(i*1.2f);
				mData.texcoords[0].push_back(j*1.2f);

				mData.normals.push_back(0.f);
				mData.normals.push_back(1.f);
				mData.normals.push_back(0.f);
				
				if(j < VERTS_Y-1 && i < VERTS_X-1)
				{
					mData.indices.push_back(i*VERTS_Y + j);
					mData.indices.push_back(i*VERTS_Y + j + 1);
					mData.indices.push_back((i+1)*VERTS_Y + j);
					mData.indices.push_back((i+1)*VERTS_Y + j);
					mData.indices.push_back(i*VERTS_Y + j + 1);
					mData.indices.push_back((i+1)*VERTS_Y + j + 1);
				}
			}
		}

		OgreSubsystem * ogre =Engine::getPtr()->getSubsystem("OgreSubsystem")->castType<OgreSubsystem>(); 

		mMesh = ogre->createMesh(mData);
		ogre->getRootSceneNode()->addChild(mMesh);
		mMesh_outer = ogre->createMesh("ocean_outer.mesh");
		mMesh->addChild(mMesh_outer);
		mMesh_outer->setPosition(Vector3(0.f,0.f,0.f));
	}

	Ocean::~Ocean()
	{

	}

	void Ocean::update(Real delta)
	{
		mTimer -= delta;
		if(mTimer <= 0.f)
		{
			//mTimer = 0.2f;
			for(int i = 1; i < mData.vertices.size(); i+=3)
			{
				Vector3 n = Vector3(0,0.45f,0);
				mData.vertices[i] = mWaveMgr->getDisplacementFast(Vector2(
					mMesh->getPosition().x+mData.vertices[i-1],
					mMesh->getPosition().z+mData.vertices[i+1]),n)-3.1f;
				//n = n*-1;
				n.normalize();
				mData.normals[i-1] = n.x;
				mData.normals[i] = n.y;
				mData.normals[i+1] = n.z;
			}

			mMesh->updatePositions(mData);
			mMesh->updateNormals(mData);
		}
	}

	void Ocean::moved(const Message& m)
	{
		// ...
		mWaveMgr->playerMoved(m);
		if(const Vector3* pos = unpackMsg<Vector3>(m))
		{
			Vector3 nw = *pos;
			nw.y = 0;
			//nw.z = (((int)nw.z) / 25) * 25;
			nw.x = 0;
			nw.z = floor(nw.z);
			mMesh->setPosition(nw);
			//update(0.f);
		}
	}
}
