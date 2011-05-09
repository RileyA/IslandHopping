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

	protected:

		int mGenerateSteps;
		Real mStepSize;
		Real mOffset;
		int mGeneratedTo;
		Vector3 mPlayerPos;
		Rand mRand;

	private:

		std::vector<Island*> mIslands;

		// type of individual islands
		std::vector<IslandSchematic*> mIslandTypes;
		
		// types of groups of islands
		std::vector<IslandGrouping*> mIslandGroupings;

	};
}

#endif
