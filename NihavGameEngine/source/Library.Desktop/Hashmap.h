#pragma once

#include "DefaultHash.h"

namespace Library
{
	/**
	 *	Templated class for a Hashmap
	 */
	template <typename TKey, typename TData, typename HashFunctor = DefaultHash<TKey>>
	class Hashmap
	{
		
		public:
			/**
			 *	Key-Value pair
			 */
			typedef std::pair<TKey, TData> PairType;

		private:
			typedef SList<PairType> ChainType;
			typedef Vector<ChainType> BucketType;
			typedef typename ChainType::Iterator ChainIterator;

			static const std::uint32_t DEFAULT_BUCKET_SIZE = 13;
		public:
			/**
			 *	Iterator for the Hashmap
			 */
			class Iterator
			{
				friend Hashmap;
			public:
				/**
				 *	constructs a default Iterator, pointing nowhere
				 */
				Iterator();

				/**
				 *	Iterator Copy Contstructor
				 *	@param constant reference to the Iterator to be copied from
				 */
				Iterator(const Iterator& rhs);

				/**
				 *	Iterator Move Contstructor
				 *	@param R value reference to the Iterator to be moved
				 */
				Iterator(Iterator&& rhs);

				/**
				 *	Default destructor
				 */
				~Iterator() = default;

				/**
				 *	Iterator assignment operator
				 *	@param constant reference to the Iterator to be copied from
				 *	@return reference to this Iterator
				 */
				Iterator& operator=(const Iterator& rhs);

				/**
				 *	Iterator Move assignment operator
				 *	@param R value reference to the Iterator to be moved
				 *	@return reference to this Iterator
				 */
				Iterator& operator=(Iterator&& rhs);

				/**
				 *	Pre-fix Increment operator
				 *	@return reference to the incremented Iterator
				 */
				Iterator& operator++();

				/**
				 *	Post-fix Increment operator
				 *	@param dummy parameter to differentiate between prefix and postforx operators
				 *	@return copy of Iterator before increment
				 */
				Iterator operator++(int);

				/**
				 *	Equality operator
				 *	@param constant reference to the Iterator to be compared with
				 *	@return true if owner, current bucket and current chain node of both iterators is same
				 */
				bool operator==(const Iterator& rhs) const;

				/**
				 *	Not-Equals operator
				 *	@param constant reference to the Iterator to be compared with
				 *	@return compliment of equality operator
				 */
				bool operator!=(const Iterator& rhs) const;

				/**
				 *	Dereference operator
				 *	@return reference to the Key-Value pair pointed by the current Iterator
				 */
				PairType& operator*();

				/**
				 *	Dereference(arrow) operator
				 *	@return pointer to the Key-Value pair pointed by the current Iterator
				 */
				PairType* operator->();

			private:
				/**
				 *	constructs an Iterator
				 *	@param owner Hashmap of this Iterator
				 *	@param index of the bucket this Iterator will be pointing to
				 *	@param Iterator of the chain this Iterator will be poinitng to
				 */
				Iterator(const Hashmap* owner, std::uint32_t currentBucketIndex, const ChainIterator& currentPair);

				const Hashmap* mOwner;
				std::uint32_t mCurrentBucketIndex;
				ChainIterator mCurrentPair;
			};

			/**
			 *	constructs a Hashmap with the specified number of buckets
			 *	@param initial number of buckets for the Hashmap to start with
			 */
			explicit Hashmap(std::uint32_t bucketSize = DEFAULT_BUCKET_SIZE);

			/**
			 *	default copy contructor
			 *	@param constant reference to the Hashmap to be copied from
			 */
			Hashmap(const Hashmap& rhs) = default;

			/**
			 *	default copy assignment operator
			 *	@param constant reference to the Hashmap to be copied from
			 *	@return reference to this updated Hashmap
			 */
			Hashmap& operator=(const Hashmap& rhs) = default;

			/**
			 *	move constructor
		 	 *	@param reference to the Hashmap to be moved
			 */
			Hashmap(Hashmap&& rhs);

			/**
			 *	move assignment operator
			 *	@param reference to the Hashmap to be move
			 *	@return reference to this updated Hashmap
			 */
			Hashmap& operator=(Hashmap&& rhs);

			/**
			 *	default destructor
			 */
			~Hashmap() = default;

			/**
			 *	finds the given key in the hashmap
			 *	@param key to be searched
			 *	@return iterator pointing to the Key-Value pair, if found; points to end() otherwise
			 */
			Iterator Find(const TKey& key) const;

			/**
			 *	inserts the given pair in the Hashmap; if the key already exists, do nothing
			 *	@param Key-Value pair to be inserted
			 *	@return if key already exists, iterator pointing to that pair, else points to newly inserted pair
			 */
			Iterator Insert(const PairType& data);

			Iterator Insert(const TKey& key, TData& data);

			Iterator Insert(const PairType& data, bool& didNewInsert);

			/**
			 *	index operator - inserts the key if not already present
			 *	@param Key
			 *	@return reference to data associated with the given key
			 */
			TData& operator[](const TKey& key);

			/**
			 *	removed pair associated with given key, does nothing if key is not found
			 *	@param Key for the data to be removed
			 */
			void Remove(const TKey& key);

			/**
			 *	deleted all data of the Hashmap
			 */
			void Clear();

			/**
			 *	number of Key-Value pairs currently stored in the Hashmap
			 *	@return unsigned integer represting the number of elements
			 */
			std::uint32_t Size() const;

			/**
			 *	checks if a pair a associated with given key exists in the Hashmap
			 *	@param Key to be queried
			 *	@return true if a pair for this key exists, false otherwise
			 */
			bool ContainsKey(const TKey& key) const;

			/**
			 *	gets the Iterator pointing to first element in the Hashmap
			 *	@return Iterator, point to end() if list is empty
			 */
			Iterator begin() const;

			/**
			 *	gets the Iterator pointing to end of the Hashmap
			 *	@return Iterator
			 */
			Iterator end() const;

			/**
			 *	ratio of number of occupied buckets over total number of available buckets
			 *	@return float value depicting the ratio
			 */
			std::float_t LoadFactor() const;

			/**
			 *	Rehashes the map
			 *	@param number of buckets to be used now in the hashmap
			 */
			void Rehash(std::uint32_t numberOfBuckets);

		private:
			BucketType buckets;
			static const HashFunctor mHashFunction;
			std::uint32_t mSize;

			std::uint32_t mLowestUsedBucketIndex;
			std::uint32_t mHighestUsedBucketIndex;
	};

}
#include "Hashmap.inl"

