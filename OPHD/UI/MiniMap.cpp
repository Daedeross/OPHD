#include "MiniMap.h"

#include "../Cache.h"
#include "../Map/TileMap.h"
#include "../Map/MapView.h"
#include "../Things/Robots/Robot.h"
#include "../States/Route.h"
#include "../StructureManager.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Math/Vector.h>
#include <NAS2D/Math/Rectangle.h>
#include <NAS2D/Renderer/Color.h>
#include <NAS2D/Renderer/Renderer.h>

#include <map>


namespace
{
	const std::string MapTerrainExtension = "_a.png";
	const std::string MapDisplayExtension = "_b.png";
}


MiniMap::MiniMap(MapView& mapView, TileMap* tileMap, const std::map<Robot*, Tile*>& robotList, const std::string& mapName) :
	mMapView{mapView},
	mTileMap{tileMap},
	mRobotList{robotList},
	mIsHeightMapVisible{false},
	mBackgroundSatellite{mapName + MapDisplayExtension},
	mBackgroundHeightMap{mapName + MapTerrainExtension},
	mUiIcons{imageCache.load("ui/icons.png")}
{}


bool MiniMap::heightMapVisible() const
{
	return mIsHeightMapVisible;
}


void MiniMap::heightMapVisible(bool isVisible)
{
	mIsHeightMapVisible = isVisible;
}


/**
 * Draws the minimap and all icons/overlays for it.
 */
void MiniMap::draw() const
{
	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();
	const auto miniMapBoxFloat = mRect.to<float>();
	renderer.clipRect(miniMapBoxFloat);

	renderer.drawImage((mIsHeightMapVisible ? mBackgroundHeightMap : mBackgroundSatellite), miniMapBoxFloat.startPoint());

	const auto miniMapOffset = mRect.startPoint() - NAS2D::Point{0, 0};
	const auto ccPosition = ccLocation();
	if (ccPosition != CcNotPlaced)
	{
		const auto ccOffsetPosition = ccPosition + miniMapOffset;
		const auto ccCommRangeImageRect = NAS2D::Rectangle{166, 226, 30, 30};
		renderer.drawSubImage(mUiIcons, ccOffsetPosition - ccCommRangeImageRect.size() / 2, ccCommRangeImageRect);
		renderer.drawBoxFilled(NAS2D::Rectangle<int>::Create(ccOffsetPosition - NAS2D::Vector{1, 1}, NAS2D::Vector{3, 3}), NAS2D::Color::White);
	}

	auto& structureManager = NAS2D::Utility<StructureManager>::get();
	for (auto commTower : structureManager.getStructures<CommTower>())
	{
		if (commTower->operational())
		{
			const auto commTowerPosition = structureManager.tileFromStructure(commTower).xy();
			const auto commTowerRangeImageRect = NAS2D::Rectangle{146, 236, 20, 20};
			renderer.drawSubImage(mUiIcons, commTowerPosition + miniMapOffset - commTowerRangeImageRect.size() / 2, commTowerRangeImageRect);
		}
	}

	for (auto minePosition : mTileMap->mineLocations())
	{
		Mine* mine = mTileMap->getTile({minePosition, 0}).mine();
		if (!mine) { break; } // avoids potential race condition where a mine is destroyed during an updated cycle.

		auto mineBeaconStatusOffsetX = 0;
		if (!mine->active()) { mineBeaconStatusOffsetX = 0; }
		else if (!mine->exhausted()) { mineBeaconStatusOffsetX = 8; }
		else { mineBeaconStatusOffsetX = 16; }

		const auto mineImageRect = NAS2D::Rectangle{mineBeaconStatusOffsetX, 0, 7, 7};
		renderer.drawSubImage(mUiIcons, minePosition + miniMapOffset - NAS2D::Vector{2, 2}, mineImageRect);
	}

	// Temporary debug aid, will be slow with high numbers of mines
	// especially with routes of longer lengths.
	auto& routeTable = NAS2D::Utility<std::map<class MineFacility*, Route>>::get();
	for (auto route : routeTable)
	{
		for (auto tile : route.second.path)
		{
			const auto tilePosition = static_cast<Tile*>(tile)->xy();
			renderer.drawPoint(tilePosition + miniMapOffset, NAS2D::Color::Magenta);
		}
	}

	for (auto robotEntry : mRobotList)
	{
		const auto robotPosition = robotEntry.second->xy();
		renderer.drawPoint(robotPosition + miniMapOffset, NAS2D::Color::Cyan);
	}

	const auto& viewArea = mMapView.viewArea();
	renderer.drawBox(viewArea.translate(miniMapOffset + NAS2D::Vector{1, 1}), NAS2D::Color{0, 0, 0, 180});
	renderer.drawBox(viewArea.translate(miniMapOffset), NAS2D::Color::White);

	renderer.clipRectClear();
}


void MiniMap::onActivate()
{
	mLeftButtonDown = false;
}


void MiniMap::onMouseUp(NAS2D::EventHandler::MouseButton /*button*/, int /*x*/, int /*y*/)
{
	mLeftButtonDown = false;
}


void MiniMap::onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y)
{
	if (button == NAS2D::EventHandler::MouseButton::Left)
	{
		mLeftButtonDown = true;
		const auto mousePosition = NAS2D::Point{x, y};
		if (mRect.contains(mousePosition))
		{
			onSetView(mousePosition);
		}
	}
}


void MiniMap::onMouseMove(int x, int y, int /*rX*/, int /*rY*/)
{
	if (mLeftButtonDown)
	{
		const auto mousePosition = NAS2D::Point{x, y};
		if (mRect.contains(mousePosition))
		{
			onSetView(mousePosition);
		}
	}
}


void MiniMap::onSetView(NAS2D::Point<int> mousePixel)
{
	const auto position = NAS2D::Point{0, 0} + (mousePixel - mRect.startPoint());
	mMapView.centerOn(position);
}
