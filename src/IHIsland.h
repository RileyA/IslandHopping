#ifndef IH_ISLAND_H
#define IH_ISLAND_H

#include "IH.h"
#include "OryxObject.h"

namespace IH
{
	class Generator;
	class WaveManager;

	class Island : public Object
	{
	public:

		friend class Generator;

		Island(String mesh, size_t id, Vector3 position = Vector3::ZERO, Real roll = 0.f,
			long long seed = 0, Generator* parent = 0);
		~Island();

		void update(Real delta);
		void score();
		void respawn(Vector3 position, size_t id, Real roll = 0.f);
		void hide();

	private:

		String mMeshName;
		size_t mId;

		Mesh* mMesh;
		CollisionObject* mCollide;
		Generator* mParent;
		Rand mGen;
		Vector3 mPosition;
		Real bob;
		Real bob_offset;
		WaveManager* mWaves;

		bool mActive;
		bool mScored;
	};
}

#endif
