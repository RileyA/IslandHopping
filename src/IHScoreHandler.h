#ifndef IH_SCOREHANDLER_H
#define IH_SCOREHANDLER_H

#include "IH.h"
#include "OryxObject.h"

namespace IH
{
	class ScoreHandler : public Object
	{
	public:

		ScoreHandler();
		virtual ~ScoreHandler(){}

		void update(Real delta);
		void score(const Message& message);
		void move(const Message& message);
	
	private:

		long long mScore;
		unsigned long long mDistance;
		int mCombo;
		Real mStart;

	};
}

#endif
