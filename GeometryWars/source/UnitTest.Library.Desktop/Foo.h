#pragma once

/**
 *	Class Foo for testing static library linking
 */
class Foo
{
public:
	/**
 	 *	Contructor of class Foo
	 *	@param data integer value stored in Foo
	 */
	Foo(int data = 10);

	/**
	 *	Copy Contructor of class Foo
	 *	@param reference to the right hand side variable
	 */
	Foo(const Foo& rhs);

	Foo(Foo&& rhs);

	/**
	 *	destructor
	 */
	~Foo();

	/**
	 *	Assignment operator override
	 *	@param reference to the right hand side variable
	 *	@return reference to the resultant Foo
	 */
	Foo& operator=(const Foo& rhs);

	Foo& operator=(Foo&& rhs);

	/**
	 *	Equality operator override
	 *	@param reference to the right hand side variable
	 *	@return boolean
	 */
	bool operator== (const Foo& rhs) const;

	/**
	 *	Not Equals operator override
	 *	@param reference to the right hand side variable
	 *	@return boolean
	 */
	bool operator!= (const Foo& rhs) const;

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

