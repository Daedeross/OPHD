#pragma once

#include "PowerStructure.h"

#include "../../Constants/Strings.h"


const int SOLAR_PANEL_BASE_PRODUCUCTION = 50;


class SolarPanelArray : public PowerStructure
{
public:
	SolarPanelArray(float meanSolarDistance) : PowerStructure(constants::SolarPanel1,
		"structures/solar_array1.sprite",
		StructureClass::EnergyProduction,
		StructureID::SID_SOLAR_PANEL1),
		mMeanSolarDistance(meanSolarDistance)
	{
		maxAge(1000);
		turnsToBuild(4);
		requiresCHAP(false);
		hasCrime(true);
	}

protected:
	int calculateMaxEnergyProduction() override
	{
		// Prevent possible dividing by zero
		float solarDistance = mMeanSolarDistance != 0 ? mMeanSolarDistance : 1;

		return static_cast<int>(SOLAR_PANEL_BASE_PRODUCUCTION / solarDistance);
	}

private:
	const float mMeanSolarDistance;
};
