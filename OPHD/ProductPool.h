#pragma once

#include "Common.h"
#include "Constants.h"

#include <NAS2D/Dictionary.h>

#include <array>


int storageRequiredPerUnit(ProductType type);

class ProductPool
{
public:
	using ProductTypeCount = std::array<int, ProductType::PRODUCT_COUNT>;


	ProductPool() = default;
	~ProductPool() = default;

	ProductPool(const ProductPool&) = default;
	ProductPool& operator=(const ProductPool&) = default;

	int capacity() const;

	int productStorageRequirement(ProductType type) const;

	bool canStore(ProductType type, int count);
	bool empty() const;
	bool atCapacity() const;

	void transferAllTo(ProductPool& destination);
	void store(ProductType type, int count);
	int pull(ProductType type, int count);
	int count(ProductType type);

	int availableStorage() const;

	NAS2D::Dictionary serialize();
	void deserialize(const NAS2D::Dictionary& dictionary);

	void verifyCount();

private:
	ProductTypeCount mProducts = {{0}};

	int mCapacity = constants::BaseProductCapacity;
	int mCurrentStorageCount = 0;
};
