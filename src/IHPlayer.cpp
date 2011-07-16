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
			,Vector3(0,-0.8,0), 1.15f, 0.3f);
		mAccumulator = 0.f;
		mInterpolation = 0.f;
		mPositionLast = Vector3(0,2,0);
		mGravityFactor = -2.f;
		mOnSolidGround = false;
		mExtraJumps = 0;
		mSpeed = 18.f;
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
		{
			mGravityFactor -= delta * 20;
		}

		Real offset = mGravityFactor < 0.f ? -1.f : 1.f;
		Real ab = delta * mGravityFactor;
		ab *= mGravityFactor > 0.f ? 1.f : 2.f;
		if(mGravityFactor < 0.f && TimeManager::getPtr()->getTimeDecimal() < mOnSolidGround + 0.25f)
			ab *= 0.175f;
		if(ab < 0.f)
			ab *= -1;
		mController->translate(mController->move(Vector3(0,offset,0), ab, 2));
		Real dist = mMove.normalize();
		mController->translate(mController->move(mMove * mSpeed, dist * delta, 5));
		
		BulletSubsystem* b = dynamic_cast<BulletSubsystem*>(Engine::getPtr()->getSubsystem("BulletSubsystem"));
		
		// check if we're on solid ground now
		RaycastReport rr = b->raycast(getPosition(),Vector3(0,-1,0),
			1.7f,COLLISION_GROUP_3,COLLISION_GROUP_3);
		mOnSolidGround = std::max(rr.hit * 
			TimeManager::getPtr()->getTimeDecimal(), mOnSolidGround); 

		if(rr.hit && mGravityFactor < 5.f)
		{
			static_cast<Island*>(rr.userData)->score();
			mExtraJumps = 1;
			// auto jump experiment: 
			//mGravityFactor = 13.5f;
			//mOnSolidGround = 0.f;
		}

		mInterpolation = mAccumulator / TIMESTEP;
		mMove = Vector3::ZERO;

		Vector3 movement = getPosition();
		getSignal("moved")->send(movement+Vector3(0,-0.45f,-0.3f));
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
		if(mGravityFactor < 11.f && TimeManager::getPtr()->getTimeDecimal() < mOnSolidGround + 0.5f)
		{
			mGravityFactor = std::min(mSpeed * 1.25f, 13.5f);
			mOnSolidGround = 0.f;
			dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D(
				"../media/audio/jump_01.wav");
		}
		else if(mExtraJumps > 0)
		{
			--mExtraJumps;
			if(mGravityFactor < 7.5f)
				mGravityFactor = std::min(mSpeed, 11.f);
			else
				mGravityFactor+=2.5f;
			dynamic_cast<ALSubsystem*>(Engine::getPtr()->getSubsystem("ALSubsystem"))->play2D(
				"../media/audio/jump_01.wav");
		}
	}
}
