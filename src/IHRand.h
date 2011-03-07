#ifndef IH_RAND_H
#define IH_RAND_H

#include "IH.h"
#include <cstdlib>

namespace IH
{
	// so this can be switched easily
	typedef unsigned long long ruint;

	/** Very simple pseudorandom number generator
	 *		TODO: Integrate this into the engine itself? */
	class Rand
	{
	public:

		/** Creates a random number generator
		 *		@param seed The seed to use (or 0 for time(0)) */
		Rand(ruint seed = 0);

		/** Destructor */
		virtual ~Rand(){}
		
		/** Gets a (pseudo) random number */
		virtual ruint gen();

		/** Gets a random number within a range */
		ruint gen(int min, int max);

		/** Gets a random float */
		float genFloat(float min, float max);
	
		/** Gets a random character */
		char genChar();

		// Maximum value this can return
		static const ruint IHRAND_MAX;

	private:

		// The seed
		ruint mSeed;

	};
}

#endif
