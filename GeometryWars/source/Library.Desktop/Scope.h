#pragma once
#include "RTTI.h"
namespace Library
{
	/**
	 *	Class which forms a dynamic hierarchical database for Datum types
	 *	@inherits RTTI
	 */
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);
	public:
		typedef Hashmap<std::string, Datum> SymbolTable;
		typedef Vector<SymbolTable::PairType*> SymbolList;
		typedef SymbolTable::PairType SymbolPair;

		/**
		 *	Initialized the symbol table and list
		 *	@param size of the symbol table
		 */
		explicit Scope(std::uint32_t initialTableSize = DEFAULT_TABLE_SIZE);

		/**
		 *	Copy constructor
		 *	@param reference to the Scope to be copied from
		 */
		Scope(const Scope& rhs);

		/**
		 *	Move constructor
		 *	@param reference to the Scope to be moved
		 */
		Scope(Scope&& rhs);

		/**
		 *	destrcutor
		 */
		virtual ~Scope();

		/**
		 *	Copy assignment operator
		 *	@param reference to the Scope to be copied from
		 *	@return reference to this Scope
		 */
		Scope& operator=(const Scope& rhs);

		/**
		 *	Move assignment operator
		 *	@param reference to the Scope to be move
		 *	@return reference to this Scope
		 */
		Scope& operator=(Scope&& rhs);

		/**
		 *	looks for a datum stored with the given name within the current scope
		 *	@param key to be searched
		 *	@return pointer to the datum stored with the given key, nullptr if no sucj key existss
		 */
		Datum* Find(const std::string& name) const;

		/**
		 *	looks for a datum stored with the given name within the current ancestory
		 *	@param key to be searched
		 *	@param output parameter to store pointe to the scope where the Datum was found
		 *	@return pointer to the datum stored with the given key, nullptr if no sucj key existss
		 */
		Datum* Search(const std::string& name, Scope** owner = nullptr) const;

		/**
		 *	Adds a Datum with the given name, if it does not already exist, to this Scope
		 *	@param name of the Datum to be added
		 *	@return reference to the Datum in the Scope stored with the given name
		 */
		Datum& Append(const std::string& name);

		/**
		 *	Adds a Scope to the Datum stored with the given name
		 *	@param name of the Datum to which a new Scope will be added
		 *	@return reference to the newly added Scope
		 */
		Scope& AppendScope(const std::string& name);

		/**
		 *	Reparents the given Scope and stores it in the Datum represented by the given name
		 *	@param name of the Datum to which a new Scope will be added
		 */
		void Adopt(const std::string& name, Scope& child);

		/**
		 *	Removes this Scope from its parents list
		 */
		void Orphan();

		/**
		 *	Accesor for this Scope's parent
		 *	@return pointer to the parent Scope, nullptr if this is a root Scope
		 */
		Scope* GetParent() const;

		/**
		 *	Wrapper for Append
		 *	@param name of the Datum to be added
		 *	@return reference to the Datum in the Scope stored with the given name
		 */
		Datum& operator[](const std::string& name);

		/**
		 *	const overload - wrapper for Find
		 *	@param name of the datum to be searched
		 *	@return constant reference to the Datum in the Scope stored with the given name
		 *	@exception if there is no pair with the given name
		 */
		const Datum& operator[](const std::string& name) const;

		/**
		 *	Accesor and mutator for the Datum added to this Scope at the given order index
		 *	@param order in which Datums was added to the Scope
		 *	@return reference to the Datum in the Scope stored at the given order index
		 */
		Datum& operator[](std::uint32_t index);

		/**
		 *	Accesor for the Datum added to this Scope at the given order index
		 *	@param order in which Datums was added to the Scope
		 *	@return constant reference to the Datum in the Scope stored at the given order index
		 */
		const Datum& operator[](std::uint32_t index) const;

		/**
		 *	Accesor for the name-Datum pair added to this Scope at the given order index
		 *	@param order in which Datums was added to the Scope
		 *	@return constant reference to the name-Datum pair in the Scope stored at the given order index
		 */
		const SymbolPair& GetPair(std::uint32_t index) const;

		/**
		 *	Total number of name-datum pairs stored in this Scope
		 *	@return number of pairs
		 */
		std::uint32_t Size() const;

		/**
		 *	Removes the pair with the given name from itself
		 *	@param name corresponding to the datum to be removed
		 */
		void Remove(const std::string& name);

		/**
		 *	Equality operator - does a recursive check on all child scopes and datum values
		 *	@param reference to the Scope to be compared
		 *	@return true if both Scopes have identical data
		 */
		bool operator==(const Scope& rhs) const;

		/**
		 *	Not Equals operator - compliment of Equality operator
		 *	@param reference to the Scope to be compared
		 *	@return false if both Scopes have identical data
		 */
		bool operator!=(const Scope& rhs) const;

		/**
		 *	Gets the name of the Datum the given Scope is stored in
		 *	@param child scope to be searched
		 *	@return name of Datum if found, empty string if gievn Scope if not a child of this Scope
		 */
		std::string FindName(const Scope& child) const;

		/**
		 *	Destructive and recursive deletion of this Scope
		 */
		virtual void Clear();

		/**
		 *	acts as a virtual copy constructor
		 *	@param constant reference to the Scope to be copied / cloned
		 *	@return pointer to the newly instantiated Scope
		 */
		virtual Scope* Clone(const Scope& rhs) const;

		/**
		 *	Overriden from parent
		 *	@return name of class
		 */
		virtual std::string ToString() const override;

		/**
		 *	Overriden from parent
		 *	@param RTTI object to be compared with
		 *	@return if given RTTI is a Scope, then return result of equality opertor, false otherwise
		 */
		virtual bool Equals(const RTTI* rhs) const override;

	private:

		/**
		 *	Checks to see if given child is actually an ancestor of this Scope or not
		 *	@param pointer to Scope to be checked for ancestory
		 *	@return true if given scope lies in the upward ancestory of the current Scope
		 */
		bool IsAncestor(const Scope* child) const;

		/**
		 *	Finds the datum in which the given scope resides
		 *	@param reference to the Scope to be searched
		 *	@param output parameter - index in the Datum where this Scope was found
		 *	@return pointer to Datum where the given Scope was found, nullptr if no such Datum exists
		 */
		Datum* FindContainedScope(const Scope& child, std::uint32_t& index) const;

		/**
		 *	Removes the given scope from its parent Scope
		 *	@param reference to the Scope to be removed
		 */
		void RemoveChildScope(const Scope& child);

		/**
		 *	Overload used when we already know the Datum and index of the Scope to be removed
		 *	@param Datum where the scope resides
		 *	@param index of the scope in the Datum
		 */
		void RemoveChildScope(Datum& containerDatum, std::uint32_t index);

		/**
		 *	Overload used when we already know the Datum and index of the Scope to be orphaned in its parent
		 *	@param Datum where the scope resides
		 *	@param index of the scope in the Datum
		 */
		void Orphan(Datum& containerDatum, std::uint32_t index);

		/**
		 *	Overload used when we already already have allocated the scope we want to append
		 *	@param name of Datum in which the new scope will be stored
		 *	@param reference to Scope to be appended
		 *	@return reference to newly appended Scope
		 */
		Scope& AppendScope(const std::string& name, Scope& scopeToAppend);

		/**
		 *	Recursively copies the child Scopes in rhs into the current Scope
		 *	@param reference to Scope to be copied from
		 */
		void RecursiveScopeChildrenCopy(const Scope& rhs);

		/**
		 *	Fixes the parent pointers of the imeediate child Scopes and that of the Datum pointer of the parent
		 *	@param reference to Scope to be copied from
		 */
		void PointerFixupAfterMove(Scope& rhs);

		Scope* mParent;

		SymbolTable mSymbolTable;
		SymbolList mSymbolList;

		static const std::uint32_t DEFAULT_TABLE_SIZE = 13;
	};
}
