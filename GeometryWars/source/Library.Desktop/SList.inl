#include "pch.h"
#include "SList.h"

namespace Library
{
	template <typename T>
	SList<T>::SList() :
		mFront(nullptr), mBack(nullptr), mSize(0)
	{
	}

	template <typename T>
	SList<T>::SList(const SList<T>& rhs) :
		SList()
	{
		operator=(rhs);
	}

	template <typename T>
	SList<T>::SList(SList<T>&& rhs) :
		mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack)
	{
		rhs.mSize = 0;
		rhs.mBack = nullptr;
		rhs.mFront = nullptr;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(SList<T>&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mSize = rhs.mSize;
			mFront = rhs.mFront;
			mBack = rhs.mBack;

			rhs.mSize = 0;
			rhs.mBack = nullptr;
			rhs.mFront = nullptr;
		}
		return *this;
	}


	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList<T>& rhs)
	{
		if (this != &rhs)
		{
			// clear the list corresponding to the this object
			Clear();

			// deep copy the rhs list
			SList<T>::Iterator itr = rhs.begin();
			for (; itr != rhs.end(); ++itr)
			{
				PushBack(*itr);
			}
		}
		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& item)
	{
		mFront = new Node(item, mFront);
		// if this is the first item in the list
		if (IsEmpty())
		{
			mBack = mFront;
		}
		mSize++;
		return Iterator(mFront, this);
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		// if the list is empty, throw an exception
		if (IsEmpty())
		{
			throw std::exception("ClearScreen operation. List is empty.");
		}
		// delete the Front node and update the Front pointer to the next Node in the list
		Node* temp = mFront->Next;

		delete mFront;
		mFront = temp;
		mSize--;

		if (IsEmpty())
		{
			mBack = nullptr;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& item)
	{
		Node* temp = new Node(item);
		if (IsEmpty())
		{
			mBack = temp;
			mFront = mBack;
		}
		else
		{
			mBack->Next = temp;
			mBack = mBack->Next;
		}
		mSize++;
		return Iterator(mBack, this);
	}

	template <typename T>
	bool SList<T>::IsEmpty() const
	{
		return (mSize == 0);
	}

	template <typename T>
	T& SList<T>::Front()
	{
		if (mFront == nullptr)
		{
			throw std::exception("ClearScreen operation. List is empty.");
		}
		return mFront->mData;
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (mFront == nullptr)
		{
			throw std::exception("ClearScreen operation. List is empty.");
		}
		return mFront->mData;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		if (mBack == nullptr)
		{
			throw std::exception("ClearScreen operation. List is empty.");
		}
		return mBack->mData;
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (mBack == nullptr)
		{
			throw std::exception("ClearScreen operation. List is empty.");
		}
		return mBack->mData;
	}


	template <typename T>
	std::uint32_t SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopFront();
		}
		mFront = nullptr;
		mBack = nullptr;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		SList<T>::Iterator itr(mFront, this);
		return itr;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		SList<T>::Iterator itr(nullptr, this);
		return itr;
	}

	template <typename T>
	typename void SList<T>::InsertAfter(const T& dataToInsert, const Iterator& itr)
	{
		if (itr.mOwnerList != this)
		{
			throw std::exception("Argument Iterator does not belong to the invoked SList.");
		}
		if (itr.mCurrentNode == nullptr)
		{
			PushBack(dataToInsert);
		}
		else
		{
			Node* nodeToInsert = new Node(dataToInsert);
			nodeToInsert->Next = itr.mCurrentNode->Next;
			itr.mCurrentNode->Next = nodeToInsert;
			mSize++;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& dataToFind) const
	{
		SList<T>::Iterator itr = begin();
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
	typename void SList<T>::Remove(const T& dataToRemove)
	{
		SList<T>::Iterator itrToRemove = begin();
		if (!IsEmpty())
		{
			// if the first element of the list has to be removed
			if (*itrToRemove == dataToRemove)
			{
				PopFront();
			}
			else
			{
				++itrToRemove;
				SList<T>::Iterator previousItr = begin();
				for (; itrToRemove != end(); ++itrToRemove)
				{
					if (*itrToRemove == dataToRemove)
					{
						(previousItr.mCurrentNode)->Next = (itrToRemove.mCurrentNode)->Next;
						if (itrToRemove.mCurrentNode == mBack)
						{
							mBack = previousItr.mCurrentNode;
						}
						mSize--;
						delete itrToRemove.mCurrentNode;

						break;
					}
					++previousItr;
				}
			}

		}
	}

	template <typename T>
	SList<T>::Node::Node(const T& data, Node* nextNode) :
		mData(data), Next(nextNode)
	{}

#pragma region Iterator

	template <typename T>
	SList<T>::Iterator::Iterator() :
		mCurrentNode(nullptr), mOwnerList(nullptr)
	{}

	template <typename T>
	SList<T>::Iterator::Iterator(Node* currentNode, const SList<T>* ownerList) :
		mCurrentNode(currentNode), mOwnerList(ownerList)
	{}

	template <typename T>
	SList<T>::Iterator::Iterator(const Iterator& rhs) :
		mCurrentNode(rhs.mCurrentNode), mOwnerList(rhs.mOwnerList)
	{}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(const Iterator& rhs)
	{
		if (this != &rhs)
		{
			mCurrentNode = rhs.mCurrentNode;
			mOwnerList = rhs.mOwnerList;
		}
		return *this;
	}

	template <typename T>
	SList<T>::Iterator::Iterator(Iterator&& rhs) :
		mCurrentNode(rhs.mCurrentNode), mOwnerList(rhs.mOwnerList)
	{
		rhs.mCurrentNode = nullptr;
		rhs.mOwnerList = nullptr;
	}


	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator=(Iterator&& rhs)
	{
		if (this != &rhs)
		{
			mCurrentNode = rhs.mCurrentNode;
			mOwnerList = rhs.mOwnerList;
			rhs.mCurrentNode = nullptr;
			rhs.mOwnerList = nullptr;
		}
		return *this;
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return (mOwnerList == rhs.mOwnerList) && (mCurrentNode == rhs.mCurrentNode);
	}

	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(*this == rhs);
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mOwnerList == nullptr)
		{
			throw std::exception("Non hosted Iterator. Iterator not attached to a valid SList.");
		}
		if (mCurrentNode == nullptr)
		{
			throw std::exception("Null pointer exception. Iterator points to end of SList");
		}
		mCurrentNode = mCurrentNode->Next;
		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template <typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (mOwnerList == nullptr)
		{
			throw std::exception("Non hosted Iterator. Iterator not attached to a valid SList.");
		}
		if (mCurrentNode == nullptr)
		{
			throw std::exception("Null pointer exception. Iterator points to end of SList");
		}
		return mCurrentNode->mData;
	}

#pragma endregion

}