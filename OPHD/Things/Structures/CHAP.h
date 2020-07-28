#pragma once

#include "Structure.h"

class CHAP : public Structure
{
public:
	CHAP() : Structure(constants::CHAP, "structures/chap.sprite", StructureClass::LifeSupport)
	{
		maxAge(600);
		turnsToBuild(5);

		requiresCHAP(false);
	}

protected:
	void defineResourceInput() override
	{
		resourcesIn().commonMinerals(2);
		resourcesIn().rareMinerals(1);
		resourcesIn().rareMetals(1);
		resourcesIn().energy(10);
	}
};
