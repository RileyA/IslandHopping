#ifndef IH_GENERATOR_H
#define IH_GENERATOR_H

#include "IH.h"
#include "IHIsland.h"
#include "IHIslandGrouping.h"
#include "OryxObject.h"
#include <set>

namespace IH
{
	class Generator : public Object
	{
	public:

		Generator(unsigned int mSeed,int steps = 5, Real stepSize = 20.f);
		virtual ~Generator();

		virtual void init();
		virtual void deinit();
		virtual void update(Real delta);

		virtual void generateTo(Vector3 position);
		virtual void generate(int step);

		void playerMoved(const Message& position);
		Island* makeIsland(String mesh, size_t id, Vector3 pos, Real roll = 0.f);
		void makeIslandGrouping(Vector3 pos);
		Colour getIdColor(size_t id){return mIslandIds[id];}

	protected:

		int mGenerateSteps;
		Real mStepSize;
		Real mOffset;
		int mGeneratedTo;
		Vector3 mPlayerPos;
		Rand mRand;

	private:

		std::vector<Island*> mIslands;
		std::vector<Island*> mSpareIslands;

		// island meshes available
		std::vector<String> mMeshes;

		// id -> color mapping
		std::map<size_t, Colour> mIslandIds;

		unsigned int mWeightTotal;

		struct IslandGrouping
		{
			IslandGrouping(unsigned int w, std::ifstream& file):weight(w)
			{
				String temp;
				unsigned int mesh  = 0;
				Vector3 pos = Vector3::ZERO;
				Vector3 offset = Vector3::ZERO;
				file >> temp;
				while(temp != "\\")
				{
					file >> mesh; // mesh index
					file >> temp; // [
					file >> pos.x; // x coord
					file >> pos.z; // z coord
					file >> temp; // ]
					file >> temp; // [
					file >> offset.x; // x coord
					file >> offset.z; // z coord
					file >> temp; // ]
					// save
					meshes.push_back(mesh);
					positions.push_back(pos);
					deviances.push_back(offset);

					// start back again
					file >> temp;  // | or '\'
				}
			}

			bool operator<(const IslandGrouping& other) const
			{
				// order by weight
				return weight < other.weight;
			}

			std::vector<Vector3> positions;
			std::vector<Vector3> deviances;
			std::vector<unsigned int> meshes;
			unsigned int weight;
		};

		// multiset of island groupings
		std::multiset<IslandGrouping> mIslandGroupings;
	};
}

#endif
