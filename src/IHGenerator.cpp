#include "IHGenerator.h"
#include "OryxMessageAny.h"
#include "OryxBucket.h"

namespace IH
{
	Generator::Generator(unsigned int mSeed, int steps, Real stepSize)
		:Object(),mStepSize(stepSize),mGenerateSteps(steps),mOffset(0.f),mRand(mSeed),
			mGeneratedTo(2),mWeightTotal(0)
	{
		createSlot("playerPos", this, &Generator::playerMoved);
		if(!Engine::getPtr()->getBucket("Islands"))
			Engine::getPtr()->createBucket("Islands")->setTemporary(true);
		//mIslandTypes.push_back(new IslandSchematic("mini_island.mesh",Colour(0.4f,0.8f,0.5f),0));

		std::ifstream file;
		file.open("IslandGroups.ih");

		if(file.is_open())
		{	
			unsigned int weight;
			while(file.good())
			{
				file >> weight;
				if(file.good()) 
				{
					mIslandGroupings.insert(IslandGrouping(weight, file));
					mWeightTotal += weight;
				}
			}
			file.close();
		}

		std::ifstream meshfile;
		meshfile.open("Islands.ih");
		if(meshfile.is_open())
		{
			String temp;
			while(meshfile.good())
			{
				meshfile >> temp;
				mMeshes.push_back(temp);
			}
			meshfile.close();
		}

		std::ifstream cfile;
		cfile.open("IslandColors.ih");
		if(cfile.is_open())
		{
			unsigned int id;
			Colour c;
			while(cfile.good())
			{
				cfile >> id;
				cfile >> c.r;
				cfile >> c.g;
				cfile >> c.b;
				mIslandIds[id] = c;
			}
			meshfile.close();
		}

		// make a start island
		makeIsland("runway_island.mesh", 2000, Vector3(0,-2.125,0), 0.f);
		//generateTo(Vector3(0,0,-60));
	}
	//-----------------------------------------------------------------------
	
	Generator::~Generator()
	{

	}
	//-----------------------------------------------------------------------
	
	void Generator::init()
	{
	}
	//-----------------------------------------------------------------------
	
	void Generator::deinit()
	{
		// islands are Oryx::Objects, so they get deleted by the engine
		mIslands.clear();
		mSpareIslands.clear();
		mIslandGroupings.clear();
	}
	//-----------------------------------------------------------------------
	
	void Generator::update(Real delta)
	{
		// ...
	}
	//-----------------------------------------------------------------------
	
	void Generator::generateTo(Vector3 position)
	{
		// clear old islands first
		for(int i = 0; i < mIslands.size(); ++i)
		{
			if(!mIslands[i]->mActive)
			{
				mSpareIslands.push_back(mIslands[i]);
				mIslands.erase(mIslands.begin()+i);
				--i;
			}
			else if(mIslands[i]->mMesh->getPosition().z > position.z + mStepSize * 5)
			{
				mIslands[i]->hide();
				mSpareIslands.push_back(mIslands[i]);
				mIslands.erase(mIslands.begin()+i);
				--i;
			}
		}

		int step = -1 * std::min(0,static_cast<int>((position.z + mOffset) / mStepSize));
		generate(step + mGenerateSteps);
	}
	//-----------------------------------------------------------------------

	void Generator::generate(int dist)
	{
		while(dist > mGeneratedTo)
		{
			// make some islands!
			makeIslandGrouping(Vector3(0,-2.125,-mStepSize * mGeneratedTo - mOffset));
			makeIslandGrouping(Vector3(-10,-2.125,-mStepSize * mGeneratedTo - mOffset));
			makeIslandGrouping(Vector3(10,-2.125,-mStepSize * mGeneratedTo - mOffset));
			//mOffset += 1 * mGeneratedTo * 0.5f;
			++mGeneratedTo;
		}
	}
	//-----------------------------------------------------------------------

	void Generator::playerMoved(const Message& position)
	{
		if(const Vector3* pos = unpackMsg<Vector3>(position))
		{
			mPlayerPos = *pos;
			generateTo(mPlayerPos);
		}
	}
	//-----------------------------------------------------------------------
	
	Island* Generator::makeIsland(String mesh, size_t id, Vector3 pos, Real roll)
	{
		for(int i = 0; i < mSpareIslands.size(); ++i)
		{
			if(mSpareIslands[i]->mMeshName == mesh)
			{
				mIslands.push_back(mSpareIslands[i]);
				mIslands.back()->respawn(pos, id, roll);
				mSpareIslands.erase(mSpareIslands.begin() + i);
				return mIslands.back();
			}
		}

		mIslands.push_back(new Island(mesh, id, pos, roll, mRand.gen(), this));
		return mIslands.back();
	}
	//-----------------------------------------------------------------------

	void Generator::makeIslandGrouping(Vector3 pos)
	{
		long long seed = mRand.gen(0,mWeightTotal-1);
		IslandGrouping* group = 0;
		for(std::multiset<IslandGrouping>::iterator it = mIslandGroupings.begin(); 
			it != mIslandGroupings.end(); ++it)
		{
			if((*it).weight >= seed)
			{
				group = const_cast<IslandGrouping*>(&(*it));// yucks
				break;
			}
			seed -= (*it).weight;
		}

		if(!group)
			return;

		// now actually create it
		for(int i = 0; i < group->meshes.size(); ++i)
		{
			makeIsland(mMeshes[group->meshes[i]], mRand.gen(0,2), pos + group->positions[i] +
				Vector3(mRand.genFloat(group->deviances[i].x), mRand.genFloat(-0.05f,-0.3f), mRand.genFloat(group->deviances[i].z)));
		}
	}
	//-----------------------------------------------------------------------
}
