#ifndef IH_GENERATOR_H
#define IH_GENERATOR_H

#include "IH.h"
#include "IHIsland.h"
#include "IHIslandGrouping.h"
#include "OryxObject.h"

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

		// types of groups of islands
		std::vector<IslandGrouping*> mIslandGroupings;

		// island meshes available
		std::vector<String> mMeshes;

		// id -> color mapping
		std::map<size_t, Colour> mIslandIds;
	};
}

#endif
