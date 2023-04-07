#ifndef ATOMICCOUNTER_H
#define ATOMICCOUNTER_H
#include <atomic>
namespace PH {
class AtomicCounter{
public:
	typedef int ValueType; 

	AtomicCounter();

	explicit AtomicCounter(ValueType initialValue);

	AtomicCounter(const AtomicCounter& counter);


	~AtomicCounter();


	AtomicCounter& operator = (const AtomicCounter& counter);


	AtomicCounter& operator = (ValueType value);

	operator ValueType () const;

	ValueType value() const;

	ValueType operator ++ (); // prefix

	ValueType operator ++ (int); // postfix

	ValueType operator -- (); // prefix

	ValueType operator -- (int); // postfix

	bool operator ! () const;

private:
	std::atomic<int> _counter;
};


//
// inlines
//

inline AtomicCounter::operator AtomicCounter::ValueType() const
{
	return _counter.load();
}


inline AtomicCounter::ValueType AtomicCounter::value() const
{
	return _counter.load();
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ () // prefix
{
	return ++_counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator ++ (int) // postfix
{
	return _counter++;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- () // prefix
{
	return --_counter;
}


inline AtomicCounter::ValueType AtomicCounter::operator -- (int) // postfix
{
	return _counter--;
}


inline bool AtomicCounter::operator ! () const
{
	return _counter.load() == 0;
}
}
#endif // ATOMICCOUNTER_H
