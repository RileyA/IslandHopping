#ifndef IH_PLAYER_H
#define IH_PLAYER_H

#include "IH.h"
#include "Oryx.h"
#include "OryxEngine.h"
#include "OryxObject.h"

namespace IH
{
	class IHPlayer : public Object
	{
	public:

		IHPlayer(String name);
		virtual ~IHPlayer();
		void update(Real delta);
		Vector3 getPosition();
		void setMove(const Message& move);
		void jump(const Message& j);
	
	private:

		static const Real TIMESTEP = 0.01f;

		CharPrimitive* mController;
		Real mAccumulator;
		Real mInterpolation;
		Vector3 mPositionLast;

		Vector3 mMove;
		Real mGravityFactor;
		bool mJumping;
		Real mOnSolidGround;

		Real mSpeed;

		unsigned int mExtraJumps;

	};
}

#endif
