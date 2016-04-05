#pragma once

/**
 *	Class Bar for testing static library linking
 */
class Bar
{
public:
	/**
 	 *	Contructor of class Bar
	 *	@param data integer value stored in Bar
	 */
	Bar(int data = 10);

	/**
	 *	Copy Contructor of class Bar
	 *	@param reference to the right hand side variable
	 */
	Bar(const Bar& rhs);

	Bar(Bar&& rhs);

	/**
	 *	destructor
	 */
	~Bar();

	/**
	 *	Assignment operator override
	 *	@param reference to the right hand side variable
	 *	@return reference to the resultant Bar
	 */
	Bar& operator=(const Bar& rhs);

	Bar& operator=(Bar&& rhs);

	/**
	 *	Equality operator override
	 *	@param reference to the right hand side variable
	 *	@return boolean
	 */
	bool operator== (const Bar& rhs) const;

	/**
	 *	Not Equals operator override
	 *	@param reference to the right hand side variable
	 *	@return boolean
	 */
	bool operator!= (const Bar& rhs) const;

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
	 *	Getter for the pointer type member variable of Bar
	 *	@return pointer to an integer
	 */
	int* GetPointerData() const;

private:
	int mData;
	int* mPointerData;
};

