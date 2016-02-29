#pragma once
#include "RTTI.h"
class FooRTTI : public Library::RTTI
{
	RTTI_DECLARATIONS(FooRTTI, RTTI);
public:
	/**
	*	Contructor of class Foo
	*	@param data integer value stored in Foo
	*/
	FooRTTI(int data = 10);

	/**
	*	Copy Contructor of class Foo
	*	@param reference to the right hand side variable
	*/
	FooRTTI(const FooRTTI& rhs);

	/**
	*	destructor
	*/
	virtual ~FooRTTI() override;

	/**
	*	Assignment operator override
	*	@param reference to the right hand side variable
	*	@return reference to the resultant Foo
	*/
	FooRTTI& operator= (const FooRTTI& rhs);

	/**
	*	Equality operator override
	*	@param reference to the right hand side variable
	*	@return boolean
	*/
	bool operator==(const FooRTTI& rhs) const;

	/**
	*	Not Equals operator override
	*	@param reference to the right hand side variable
	*	@return boolean
	*/
	bool operator!= (const FooRTTI& rhs) const;

	/**
	*	Getter for the member variable Data
	*	@return integer
	*/
	int GetData() const;

	/**
	*	Setter for the member variable Data
	*	@param data value to be copied into the member variable
	*	@return integer
	*/
	void SetData(int data);

	/**
	*	Getter for the pointer type member variable of Foo
	*	@return pointer to an integer
	*/
	int* GetPointerData() const;

private:
	int mData;
	int* mPointerData;
};

