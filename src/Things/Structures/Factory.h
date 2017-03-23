#pragma once

#include "Structure.h"

#include "../../RobotPool.h"


/**
 * FIXME:	Maybe should inherit from Resources and add the TurnsToBuild instead
 *			of using composition.
 */
struct ProductionCost
{
	ProductionCost() : TurnsToBuild(0) {}

	ProductionCost(int turns, int commonMetals, int commonMinerals, int rareMetals, int rareMinerals) : TurnsToBuild(turns)
	{
		CostPerTurn.commonMetals(commonMetals);
		CostPerTurn.commonMinerals(commonMinerals);
		CostPerTurn.rareMetals(rareMetals);
		CostPerTurn.rareMinerals(rareMinerals);
	}

	~ProductionCost() {}

	void clear()
	{
		CostPerTurn.clear();
		TurnsToBuild = 0;
	}

	ResourcePool	CostPerTurn;
	int				TurnsToBuild;
};



/**
 * \brief	Defines the Factory interface.
 * 
 * Factory derives from Structure and provides the basic factory interface and
 * the underlying factory production code. Exactly what a factory is capable of
 * producing is up to the derived factory type.
 * 
 * \note	The Factory interface defines two public functions: resourcePool() and robotoPool().
 *			These must be called manually whenever a derived factory type is added to the game as
 *			there are no manager objects that do this (by design).
 * 
 * \warning	There are no sanity checks in the underlying production code to check if resourcePool
 *			or robotPool have been properly set. It is assumed that they have been.
 */
class Factory : public Structure
{

public:

	enum ProductionType
	{
		PRODUCTION_NONE,

		// Surface Factories
		PRODUCTION_DIGGER,
		PRODUCTION_DOZER,
		PRODUCTION_MINER
	};

	typedef NAS2D::Signals::Signal1<ProductionType> ProductionCallback;

	typedef std::vector<ProductionType> ProductionTypeList;

public:

	Factory(const std::string& name, const std::string& sprite_path);
	virtual ~Factory();

	virtual void updateProduction();

	void resourcePool(ResourcePool* _r) { mResourcesPool = _r; }

	int productionTurnsToComplete() const { return mTurnsToComplete; }
	void productionTurnsToComplete(int _l) { mTurnsToComplete = _l; }

	int productionTurnsCompleted() const { return mTurnsCompleted; }
	void productionTurnsCompleted(int _t) { mTurnsCompleted = _t; }
	void productionResetTurns() { mTurnsCompleted = 0; }

	ProductionType productionType() const { return mProduction; }
	void productionType(ProductionType _p);

	const ProductionTypeList& productionList() const { return mAvailableProducts; }

	const ProductionCost& productionCost(ProductionType) const;

	virtual void initFactory() = 0;

	ProductionCallback& productionComplete() { return mProductionComplete; }

protected:

	void productionComplete(ProductionType _p);

	void clearProduction();

	void addProduct(ProductionType _p);
	bool enoughResourcesAvailable();

	ResourcePool* resourcePool() { return mResourcesPool; }

private:

	int								mTurnsCompleted;
	int								mTurnsToComplete;

	ProductionType					mProduction;

	ProductionTypeList				mAvailableProducts;			/**< List of products that the Factory can produce. */

	ProductionCallback				mProductionComplete;		/**< Callback used when production is complete. */

	ResourcePool*					mResourcesPool;				/**< Pointer to the player's resource pool. UGLY. */
};