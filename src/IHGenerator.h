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
		//std::vector<IslandGrouping*> mIslandGroupings;

	};
}
