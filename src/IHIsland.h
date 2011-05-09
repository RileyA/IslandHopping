#ifndef IH_ISLAND_H
#define IH_ISLAND_H

#include "IH.h"

namespace IH
{
	class Generator;

	struct IslandSchematic
	{
	public:

		IslandSchematic(String _mesh = "basic_island.mesh", Colour _color = Colour(), unsigned int _id = 0)
			:mesh(_mesh),color(_color),id(_id) {}

		const String mesh;
		const Colour color;
		const unsigned int id;

	};

	class Island
	{
	public:

		Island(IslandSchematic* schematic, Vector3 position = Vector3::ZERO, Real roll = 0.f);
		~Island();

		void score();
		void respawn(Vector3 position, Real roll = 0.f);
		void hide();

	private:

		IslandSchematic* mSchematic;

		Mesh* mMesh;
		Generator* mParent;

		bool mActive;
	};
}

#endif
