#pragma once

#include "Tile.h"

#include "../States/Planet.h"
#include "../Things/Structures/Structure.h"
#include "../MicroPather/micropather.h"

#include <NAS2D/Renderer/Point.h>
#include <NAS2D/Renderer/Vector.h>

#include <algorithm>

using Point2dList = std::vector<NAS2D::Point<int>>;

class TileMap: public micropather::Graph
{
public:
	enum TileMapLevel
	{
		LEVEL_SURFACE = 0,
		LEVEL_UG_1,
		LEVEL_UG_2,
		LEVEL_UG_3,
		LEVEL_UG_4
	};

public:
	TileMap(const std::string& mapPath, const std::string& tilesetPath, int maxDepth, int mineCount, Planet::Hostility hostility /*= constants::Hostility::None*/, bool setupMines = true);
	~TileMap() override;

	Tile* getTile(NAS2D::Point<int> position, int level);
	Tile* getTile(NAS2D::Point<int> position) { return getTile(position, mCurrentDepth); }

	Tile* getVisibleTile(NAS2D::Point<int> position, int level);
	Tile* getVisibleTile() { return getVisibleTile(tileMouseHover(), mCurrentDepth); }

	bool isVisibleTile(NAS2D::Point<int> position, int z) const;
	bool isVisibleTile(NAS2D::Point<int> position) const { return isVisibleTile(position, mCurrentDepth); }
	bool isVisibleTile(const Tile& t) { return isVisibleTile(t.position(), t.depth()); }

	const NAS2D::Rectangle<int>& boundingBox() const { return mMapBoundingBox; }

	const NAS2D::Point<int>& mapViewLocation() const { return mMapViewLocation; }
	void mapViewLocation(NAS2D::Point<int> point);
	void centerMapOnTile(Tile*);

	bool tileHighlightVisible() const;
	NAS2D::Point<int> tileMouseHover() const { return mMapHighlight; }

	const Point2dList& mineLocations() const { return mMineLocations; }
	void removeMineLocation(const NAS2D::Point<int>& pt);

	void toggleShowConnections() { mShowConnections = !mShowConnections; }

	int edgeLength() const { return mEdgeLength; }
	NAS2D::Vector<int> size() const { return mSizeInTiles; }

	int currentDepth() const { return mCurrentDepth; }
	void currentDepth(int i) { mCurrentDepth = std::clamp(i, 0, mMaxDepth); }

	int maxDepth() const { return mMaxDepth; }

	void injectMouse(NAS2D::Point<int> position) { mMousePosition = position; }

	void initMapDrawParams(NAS2D::Vector<int>);
	
	void draw();

	void serialize(NAS2D::Xml::XmlElement* element, const Planet::Attributes& planetAttributes);
	void deserialize(NAS2D::Xml::XmlElement* element);

public:
	/** MicroPather public interface implementation. */
	float LeastCostEstimate(void* stateStart, void* stateEnd) override;
	void AdjacentCost(void* state, std::vector<micropather::StateCost>* adjacent) override;
	void PrintStateInfo(void* /*state*/) override {}

	void pathStartAndEnd(void* start, void* end);

protected:
	enum MouseMapRegion
	{
		MMR_MIDDLE,
		MMR_TOP_RIGHT,
		MMR_TOP_LEFT,
		MMR_BOTTOM_RIGHT,
		MMR_BOTTOM_LEFT
	};
	
	std::vector<std::vector<MouseMapRegion> > mMouseMap;

private:
	using TileGrid = std::vector<std::vector<Tile> >;
	using TileArray = std::vector<TileGrid>;
	
private:
	TileMap(const TileMap&) = delete; /**< Not Allowed */
	TileMap& operator=(const TileMap&) = delete; /**< Not allowed */

private:
	void buildMouseMap();
	void buildTerrainMap(const std::string& path);
	void setupMines(int, Planet::Hostility);

	void updateTileHighlight();

	MouseMapRegion getMouseMapRegion(int x, int y);

private:
	int mEdgeLength = 0;
	const NAS2D::Vector<int> mSizeInTiles;

	int mMaxDepth = 0; /**< Maximum digging depth. */
	int mCurrentDepth = 0; /**< Current depth level to view. */

	std::pair<void*, void*> mPathStartEndPair = { nullptr, nullptr };

	std::string mMapPath;
	std::string mTsetPath;

	TileArray mTileMap;

	const NAS2D::Image mTileset;
	const NAS2D::Image mMineBeacon;

	NAS2D::Timer mTimer;

	NAS2D::Point<int> mMousePosition; /**< Current mouse position. */
	NAS2D::Point<int> mMapHighlight; /**< Tile the mouse is pointing to. */
	NAS2D::Point<int> mMapViewLocation;

	NAS2D::Point<int> mMapPosition; /** Where to start drawing the TileMap on the screen. */

	Point2dList mMineLocations; /**< Location of all mines on the map. */

	NAS2D::Rectangle<int> mMapBoundingBox; /** Area that the TileMap fills when drawn. */

	bool mShowConnections = false; /**< Flag indicating whether or not to highlight connectedness. */
};
