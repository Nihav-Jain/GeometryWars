#include "pch.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope, RTTI);

	const std::uint32_t Scope::DEFAULT_TABLE_SIZE;

	Scope::Scope(std::uint32_t initialTableSize) :
		mParent(nullptr), mSymbolTable(), mSymbolList(initialTableSize)
	{}

	Scope::Scope(const Scope& rhs) :
		mParent(nullptr), mSymbolTable(), mSymbolList(rhs.mSymbolList.Size())
	{
		RecursiveScopeChildrenCopy(rhs);
	}

	Scope::Scope(Scope&& rhs) :
		mParent(rhs.mParent), mSymbolTable(std::move(rhs.mSymbolTable)), mSymbolList(std::move(rhs.mSymbolList))
	{
		PointerFixupAfterMove(rhs);
		rhs.mParent = nullptr;
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
			mSymbolTable.Rehash(rhs.DEFAULT_TABLE_SIZE);
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

			PointerFixupAfterMove(rhs);

			rhs.mParent = nullptr;
		}

		return *this;
	}

	Datum* Scope::Find(const std::string& name) const
	{
		if (name.empty())
			return nullptr;

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
		{
			if(owner != nullptr)
				*owner = nullptr;
			return nullptr;
		}

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
		return scopeDatum.Get<Scope>(scopeDatum.Size() - 1);
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
					Scope* newScopeCopy = orderedSymbol->second.Get<Scope>(i).Clone(orderedSymbol->second.Get<Scope>(i)); // new Scope(*orderedSymbol->second.Get<Scope*>(i));
					AppendScope(orderedSymbol->first, *newScopeCopy);
				}
				if (orderedSymbol->second.Size() == 0U)
					Append(orderedSymbol->first);
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
					symbol->second.Get<Scope>(i).mParent = this;
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

	const Scope::SymbolPair& Scope::GetPair(std::uint32_t index) const
	{
		if (index >= mSymbolList.Size())
			throw std::exception("Index out of bounds");
		return *mSymbolList[index];
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

	const Datum& Scope::operator[](const std::string& name) const
	{
		Datum* datum = Find(name);
		assert(datum != nullptr);
		return *datum;
	}

	Datum& Scope::operator[](std::uint32_t index)
	{
		return (*mSymbolList[index]).second;
	}

	const Datum& Scope::operator[](std::uint32_t index) const
	{
		return (*mSymbolList[index]).second;
	}

	std::uint32_t Scope::Size() const
	{
		return mSymbolList.Size();
	}

	void Scope::Remove(const std::string& name)
	{
		SymbolTable::Iterator itr = mSymbolTable.Find(name);
		if (itr == mSymbolTable.end())
			return;
		
		SymbolPair& pair = *itr;
		mSymbolList.Remove(&pair);

		if (pair.second.Type() == Datum::DatumType::TABLE)
		{
			while (pair.second.Size() > 0)
			{
				pair.second.Get<Scope>(pair.second.Size() - 1).Clear();
				pair.second.Remove(pair.second.Size() - 1);
			}
		}
		pair.second.Clear();
		mSymbolTable.Remove(pair.first);
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		if (this == &rhs)
			return true;

		bool areEqual = false;
		if (mSymbolList.Size() == rhs.mSymbolList.Size())
		{
			areEqual = true;
			for (std::uint32_t i = 0; i < mSymbolList.Size(); ++i)
			{
				if (mSymbolList[i]->first == "this")
					continue;
				if (*mSymbolList[i] != *rhs.mSymbolList[i])
				{
					areEqual = false;
					break;
				}
			}
		}
		return areEqual;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

	std::string Scope::FindName(const Scope& child) const
	{
		std::string childScopeName;
		SymbolTable::Iterator itr = mSymbolTable.begin();
		for (; itr != mSymbolTable.end(); ++itr)
		{
			Datum& datum = itr->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				std::uint32_t i;
				for (i = 0; i < datum.Size(); i++)
				{
					if (&datum.Get<Scope>(i) == &child)
					{
						childScopeName = itr->first;
						break;
					}
				}
				if (i < datum.Size())
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
						Orphan(symbol->second, 0);
						delete &symbol->second.Get<Scope>();
					}
				}
			}
		}

		mSymbolList.Clear();
		mSymbolTable.Clear();
	}

	Scope* Scope::Clone(const Scope& rhs) const
	{
		return new Scope(rhs);
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

	bool Scope::IsAncestor(const Scope* child) const
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
					if (&symbol->second.Get<Scope>(i) == &child)
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
		RemoveChildScope(*containerDatum, index);
	}

	void Scope::RemoveChildScope(Datum& containerDatum, std::uint32_t index)
	{
		containerDatum.Remove(index);
	}

	void Scope::Orphan(Datum& containerDatum, std::uint32_t index)
	{
		if (mParent != nullptr)
			mParent->RemoveChildScope(containerDatum, index);
		mParent = nullptr;
	}

}

