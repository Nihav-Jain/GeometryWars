#include "pch.h"
#include "Vector.h"

namespace Library
{
	template <typename T, typename F>
	const F Vector<T, F>::ReserveStrategyFunctor;

#pragma region Vector

	template<typename T, typename F>
	Vector<T, F>::Vector(std::uint32_t defaultCapacity, bool fixedSize) :
		mCapacity(0), mSize(0), mDataArray(nullptr)
	{
		Reserve(defaultCapacity, fixedSize);
	}

	template<typename T, typename F>
	Vector<T, F>::Vector(std::initializer_list<T> initList) :
		mCapacity(0), mSize(0), mDataArray(nullptr)
	{
		Reserve(static_cast<std::uint32_t>(initList.size()));
		for (auto& item : initList)
		{
			PushBack(item);
		}
	}

	template<typename T, typename F>
	Vector<T, F>::Vector(const Vector<T, F>& rhs) :
		Vector(rhs.Capacity())
	{
		for (auto& itr : rhs)
		{
			PushBack(itr);
		}
	}

	template<typename T, typename F>
	Vector<T, F>::Vector(Vector&& rhs) :
		mSize(rhs.mSize), mCapacity(rhs.mCapacity), mDataArray(std::move(rhs.mDataArray))
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
	}

	template<typename T, typename F>
	Vector<T, F>& Vector<T, F>::operator=(const Vector<T, F>& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Reserve(rhs.Capacity());
			Vector<T, F>::Iterator itr = rhs.begin();
			for (; itr != rhs.end();++itr)
			{
				PushBack(*itr);
			}
		}
		return *this;
	}

	template<typename T, typename F>
	Vector<T, F>& Vector<T, F>::operator=(Vector&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mDataArray = std::move(rhs.mDataArray);

			rhs.mSize = 0;
			rhs.mCapacity = 0;
		}
		return *this;
	}

	template<typename T, typename F>
	Vector<T, F>::~Vector()
	{
		Clear();
	}

	template<typename T, typename F>
	T& Vector<T, F>::operator[](std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Vector index out of bounds.");
		}
		return mDataArray[index];
	}

	template<typename T, typename F>
	const T& Vector<T, F>::operator[](std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Vector index out of bounds.");
		}
		return mDataArray[index];
	}

	template<typename T, typename F>
	void Vector<T, F>::PopBack()
	{
		if (IsEmpty())
		{
			throw std::exception("ClearScreen operation. Vector is empty.");
		}

		mDataArray[mSize - 1].~T();
		mSize--;
	}

	template<typename T, typename F>
	T& Vector<T, F>::Front()
	{
		if (IsEmpty())
		{
			throw std::exception("ClearScreen operation. Vector is empty.");
		}
		return mDataArray[0];
	}

	template<typename T, typename F>
	const T& Vector<T, F>::Front() const
	{
		if (IsEmpty())
		{
			throw std::exception("ClearScreen operation. Vector is empty.");
		}
		return mDataArray[0];
	}

	template<typename T, typename F>
	T& Vector<T, F>::Back()
	{
		if (IsEmpty())
		{
			throw std::exception("ClearScreen operation. Vector is empty.");
		}
		return mDataArray[mSize-1];
	}

	template<typename T, typename F>
	const T& Vector<T, F>::Back() const
	{
		if (IsEmpty())
		{
			throw std::exception("ClearScreen operation. Vector is empty.");
		}
		return mDataArray[mSize - 1];
	}

	template<typename T, typename F>
	typename Vector<T, F>::Iterator Vector<T, F>::PushBack(const T& dataToPush)
	{
		// if list is full
		if (mSize == mCapacity)
		{
			std::uint32_t newCapacityValue = ReserveStrategyFunctor(mSize, mCapacity);
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

	template<typename T, typename F>
	bool Vector<T, F>::IsEmpty() const
	{
		return (0 == mSize);	// yoda statement
	}

	template<typename T, typename F>
	T& Vector<T, F>::At(std::uint32_t index)
	{
		return operator[](index);
	}

	template<typename T, typename F>
	void Vector<T, F>::Reserve(std::uint32_t newCapacity, bool fixedSize)
	{
		if (newCapacity > mCapacity)
		{
			mDataArray = (T*)realloc(mDataArray, newCapacity * sizeof(T));
			mCapacity = newCapacity;
			if (fixedSize)
			{
				for (std::uint32_t i = mSize; i < newCapacity; i++)
				{
					new (mDataArray + i)T();
				}
				mSize = newCapacity;
			}
		}
	}

	template<typename T, typename F>
	void Vector<T, F>::Clear()
	{
		for (std::uint32_t i = 0; i < mSize; i++)
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

	template<typename T, typename F>
	typename Vector<T, F>::Iterator Vector<T, F>::Find(const T& dataToFind) const
	{
		Vector<T, F>::Iterator itr = begin();
		for (; itr != end(); ++itr)
		{
			if (*itr == dataToFind)
			{
				break;
			}
		}
		return itr;
	}

	template<typename T, typename F>
	typename void Vector<T, F>::Remove(const T& dataToRemove)
	{
		Remove(Find(dataToRemove));
	}

	template<typename T, typename F>
	typename void Vector<T, F>::Remove(const Iterator& itr)
	{
		if (itr.mOwnerVector != this)
		{
			throw new std::exception("Iterator does not belong to this Vector");
		}

		if (itr != end())
		{
			(*itr).~T();
			// Note: mCurrentIndex of iterator is a 1 based index instead of being 0 based
			std::memmove(mDataArray + itr.mCurrentIndex - 1, mDataArray + itr.mCurrentIndex, sizeof(T) * (mSize - itr.mCurrentIndex));
			mSize--;
		}
	}

	template<typename T, typename F>
	typename void Vector<T, F>::Remove(Iterator itrBegin, Iterator itrEnd)
	{
		if (itrBegin.mOwnerVector != this || itrEnd.mOwnerVector != this)
		{
			throw new std::exception("Iterator does not belong to this Vector");
		}
		if (itrBegin.mCurrentIndex > itrEnd.mCurrentIndex)
		{
			throw new std::exception("ClearScreen range. Param 1 should be be less than Param 2");
		}

		Iterator itr = itrBegin;
		for (; itr != itrEnd; ++itr)
		{
			(*itr).~T();
		}

		std::memmove(mDataArray + itrBegin.mCurrentIndex - 1, mDataArray + itrEnd.mCurrentIndex - 1, sizeof(T) * (mSize - itrEnd.mCurrentIndex + 1));
		mSize = mSize - (itrEnd.mCurrentIndex - itrBegin.mCurrentIndex);
	}
	
	template<typename T, typename F>
	std::uint32_t Vector<T, F>::Size() const
	{
		return mSize;
	}

	template<typename T, typename F>
	std::uint32_t Vector<T, F>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T, typename F>
	typename Vector<T, F>::Iterator Vector<T, F>::begin() const
	{
		Vector<T, F>::Iterator itr(1, this);
		return itr;
	}

	template<typename T, typename F>
	typename Vector<T, F>::Iterator Vector<T, F>::end() const
	{
		Vector<T, F>::Iterator itr(mSize + 1, this);
		return itr;
	}

#pragma endregion

#pragma region Iterator

	template<typename T, typename F>
	Vector<T, F>::Iterator::Iterator() :
		mCurrentIndex(0), mOwnerVector(nullptr)
	{}

	template<typename T, typename F>
	Vector<T, F>::Iterator::Iterator(std::uint32_t currentIndex, const Vector* ownerVector) :
		mCurrentIndex(currentIndex), mOwnerVector(ownerVector)
	{}

	template<typename T, typename F>
	Vector<T, F>::Iterator::Iterator(const Iterator& rhs) :
		mCurrentIndex(rhs.mCurrentIndex), mOwnerVector(rhs.mOwnerVector)
	{}

	template<typename T, typename F>
	Vector<T, F>::Iterator::Iterator(Iterator&& rhs) :
		mCurrentIndex(rhs.mCurrentIndex), mOwnerVector(rhs.mOwnerVector)
	{
		rhs.mCurrentIndex = 0;
		rhs.mOwnerVector = nullptr;
	}

	template<typename T, typename F>
	typename Vector<T, F>::Iterator& Vector<T, F>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			mCurrentIndex = rhs.mCurrentIndex;
			mOwnerVector = rhs.mOwnerVector;
		}
		return *this;
	}

	template<typename T, typename F>
	typename Vector<T, F>::Iterator& Vector<T, F>::Iterator::operator=(Iterator&& rhs)
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

	template<typename T, typename F>
	bool Vector<T, F>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mOwnerVector == rhs.mOwnerVector) && (mCurrentIndex == rhs.mCurrentIndex);
	}

	template<typename T, typename F>
	bool Vector<T, F>::Iterator::operator!=(const Iterator & rhs) const
	{
		return !(*this == rhs);
	}

	template<typename T, typename F>
	typename Vector<T, F>::Iterator& Vector<T, F>::Iterator::operator++()
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

	template<typename T, typename F>
	typename Vector<T, F>::Iterator Vector<T, F>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template<typename T, typename F>
	T& Vector<T, F>::Iterator::operator*()
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

	template<typename T, typename F>
	const T& Vector<T, F>::Iterator::operator*() const
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

}