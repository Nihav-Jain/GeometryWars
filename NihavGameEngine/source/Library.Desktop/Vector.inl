#include "pch.h"
#include "Vector.h"

namespace Library
{
#pragma region Vector

	template<typename T>
	Vector<T>::Vector() :
		mCapacity(0), mSize(0), mDataArray(nullptr)
	{
		Reserve(Vector::DEFAULT_CAPACITY);
	}

	template <typename T>
	Vector<T>::Vector(const Vector<T>& rhs) :
		Vector()
	{
		operator=(rhs);
	}

	template <typename T>
	Vector<T>::Vector(Vector&& rhs) :
		mSize(rhs.mSize), mCapacity(rhs.mCapacity), mDataArray(rhs.mDataArray)
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mDataArray = nullptr;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Vector<T>::Iterator itr = rhs.begin();
			for (; itr != rhs.end();++itr)
			{
				PushBack(*itr);
			}
		}
		return *this;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mDataArray = rhs.mDataArray;

			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mDataArray = nullptr;
		}
		return *this;
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Clear();
	}

	template <typename T>
	T& Vector<T>::operator[](uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Vector index out of bounds.");
		}
		return mDataArray[index];
	}

	template <typename T>
	const T& Vector<T>::operator[](uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Vector index out of bounds.");
		}
		return mDataArray[index];
	}

	template <typename T>
	void Vector<T>::PopBack()
	{
		if (IsEmpty())
		{
			throw std::exception("Invalid operation. Vector is empty.");
		}

		mDataArray[mSize - 1].~T();
		mSize--;
	}

	template <typename T>
	T& Vector<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::exception("Invalid operation. Vector is empty.");
		}
		return mDataArray[0];
	}

	template <typename T>
	const T& Vector<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::exception("Invalid operation. Vector is empty.");
		}
		return mDataArray[0];
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::exception("Invalid operation. Vector is empty.");
		}
		return mDataArray[mSize-1];
	}

	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::exception("Invalid operation. Vector is empty.");
		}
		return mDataArray[mSize - 1];
	}

	template <typename T>
	template <typename F>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T& dataToPush, F reserveStrategyFunctor)
	{
		// if list is full
		if (mSize == mCapacity)
		{
			uint32_t newCapacityValue = reserveStrategyFunctor(mSize, mCapacity);
			if (newCapacityValue <= mCapacity)
			{
				throw std::exception("Bad reserve strategy.");
			}
			Reserve(newCapacityValue);
		}
		new (mDataArray + mSize)T(dataToPush);
		mSize++;

		return Iterator(mSize, this);
	}

	template<typename T>
	bool Vector<T>::IsEmpty() const
	{
		return (0 == mSize);	// yoda statement
	}

	template <typename T>
	T& Vector<T>::At(uint32_t index)
	{
		return operator[](index);
	}

	template <typename T>
	void Vector<T>::Reserve(uint32_t newCapacity)
	{
		if (newCapacity > mCapacity)
		{
			if (mDataArray == nullptr)
			{
				mDataArray = (T*)malloc(newCapacity * sizeof(T));
			}
			else
			{
				mDataArray = (T*)realloc(mDataArray, newCapacity * sizeof(T));
			}
			mCapacity = newCapacity;
		}
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (uint32_t i = 0; i < mSize; i++)
		{
			mDataArray[i].~T();
		}
		if (mCapacity != 0)
		{
			free(mDataArray);
		}
		mSize = 0;
		mCapacity = 0;
		mDataArray = nullptr;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& dataToFind) const
	{
		Vector<T>::Iterator itr = begin();
		for (; itr != end(); ++itr)
		{
			if (*itr == dataToFind)
			{
				break;
			}
		}
		return itr;
	}

	template <typename T>
	typename void Vector<T>::Remove(const T& dataToRemove)
	{
		Remove(Find(dataToRemove));
	}

	template <typename T>
	typename void Vector<T>::Remove(Iterator itr)
	{
		if (itr.mOwnerVector != this)
		{
			throw new std::exception("Iterator does not belong to this Vector");
		}
		if (itr.mCurrentIndex == 0)
		{
			throw new std::exception("Invalid Iterator");
		}

		if (itr != end())
		{
			Vector<T>::Iterator previousItr = itr;
			++itr;
			for (; itr != end(); ++itr)
			{
				*previousItr = *itr;
				++previousItr;
			}
			(*previousItr).~T();
			mSize--;
		}

	}

	template <typename T>
	typename void Vector<T>::Remove(Iterator itrBegin, Iterator itrEnd)
	{
		if (itrBegin.mOwnerVector != this || itrEnd.mOwnerVector != this)
		{
			throw new std::exception("Iterator does not belong to this Vector");
		}
		if (itrBegin.mCurrentIndex == 0 || itrEnd.mCurrentIndex == 0)
		{
			throw new std::exception("Invalid Iterator");
		}
		if (itrBegin.mCurrentIndex > itrEnd.mCurrentIndex)
		{
			throw new std::exception("Invalid range. Param 1 should be be less than Param 2");
		}

		Iterator itr = itrEnd;
		for (; itr != end() && itrBegin != itrEnd; ++itr)
		{
			*itrBegin = *itr;
			++itrBegin;
		}
		while (itr != end())
		{
			*itrBegin = *itr;
			++itr;
			++itrBegin;
		}
		while (itrBegin != end())
		{
			PopBack();
		}
	}
	
	template <typename T>
	uint32_t Vector<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	uint32_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::begin() const
	{
		Vector<T>::Iterator itr(1, this);
		return itr;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::end() const
	{
		Vector<T>::Iterator itr(mSize + 1, this);
		return itr;
	}

#pragma endregion

#pragma region Iterator

	template <typename T>
	Vector<T>::Iterator::Iterator() :
		mCurrentIndex(0), mOwnerVector(nullptr)
	{}

	template <typename T>
	Vector<T>::Iterator::Iterator(uint32_t currentIndex, const Vector<T>* ownerVector) :
		mCurrentIndex(currentIndex), mOwnerVector(ownerVector)
	{}

	template<typename T>
	Vector<T>::Iterator::Iterator(const Iterator& rhs) :
		mCurrentIndex(rhs.mCurrentIndex), mOwnerVector(rhs.mOwnerVector)
	{}

	template <typename T>
	Vector<T>::Iterator::Iterator(Iterator&& rhs) :
		mCurrentIndex(rhs.mCurrentIndex), mOwnerVector(rhs.mOwnerVector)
	{
		rhs.mCurrentIndex = 0;
		rhs.mOwnerVector = nullptr;
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			mCurrentIndex = rhs.mCurrentIndex;
			mOwnerVector = rhs.mOwnerVector;
		}
		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(Iterator&& rhs)
	{
		if (this != &rhs)
		{
			mCurrentIndex = rhs.mCurrentIndex;
			mOwnerVector = rhs.mOwnerVector;

			rhs.mCurrentIndex = 0;
			rhs.mOwnerVector = nullptr;
		}
		return *this;
	}

	template<typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		bool isIteratorEqual = true;
		if (mOwnerVector == nullptr || rhs.mOwnerVector == nullptr)
			isIteratorEqual = false;
		else if (mOwnerVector != rhs.mOwnerVector)
			isIteratorEqual = false;
		else if (mCurrentIndex != rhs.mCurrentIndex)
			isIteratorEqual = false;
		return isIteratorEqual;
	}

	template<typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator & rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mOwnerVector == nullptr)
		{
			throw std::exception("Non hosted Iterator. Iterator not attached to a valid Vector.");
		}
		if (mCurrentIndex == 0 || mCurrentIndex > mOwnerVector->Size())
		{
			throw std::exception("vector index out of bounds");
		}
		mCurrentIndex++;
		return *this;
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template<typename T>
	T& Vector<T>::Iterator::operator*() const
	{
		if (mOwnerVector == nullptr)
		{
			throw std::exception("Non hosted Iterator. Iterator not attached to a valid SList.");
		}
		if (mCurrentIndex == 0 || mCurrentIndex > mOwnerVector->Size())
		{
			throw std::exception("vector index out of bounds");
		}
		return mOwnerVector->mDataArray[mCurrentIndex - 1];
	}
#pragma endregion

	template <typename T>
	uint32_t Vector<T>::DefaultReserveStrategy::operator()(uint32_t size, uint32_t capacity) const
	{
		UNREFERENCED_PARAMETER(size);
		return 2 * capacity + 1;
	}

}