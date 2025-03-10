#pragma once

#include "Structure.h"

#include "../../Constants/Strings.h"
#include "../../Map/Tile.h"


class CargoLander : public Structure
{
public:

	using Signal = NAS2D::Signal<>;

	CargoLander(Tile* tile) : Structure(constants::CargoLander,
		"structures/lander_0.sprite",
		StructureClass::Lander,
		StructureID::SID_CARGO_LANDER),
		mTile(tile)
	{
		maxAge(50);
		turnsToBuild(1);
		repairable(false);
		requiresCHAP(false);
		selfSustained(true);
		integrityDecayRate(2);

		enable();
	}

	Signal::Source& deploySignal() { return mDeploy; }

protected:
	void think() override
	{
		if (age() == turnsToBuild())
		{
			mDeploy();
			mTile->index(TerrainType::Dozed);
		}
	}

private:
	CargoLander() = delete;
	CargoLander(const CargoLander&) = delete;
	CargoLander& operator=(const CargoLander&) = delete;

private:
	Signal mDeploy;
	Tile* mTile = nullptr;
};
