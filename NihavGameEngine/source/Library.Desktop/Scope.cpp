#include "pch.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope);

	const std::uint32_t Scope::DEFAULT_TABLE_SIZE;

	Scope::Scope(std::uint32_t initialTableSize) :
		mParent(nullptr), mSymbolTable(initialTableSize), mSymbolList(initialTableSize), mInitialSymbolTableSize(initialTableSize)
	{}

	Scope::Scope(const Scope& rhs) :
		mParent(nullptr), mSymbolTable(rhs.mInitialSymbolTableSize), mSymbolList(rhs.mSymbolTable.Size()), mInitialSymbolTableSize(rhs.mInitialSymbolTableSize)
	{
		RecursiveScopeChildrenCopy(rhs);
	}

	Scope::Scope(Scope&& rhs) :
		mParent(rhs.mParent), mSymbolTable(std::move(rhs.mSymbolTable)), mSymbolList(std::move(rhs.mSymbolList)), mInitialSymbolTableSize(rhs.mInitialSymbolTableSize)
	{
		PointerFixupAfterMove(rhs);
		rhs.mParent = nullptr;
		rhs.mInitialSymbolTableSize = 0;
	}

	Scope::~Scope()
	{
		Clear();
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mSymbolTable.Rehash(rhs.mInitialSymbolTableSize);
			mSymbolList.Reserve(rhs.mSymbolTable.Size());
			RecursiveScopeChildrenCopy(rhs);
		}

		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mParent = rhs.mParent;
			mSymbolTable = std::move(rhs.mSymbolTable);
			mSymbolList = std::move(rhs.mSymbolList);
			mInitialSymbolTableSize = rhs.mInitialSymbolTableSize;

			PointerFixupAfterMove(rhs);

			rhs.mParent = nullptr;
			rhs.mInitialSymbolTableSize = 0;
		}

		return *this;
	}

	Datum* Scope::Find(const std::string& name) const
	{
		if (name.empty())
			throw std::exception("Cannot find Datum with empty name");

		SymbolTable::Iterator itr = mSymbolTable.Find(name);
		if(itr == mSymbolTable.end())
			return nullptr;
		return &(itr->second);
	}

	Datum* Scope::Search(const std::string& name, Scope** owner) const
	{
		Datum* dataValue = Find(name);
		if (dataValue != nullptr)
		{
			if (owner != nullptr)
				*owner = const_cast<Scope*>(this);
			return dataValue;
		}

		if (mParent == nullptr)
			return nullptr;

		return mParent->Search(name, owner);
	}

	Datum& Scope::Append(const std::string& name)
	{
		if(name.empty())
			throw std::exception("Cannot find Datum with empty name");

		Datum datum;
		bool wasInserted;

		SymbolTable::Iterator insertItr = mSymbolTable.Insert(SymbolPair(name, datum), wasInserted);
		Datum* dataValue = &(insertItr->second);
	
		if(wasInserted)
			mSymbolList.PushBack(&(*insertItr));

		return *dataValue;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		Scope* newScope = new Scope();
		return AppendScope(name, *newScope);
	}

	Scope& Scope::AppendScope(const std::string& name, Scope& scopeToAppend)
	{
		Datum& scopeDatum = Append(name);
		if (scopeDatum.Type() == Datum::DatumType::TABLE || scopeDatum.Type() == Datum::DatumType::UNKNOWN)
		{
			scopeToAppend.Orphan();
			scopeToAppend.mParent = this;
			scopeDatum.Set(scopeToAppend, scopeDatum.Size());
		}
		else
		{
			delete &scopeToAppend;
			throw std::exception("Cannot change data type of Datum");
		}
		return *scopeDatum.Get<Scope*>(scopeDatum.Size() - 1);
	}

	void Scope::RecursiveScopeChildrenCopy(const Scope& rhs)
	{
		for (auto& orderedSymbol : rhs.mSymbolList)
		{
			if (orderedSymbol->second.Type() != Datum::DatumType::TABLE)
			{
				Datum& newDatumCopy = Append(orderedSymbol->first);
				newDatumCopy = orderedSymbol->second;	// invoke Datum copy assignment operator
			}
			else
			{
				for (std::uint32_t i = 0U; i < orderedSymbol->second.Size(); ++i)
				{
					Scope* newScopeCopy = new Scope(*orderedSymbol->second.Get<Scope*>(i));
					AppendScope(orderedSymbol->first, *newScopeCopy);
				}
			}
		}
	}

	void Scope::PointerFixupAfterMove(Scope& rhs)
	{
		for (auto& symbol : mSymbolList)
		{
			if (symbol->second.Type() == Datum::DatumType::TABLE)
			{
				for (std::uint32_t i = 0; i < symbol->second.Size(); ++i)
				{
					symbol->second.Get<Scope*>(i)->mParent = this;
				}
			}
		}
		if (mParent != nullptr)
		{
			std::uint32_t index;
			Datum* myContainerDatum = mParent->FindContainedScope(rhs, index);
			if (myContainerDatum != nullptr)
			{
				myContainerDatum->Set(*this, index);
			}
		}
	}

	void Scope::Adopt(const std::string& name, Scope& child)
	{
		if (IsAncestor(&child))
		{
			throw std::exception("A child cannot adopt its own ancestor.");
		}

		AppendScope(name, child);
	}

	void Scope::Orphan()
	{
		if(mParent != nullptr)
			mParent->RemoveChildScope(*this);
		mParent = nullptr;
	}

	Scope* Scope::GetParent() const
	{
		return mParent;
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	Datum& Scope::operator[](std::uint32_t index)
	{
		return (*mSymbolList[index]).second;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		bool areEqual = false;
		if (mSymbolList.Size() == rhs.mSymbolList.Size())
		{
			areEqual = true;
			SymbolList::Iterator rhsItr = rhs.mSymbolList.begin();
			for (auto& symbol : mSymbolList)
			{
				if (*symbol != **rhsItr)
				{
					areEqual = false;
					break;
				}
				++rhsItr;
			}
		}
		return areEqual;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

	std::string Scope::FindName(const Scope* child) const
	{
		std::string childScopeName;
		SymbolTable::Iterator itr = mSymbolTable.begin();
		for (; itr != mSymbolTable.end(); ++itr)
		{
			if (itr->second.Type() == Datum::DatumType::TABLE)
			{
				std::uint32_t i;
				for (i = 0; i < itr->second.Size(); i++)
				{
					if (itr->second.Get<Scope*>(i) == child)
					{
						childScopeName = itr->first;
						break;
					}
				}
				if (i < itr->second.Size())
					break;
			}
		}

		return childScopeName;
	}

	void Scope::Clear()
	{
		Orphan();

		if (mSymbolList.Size() > 0)
		{
			for (auto& symbol : mSymbolList)
			{
				if (symbol->second.Type() == Datum::DatumType::TABLE)
				{
					while (symbol->second.Size() > 0U)
					{
						Orphan(&symbol->second, 0);
						delete symbol->second.Get<Scope*>();
					}
				}
			}
		}

		mSymbolList.Clear();
		mSymbolTable.Clear();
	}

	std::string Scope::ToString() const
	{
		return "Scope";
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (!rhs->Is("Scope"))
			return false;
		return operator==(*rhs->As<Scope>());
	}

	bool Scope::IsAncestor(Scope* child) const
	{
		if (child == this)
			return true;
		if (mParent == nullptr)
			return false;
		return mParent->IsAncestor(child);
	}

	Datum* Scope::FindContainedScope(const Scope& child, std::uint32_t& index) const
	{
		Datum* childContainer = nullptr;
		for (auto& symbol : mSymbolList)
		{
			if (symbol->second.Type() == Datum::DatumType::TABLE)
			{
				std::uint32_t i;
				for (i = 0U; i < symbol->second.Size(); i++)
				{
					if (symbol->second.Get<Scope*>(i) == &child)
					{
						childContainer = &symbol->second;
						index = i;
						break;
					}
				}
				if (i < symbol->second.Size())
					break;
			}
		}
		return childContainer;
	}

	void Scope::RemoveChildScope(const Scope& child)
	{
		std::uint32_t index = 0;
		Datum* containerDatum = FindContainedScope(child, index);
		if (containerDatum == nullptr)
		{
			throw std::exception("Given scope is not my child.");
		}
		RemoveChildScope(containerDatum, index);
	}

	void Scope::RemoveChildScope(Datum* containerDatum, std::uint32_t index)
	{
		containerDatum->Remove(index);
	}

	void Scope::Orphan(Datum* containerDatum, std::uint32_t index)
	{
		if (mParent != nullptr)
			mParent->RemoveChildScope(containerDatum, index);
		mParent = nullptr;
	}

}

