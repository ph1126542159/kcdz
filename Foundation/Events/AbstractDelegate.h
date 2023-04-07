#ifndef FOUNDATION_AbstractDelegate_INCLIUDED
#define FOUNDATION_AbstractDelegate_INCLIUDED

namespace PH
{

	template <class TArgs>
	class AbstractDelegate
	{
	public:
		AbstractDelegate()
		{

		}

		AbstractDelegate(const AbstractDelegate&) {

		}


		virtual ~AbstractDelegate()
		{

		}

		virtual bool notify(const void* sender, TArgs& argument) = 0;

		virtual bool equals(const AbstractDelegate& other)const = 0;

		virtual AbstractDelegate* clone()const = 0;

		virtual void disable() = 0;

		virtual const AbstractDelegate* unwarp()const
		{
			return this;
		}
	};


	template<>
	class AbstractDelegate<void>
	{
	public:
		AbstractDelegate()
		{

		}

		AbstractDelegate(const AbstractDelegate&)
		{

		}

		virtual ~AbstractDelegate()
		{

		}

		virtual bool notify(const void* sender) = 0;

		virtual bool equals(const AbstractDelegate& other)const = 0;

		virtual AbstractDelegate* clone()const = 0;

		virtual void disable() = 0;

		virtual const AbstractDelegate* unwarp() const
		{
			return this;
		}
	};

}

#endif // !FOUNDATION_AbstractDelegate_INCLIUDED

