#define DOUBLE Double

#define FRACTION_NBIT 30   //number of fraction bits, constant parameter on chip
#define TOLERANCE 1e-6     //tolerance of residuum
#define MAX_SIZE 1024      //maximum size of vector/matrix
#define MAX_ELEM (MAX_SIZE*16)   //maximum number of none-zero elements in system matrix(sparse)
#define FOR_DEBUG          //functions declared with attribute 'FOR_DEBUG' is dedicated to debugging, and should not be included in RTL implementation

#ifndef _WIN32
#define _ASSERT(v)
#endif

/**
* Fix point struct with varialble components
*/
struct Fix
{
	unsigned long long fraction : FRACTION_NBIT;	/**< fraction part with varriable number of bits, which are defined by macro FRACTION_NBIT */
	long long integer : (64 - FRACTION_NBIT);		/**< integer part with variable number of bits, which are determined by (64 - FRACTION_NBIT) */
};

/**
* Fix point struct
*/
struct Long
{
	unsigned int fraction;							/**< 32 bits fraction part */
	int integer;									/**< 32 bits integer part */
};

/**
* Fix point substitution for double precision reprensentation of a real number
* It defined the 4 fundalment operations of fix point numbers
*/
struct Double
{
	//////////////////////////////////////* public member varriables */////////////////////////////////////////////////////////////
	long long data;									/**< struct member, 64 bits fix point number */
	
	//////////////////////////////////////* public member functions */////////////////////////////////////////////////////////////
	Double();										/**< default construction function */
	Double(const double &value);					/**< construct a Double struct from a double value */

	int check_shift() const;						/**< check how many bits atmost can the member 'data' be shifted left */

	Double operator + (const Double &value) const;	/**< addition operator */
	Double operator - (const Double &value) const;	/**< subtraction operator */
	Double operator * (const Double &value) const;	/**< multiplication operator */
	Double operator / (const Double &value) const;	/**< division operator */

	bool operator > (const Double &value)const;		/**< check whether it is greater than another value or not */
	bool operator < (const Double &value)const;		/**< check whether it is less than another value or not */
	bool operator == (const Double &value)const;	/**< check whether it is equals to another value or not */

	Double& operator += (const Double& value);		/**< additive assignment operator */
	Double& operator -= (const Double& value);		/**< subtraction assignment operator */
	Double& operator *= (const Double& value);		/**< multiplication assignment operator */
	Double& operator /= (const Double& value);		/**< division assignment operator */

	operator double(void) const;					/**< 'double' type cast, designed for debugging only */
	bool check_overflow(double value) const;		/**< check if bits overflow is occurred during calculation */
	bool check_result(double v1, double v2) const;  /**< check if the absolute difference of two values is less than a certain tolerance */
};

/**
* 64 bits data union, interpreting 64 bits data as various type
*/
union Data
{
	Double d;										
	Fix f;
	Long l;

	Data() {};										/**< empty construction function, doing nothing */
};

