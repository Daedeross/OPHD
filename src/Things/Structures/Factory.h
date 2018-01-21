#pragma once

#include "Structure.h"

#include "../../Common.h"

/**
 * FIXME:	Maybe should inherit from Resources and add the TurnsToBuild instead
 *			of using composition.
 */
class ProductionCost
{
public:
	ProductionCost() {}

	ProductionCost(int turns, int commonMetals, int commonMinerals, int rareMetals, int rareMinerals) :
		mTurnsToBuild(turns),
		mCommonMetals(commonMetals),
		mCommonMinerals(commonMinerals),
		mRareMetals(rareMetals),
		mRareMinerals(rareMinerals)
	{}

	~ProductionCost() {}

	void clear()
	{
		mCommonMetals = 0;
		mCommonMinerals = 0;
		mRareMetals = 0;
		mRareMinerals = 0;
		mTurnsToBuild = 0;
	}

	int commonMetals() const { return mCommonMetals; }
	int commonMinerals() const { return mCommonMinerals; }
	int rareMetals() const { return mRareMetals; }
	int rareMinerals() const { return mRareMinerals; }
	int turnsToBuild() const { return mTurnsToBuild; }

private:
	ProductionCost& operator=(const ProductionCost&) = delete;
	ProductionCost(const ProductionCost&) = delete;

private:
	int				mCommonMetals = 0;
	int				mCommonMinerals = 0;
	int				mRareMetals = 0;
	int 			mRareMinerals = 0;
	int				mTurnsToBuild = 0;
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
	// Callback providing what was complete and the ID of the factory.
	typedef NAS2D::Signals::Signal2<ProductType, int> ProductionCallback;

	typedef std::vector<ProductType> ProductionTypeList;

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

	ProductType productType() const { return mProduct; }
	void productType(ProductType _p);

	const ProductionTypeList& productList() const { return mAvailableProducts; }

	const ProductionCost& productCost(ProductType) const;

	virtual void initFactory() = 0;

	ProductionCallback& productionComplete() { return mProductionComplete; }

protected:
	void productionComplete(ProductType _p);

	void clearProduction();

	void addProduct(ProductType _p);
	bool enoughResourcesAvailable();

	ResourcePool* resourcePool() { return mResourcesPool; }

private:
	int								mTurnsCompleted = 0;
	int								mTurnsToComplete = 0;

	ProductType						mProduct = PRODUCT_NONE;

	ProductionTypeList				mAvailableProducts;			/**< List of products that the Factory can produce. */

	ProductionCallback				mProductionComplete;		/**< Callback used when production is complete. */

	ResourcePool*					mResourcesPool = nullptr;	/**< Pointer to the player's resource pool. UGLY. */
};
