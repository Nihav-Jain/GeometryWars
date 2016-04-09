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

		Scope& AppendAuxiliaryScope(const std::string& name);

		/**
		 *	Returns the index in the Scope order vector from where auxiliary attributes of the derived Attributed class instance will start
		 *	@return number of prescribed attributes
		 */
		std::uint32_t AuxiliaryBegin() const;

		/**
		 *	Destructor
		 */
		virtual ~Attributed() = default;

		/**
		 *	Overriden from Scope, destroys all stored attributes and their memory
		 */
		virtual void Clear() override;

	protected:
		/**
		 *	Implement this function and add signatures for the prescribed attributes for the derived Attributed class in it
		 */
		virtual void DefinePrescribedAttributes() = 0;

		/**
		 *	Should be called in the constructor of the derived Attributed class, creates prescribed attributes as run-time data types
		 */
		void Populate();

		/**
		 *	Adds a signature as an internal prescribed attribute, see overloads for various supported data types
		 *	@param name of the attribute
		 *	@param initial (default) value of the attribute
		 *	@param (optional) number of values of the current data type to be associated with the given attribute name
		 */
		void AddInternalAttribute(const std::string& name, std::int32_t initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, std::float_t initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, const std::string& initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, const glm::vec4& initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, const glm::mat4& initialValue, std::uint32_t size = 0);
		void AddInternalAttribute(const std::string& name, RTTI* initialValue, std::uint32_t size = 0);

		/**
		 *	Adds a nested scope as a prescribed attribute
		 *	@param name of the attribute
		 *	@param (optional) initial (default) value of the attribute
		 *	@param (optional) number of values of the current data type to be associated with the given attribute name
		 */
		void AddNestedScope(const std::string& name, std::uint32_t size = 0);
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
		void AddExternalAttribute(const std::string& name, std::uint32_t size, RTTI** storage);

	private:

		/**
		 *	Helper class to maintain signatures for the prescribed attributes
		 */
		class Signature
		{
		public:
			Signature() = default;
			Signature(const std::string& name, Datum& datum);
			Signature(const Signature& rhs) = default;
			Signature(Signature&& rhs);

			~Signature() = default;

			Signature& operator=(const Signature& rhs) = default;
			Signature& operator=(Signature&& rhs);

			std::string& Name();
			Datum& SignatureDatum();
		private:
			std::string mName;
			Datum mSignatureDatum;
		};

		bool IsSignatureNameDefined(const std::string& name) const;
		Datum& AddEmptyInternalSignature(const std::string& name, Datum::DatumType type, std::uint32_t size);
		Datum& AddEmptySignature(const std::string& name);

		Vector<Signature> mSignatures;
		Vector<std::string> mPrescribedAttributes;

	};
}

