#include "IH.h"
#include "OryxObject.h"

namespace IH
{
	class Ocean : public Object
	{
	public:

		Ocean();
		virtual ~Ocean();

	private:

		MeshData mData;
		Mesh* mMesh;

		static const size_t VERTS_X = 15;
		static const size_t VERTS_Y = 15;
		static const Real SCALE_X = 100.f;
		static const Real SCALE_Y = 100.f;

	};
}
