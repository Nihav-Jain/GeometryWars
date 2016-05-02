#include "pch.h"
#include "Hashmap.h"

namespace Library
{
#pragma region Hashmap

	template<typename TKey, typename TData, typename HashFunctor>
	const HashFunctor Hashmap<TKey, TData, HashFunctor>::mHashFunction;

	template<typename TKey, typename TData, typename HashFunctor>
	const std::uint32_t Hashmap<TKey, TData, HashFunctor>::DEFAULT_BUCKET_SIZE;

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Hashmap(std::uint32_t bucketSize) :
		buckets(bucketSize, true), mSize(0), mLowestUsedBucketIndex(bucketSize-1), mHighestUsedBucketIndex(0)
	{
		if (bucketSize == 0)
			throw std::exception("Cannot instantiate Hashmap with no buckets");
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Hashmap(std::initializer_list<typename Hashmap::PairType> initList) :
		buckets(static_cast<std::uint32_t>(initList.size()) , true), mSize(0), mLowestUsedBucketIndex(static_cast<std::uint32_t>(initList.size()) - 1), mHighestUsedBucketIndex(0)
	{
		for (auto& pair : initList)
		{
			Insert(pair);
		}
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>& Hashmap<TKey, TData, HashFunctor>::operator=(Hashmap&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			buckets = std::move(rhs.buckets);
			mSize = rhs.mSize;
			mLowestUsedBucketIndex = rhs.mLowestUsedBucketIndex;
			mHighestUsedBucketIndex = rhs.mHighestUsedBucketIndex;

			rhs.mSize = 0;
			rhs.mLowestUsedBucketIndex = buckets.Size() - 1;
			rhs.mHighestUsedBucketIndex = 0;
		}
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Hashmap(Hashmap&& rhs) :
		buckets(std::move(rhs.buckets)), mSize(rhs.mSize), mLowestUsedBucketIndex(rhs.mLowestUsedBucketIndex), mHighestUsedBucketIndex(rhs.mHighestUsedBucketIndex)
	{
		rhs.mSize = 0;
		rhs.mLowestUsedBucketIndex = buckets.Size() - 1;
		rhs.mHighestUsedBucketIndex = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Find(const TKey& index) const
	{
		Iterator itr = end();
		std::uint32_t bucketIndex = mHashFunction(index) % buckets.Size();
		
		ChainIterator chainItr = buckets[bucketIndex].begin();
		for (; chainItr != buckets[bucketIndex].end(); ++chainItr)
		{
			if ((*chainItr).first == index)
			{
				itr = Iterator(this, bucketIndex, chainItr);
				break;
			}
		}
		return itr;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Insert(const PairType& data)
	{
		bool didNewInsert;
		return Insert(data, didNewInsert);
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Insert(const TKey& key, const TData& data)
	{
		return Insert(PairType(key, data));
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Insert(const PairType& data, bool& didNewInsert)
	{
		didNewInsert = false;
		Iterator itr = Find(data.first);
		if (itr == end())
		{
			std::uint32_t bucketIndex = mHashFunction(data.first) % buckets.Size();
			ChainIterator pairItr = buckets[bucketIndex].PushBack(data);
			itr = Iterator(this, bucketIndex, pairItr);

			if (bucketIndex < mLowestUsedBucketIndex)
				mLowestUsedBucketIndex = bucketIndex;
			if (bucketIndex > mHighestUsedBucketIndex)
				mHighestUsedBucketIndex = bucketIndex;

			mSize++;
			didNewInsert = true;
		}
		return itr;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	TData& Hashmap<TKey, TData, HashFunctor>::operator[](const TKey& index)
	{
		Iterator itr = Insert(PairType(index, TData()));
		return (*itr).second;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	TData& Hashmap<TKey, TData, HashFunctor>::operator[](const TKey& index) const
	{
		Iterator itr = Find(index);
		if (itr == end())
			throw std::exception("ClearScreen index");
		return (*itr).second;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	void Hashmap<TKey, TData, HashFunctor>::Remove(const TKey& index)
	{
		Iterator itr = Find(index);
		if (itr != end())
		{
			if (itr.mCurrentBucketIndex == mLowestUsedBucketIndex && buckets[mLowestUsedBucketIndex].Size() == 1)
			{
				while (mLowestUsedBucketIndex < mHighestUsedBucketIndex && buckets[mLowestUsedBucketIndex].IsEmpty())
				{
					++mLowestUsedBucketIndex;
				}
			}
			if (itr.mCurrentBucketIndex == mHighestUsedBucketIndex && buckets[mHighestUsedBucketIndex].Size() == 1)
			{
				while (mHighestUsedBucketIndex > mLowestUsedBucketIndex && buckets[mHighestUsedBucketIndex].IsEmpty())
				{
					--mHighestUsedBucketIndex;
				}
			}
			buckets[itr.mCurrentBucketIndex].Remove(*itr);
			mSize--;
		}
	}

	template<typename TKey, typename TData, typename HashFunctor>
	void Hashmap<TKey, TData, HashFunctor>::Clear()
	{
		for (auto& chain : buckets)
		{
			chain.Clear();
		}
		mSize = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	std::uint32_t Hashmap<TKey, TData, HashFunctor>::Size() const
	{
		return mSize;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	bool Hashmap<TKey, TData, HashFunctor>::ContainsKey(const TKey& index) const
	{
		Iterator itr = Find(index);
		return (itr != end());
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::begin() const
	{
		if (mLowestUsedBucketIndex > mHighestUsedBucketIndex)
			return end();
		return Iterator(this, mLowestUsedBucketIndex, buckets[mLowestUsedBucketIndex].begin());
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::end() const
	{
		return Iterator(this, mHighestUsedBucketIndex, buckets[mHighestUsedBucketIndex].end());
	}

	template<typename TKey, typename TData, typename HashFunctor>
	std::float_t Hashmap<TKey, TData, HashFunctor>::LoadFactor() const
	{
		if (buckets.IsEmpty())
			return 0;

		std::uint32_t occupiedBuckets = 0;

		for (auto& itr : buckets)
		{
			if (!itr.IsEmpty())
				occupiedBuckets++;
		}

		return static_cast<float>(occupiedBuckets) / buckets.Size();
	}

	template<typename TKey, typename TData, typename HashFunctor>
	void Hashmap<TKey, TData, HashFunctor>::Rehash(std::uint32_t numberOfBuckets)
	{
		Hashmap rehashedMap(numberOfBuckets);

		if (mSize > 0)
		{
			for (Iterator itr = begin(); itr != end(); ++itr)
			{
				rehashedMap.Insert(*itr);
			}
		}

		*this = std::move(rehashedMap);
	}


#pragma endregion

#pragma region Iterator

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::Iterator() :
		mOwner(nullptr), mCurrentBucketIndex(0), mCurrentPair()
	{}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::Iterator(const Iterator& rhs) :
		mOwner(rhs.mOwner), mCurrentBucketIndex(rhs.mCurrentBucketIndex), mCurrentPair(rhs.mCurrentPair)
	{}
	
	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::Iterator(Iterator&& rhs) :
		mOwner(rhs.mOwner), mCurrentBucketIndex(rhs.mCurrentBucketIndex), mCurrentPair(std::move(rhs.mCurrentPair))
	{
		rhs.mOwner = nullptr;
		rhs.mCurrentBucketIndex = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	Hashmap<TKey, TData, HashFunctor>::Iterator::Iterator(const Hashmap* owner, std::uint32_t currentBucketIndex, typename const Hashmap::ChainIterator& currentPair) :
		mCurrentPair(currentPair), mCurrentBucketIndex(currentBucketIndex), mOwner(owner)
	{}

	//template<typename TKey, typename TData, typename HashFunctor>
	//Hashmap<TKey, TData, HashFunctor>::Iterator::~Iterator()
	//{}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator& Hashmap<TKey, TData, HashFunctor>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			mOwner = rhs.mOwner;
			mCurrentBucketIndex = rhs.mCurrentBucketIndex;
			mCurrentPair = rhs.mCurrentPair;
		}
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator& Hashmap<TKey, TData, HashFunctor>::Iterator::operator=(Iterator&& rhs)
	{
		if (this != &rhs)
		{
			mOwner = rhs.mOwner;
			mCurrentBucketIndex = rhs.mCurrentBucketIndex;
			mCurrentPair = std::move(rhs.mCurrentPair);

			rhs.mOwner = nullptr;
			rhs.mCurrentBucketIndex = 0;
		}
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator& Hashmap<TKey, TData, HashFunctor>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("ClearScreen operation. Non-hosted iterator.");
		}
		if (*this == mOwner->end())
		{
			throw std::exception("ClearScreen operation. Cannot increment Iterator already at end.");
		}

		++mCurrentPair;
		while (mCurrentPair == (mOwner->buckets[mCurrentBucketIndex]).end())
		{
			if (mCurrentBucketIndex == mOwner->mHighestUsedBucketIndex)
			{
				break;
			}
			++mCurrentBucketIndex;
			mCurrentPair = (mOwner->buckets[mCurrentBucketIndex]).begin();
		}
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::Iterator Hashmap<TKey, TData, HashFunctor>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename bool Hashmap<TKey, TData, HashFunctor>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mOwner == rhs.mOwner) && (mCurrentBucketIndex == rhs.mCurrentBucketIndex) && (mCurrentPair == rhs.mCurrentPair);
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename bool Hashmap<TKey, TData, HashFunctor>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::PairType& Hashmap<TKey, TData, HashFunctor>::Iterator::operator*()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("ClearScreen operation. Cannot dereference a non-hosted Iterator.");
		}
		if (mCurrentPair == (mOwner->buckets[mCurrentBucketIndex]).end())
		{
			throw std::exception("ClearScreen operation. Cannot dereference Iterator pointing to end.");
		}

		return *mCurrentPair;
	}

	template<typename TKey, typename TData, typename HashFunctor>
	const typename Hashmap<TKey, TData, HashFunctor>::PairType& Hashmap<TKey, TData, HashFunctor>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("ClearScreen operation. Cannot dereference a non-hosted Iterator.");
		}
		if (mCurrentPair == (mOwner->buckets[mCurrentBucketIndex]).end())
		{
			throw std::exception("ClearScreen operation. Cannot dereference Iterator pointing to end.");
		}

		return *mCurrentPair;
	}


	template<typename TKey, typename TData, typename HashFunctor>
	typename Hashmap<TKey, TData, HashFunctor>::PairType* Hashmap<TKey, TData, HashFunctor>::Iterator::operator->()
	{
		return &(operator*());
	}

	template<typename TKey, typename TData, typename HashFunctor>
	const typename Hashmap<TKey, TData, HashFunctor>::PairType* Hashmap<TKey, TData, HashFunctor>::Iterator::operator->() const
	{
		return &(operator*());
	}

#pragma endregion

}