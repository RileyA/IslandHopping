#ifndef IH_OCEAN_H
#define IH_OCEAN_H

#include "IH.h"
#include "OryxObject.h"
#include "IHWaveManager.h"

namespace IH
{
	class Ocean : public Object
	{
	public:

		Ocean();
		virtual ~Ocean();

		void update(Real delta);
		void moved(const Message& m);

	private:

		MeshData mData;
		Mesh* mMesh;
		Mesh* mMesh_outer;
		Mesh* mFloor;

		const static size_t VERTS_X = 100;
		const static size_t VERTS_Y = 100;

		Real mTimer;
		WaveManager* mWaveMgr;

	};
}

#endif
