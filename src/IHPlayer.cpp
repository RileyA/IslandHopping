#include "IHPlayer.h"
#include "OryxMessageAny.h"
#include "BulletSubsystem/CharPrimitive.h"
#include "IHIsland.h"

namespace IH
{
	IHPlayer::IHPlayer(String name)
		//:mName(name)
	{
		createSlot("move", this, &IHPlayer::setMove);
		createSlot("jump", this, &IHPlayer::jump);
		createSignal("moved");
		mController = new CharPrimitive(dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"))
			,Vector3(0,2,0), 0.375f*1.1f, 0.75f*1.1f);
		mAccumulator = 0.f;
		mInterpolation = 0.f;
		mPositionLast = Vector3(0,2,0);
		mJumping = false;
		mGravityFactor = -2.f;
		mOnSolidGround = false;
	}
	//-----------------------------------------------------------------------
	
	IHPlayer::~IHPlayer()
	{
		if(mController)
		{
			delete mController;
			mController = 0;
		}
	}
	//-----------------------------------------------------------------------
	
	void IHPlayer::update(Real delta)
	{
		/*mAccumulator += std::min(delta, 0.5f);// cap at 0.5f to prevent craziness
		while(mAccumulator >= TIMESTEP)
		{
			mAccumulator -= TIMESTEP;
			mPositionLast = mController->getPosition();
			mController->translate(mController->move(Vector3(0,-1,0), TIMESTEP * 3));
			if(!mMove.isZeroLength())
			{
				Real dist = mMove.normalize();
				mController->translate(mController->move(mMove, dist * TIMESTEP, 5));
			}
		}*/


		if(mGravityFactor > -2.f)
			mGravityFactor -= delta * 7;

		Real offset = mGravityFactor < 0.f ? -1.f : 1.f;
		Real ab = delta * mGravityFactor;
		if(ab < 0.f)
			ab *= -1;
		mController->translate(mController->move(Vector3(0,offset,0), ab, 2));
		Real dist = mMove.normalize();
		mController->translate(mController->move(mMove, dist * delta, 5));
		
		BulletSubsystem* b = dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"));
		
		// check if we're on solid ground now
		RaycastReport rr = b->raycast(getPosition(),Vector3(0,-1,0),
			0.375f*1.1f+0.75f*1.1f+0.4f,COLLISION_GROUP_3,COLLISION_GROUP_3);
		mOnSolidGround = std::max(rr.hit * 
			TimeManager::getPtr()->getTimeDecimal(), mOnSolidGround); 

		if(rr.hit)
		{
			static_cast<Island*>(rr.userData)->score();
		}

		mInterpolation = mAccumulator / TIMESTEP;
		mMove = Vector3::ZERO;

		Vector3 movement = getPosition();
		getSignal("moved")->send(movement);
	}
	//-----------------------------------------------------------------------
	
	Vector3 IHPlayer::getPosition()
	{
		return mController->getPosition();
		//return mPositionLast * (mInterpolation) + 
		//	 mController->getPosition() * (1-mInterpolation);
	}
	//-----------------------------------------------------------------------
	
	void IHPlayer::setMove(const Message& move)
	{
		if(const Vector3* m = unpackMsg<Vector3>(move))
			mMove = *m;
	}
	//-----------------------------------------------------------------------
	
	void IHPlayer::jump(const Message& j)
	{
		if(TimeManager::getPtr()->getTimeDecimal() < mOnSolidGround + 0.5f)
		{
			mGravityFactor = 10.f;
			mJumping = true;
		}
	}
}
