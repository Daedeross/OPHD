#pragma once

#include "Things/Structures/Structure.h"


namespace NAS2D {
	namespace Xml {
		class XmlElement;
	}
}

class Tile;
class PopulationPool;
struct StorableResources;


/**
 * Handles structure updating and resource management for structures.
 *
 * Keeps track of which structures are operational, idle and disabled.
 */
class StructureManager
{
public:
	StructureManager() = default;
	~StructureManager() = default;

	void addStructure(Structure* st, Tile* t);
	void removeStructure(Structure* st);

	StructureList& structureList(Structure::StructureClass st);
	Tile* tileFromStructure(Structure* st);

	void disconnectAll();
	void dropAllStructures();

	int count() const;

	int getCountInState(Structure::StructureClass st, StructureState state);

	int disabled();
	int destroyed();

	bool CHAPAvailable();

	void updateEnergyProduction();
	int totalEnergyProduction() const { return mTotalEnergyOutput; }
	int totalEnergyUsed() const { return mTotalEnergyUsed; }
	int totalEnergyAvailable() const { return mTotalEnergyOutput - mTotalEnergyUsed; }

	void update(StorableResources&, PopulationPool&);

	void serialize(NAS2D::Xml::XmlElement* element);

private:
	using StructureTileTable = std::map<Structure*, Tile*>;
	using StructureClassTable = std::map<Structure::StructureClass, StructureList>;


	void updateStructures(StorableResources&, PopulationPool&, StructureList&);
	void updateFactoryProduction();

	bool structureConnected(Structure* st);


	StructureTileTable mStructureTileTable; /**< List mapping Structures to a particular tile. */
	StructureClassTable mStructureLists; /**< Map containing all of the structure list types available. */

	int mTotalEnergyOutput = 0; /**< Total energy output of all energy producers in the structure list. */
	int mTotalEnergyUsed = 0;
};
