#pragma once

#include <cstdint>
#include <string>

#include "glm\glm.hpp"
#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"

namespace Library
{
	/**
	 *	Abstract class to serve as a base class for native C++ classes available to the scipting system
	 *	Override the DefinePrescribedAttributes() method to add any prescribed attributes for your derived Attributed class
	 *	Call Populate() in your derived class' constructor to add your prescribed signatures to the run-time data-type system
	 */
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:

		/**
		 *	Default constructor
		 */
		Attributed();

		/**
		 *	Copy constructor (deep copy)
		 *	@param reference to the Attributed variable to be copied from
		 */
		Attributed(const Attributed& rhs);

		/**
		 *	Move constructor
		 *	@param R value reference to the Attributed variable to be moved
		 */
		Attributed(Attributed&& rhs);

		/**
		 *	Assignment operator
		 *	@param reference to the Attributed variable to be copied from
		 *	@return reference to the newly copied Attributed
		 */
		Attributed& operator=(const Attributed& rhs);

		/**
		 *	Assignment operator
		 *	@param R value reference to the Attributed variable to be moved
		 *	@return reference to the newly moved Attributed
		 */
		Attributed& operator=(Attributed&& rhs);

		/**
		 *	Checks if given name exists as a prescribed attribute
		 *	@param name of the attribute
		 *	@return true if the derived Attributed class has a prescribed attribute with the given name, false otherwise
		 */
		bool IsPrescribedAttribute(const std::string& name) const;

		/**
		 *	Checks if given name exists as an auxiliary attribute
		 *	@param name of the attribute
		 *	@return true if the derived Attributed class has an auxiliary attribute with the given name, false otherwise
		 */
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/**
		 *	Checks if given name exists as an attribute
		 *	@param name of the attribute
		 *	@return true if the derived Attributed class has an attribute with the given name, false otherwise
		 */
		bool IsAttribute(const std::string& name) const;

		/**
		 *	Add an auxiliary attribute to the current instance of the derived Attributed class
		 *	@param name of the attribute
		 *	@return reference to the Datum which will contain the value for the auxiliary attribute and is associated with the given name
		 *	@exception cannot add an auxiliary attribute with the same name as a prescribed attribute
		 */
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/**
		 *	Add an auxiliary attribute of type table to the current instance of the derived Attributed class
		 *	@param name of the attribute
		 *	@return reference to the Scope which will contain the value for the auxiliary attribute and is associated with the given name
		 *	@exception cannot add an auxiliary attribute with the same name as a prescribed attribute
		 */
		Scope& AppendAuxiliaryScope(const std::string& name);

		/**
		 *	Returns the index in the Scope order vector from where auxiliary attributes of the derived Attributed class instance will start
		 *	@return number of prescribed attributes
		 */
		std::uint32_t AuxiliaryBegin() const;

		/**
		 *	Deep copies the auxiliary attributes of the given Attributed into itself.
		 *	If any attributes of the same name already exist, they are overriden by the new values
		 *	@param constant reference to the Attributed whose auxiliary attributes are to be copied
		 */
		void CopyAuxiliaryAttributesFromAnotherAttributed(const Attributed& anotherAttributed);

		/**
		 *	Destructor
		 */
		virtual ~Attributed() = default;

		/**
		 *	Overriden from Scope, destroys all stored attributes and their memory
		 */
		virtual void Clear() override;

		static void ClearStaticMembers();

	protected:
		/**
		 *	Adds a signature as an internal prescribed attribute, see overloads for various supported data types
		 *	@param name of the attribute
		 *	@param initial (default) value of the attribute
		 *	@param (optional) number of values of the current data type to be associated with the given attribute name
		 */
		void AddInternalAttribute(const std::string& name, std::int32_t initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, std::float_t initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, const std::string& initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, const char* initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, const glm::vec4& initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, const glm::mat4& initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, bool initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, Datum* initialValue, std::uint32_t size = 0);

		/**
		 *	Adds a nested scope as a prescribed attribute
		 *	@param name of the attribute
		 *	@param (optional) initial (default) value of the attribute
		 *	@param (optional) number of values of the current data type to be associated with the given attribute name
		 */
		void AddNestedScope(const std::string& name);
		void AddNestedScope(const std::string& name, Scope& initialValue, std::uint32_t size = 0);

		/**
		 *	Adds a signature as an external prescribed attribute, see overloads for various supported data types
		 *	@param name of the attribute
		 *	@param number of values of the current data type to be associated with the given attribute name
		 *	@param pointer to the external storage
		 */
		void AddExternalAttribute(const std::string& name, std::uint32_t size, std::int32_t* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, std::float_t* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, std::string* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, glm::vec4* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, glm::mat4* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, bool* storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, RTTI** storage);
		void AddExternalAttribute(const std::string& name, std::uint32_t size, Datum** storage);

	private:
		void DefineUniqueAttributeName(const std::string& name);
		Datum& AddEmptyInternalSignature(const std::string& name, Datum::DatumType type, std::uint32_t size);
		Datum& AddEmptySignature(const std::string& name);

		bool IsParentPrescribedAttribute(const std::string& attributeName, const std::uint64_t* parentTypeIdPtr) const;
		std::uint32_t PrescribedAttributeCount(const std::uint64_t* parentTypeIdPtr) const;

		static Hashmap<std::uint64_t, Vector<std::string>> mPrescribedAttributes;
	};
}
