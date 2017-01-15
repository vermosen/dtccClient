#ifndef PATTERN_SINGLETON_HPP_
#define PATTERN_SINGLETON_HPP_

#include <boost/noncopyable.hpp>

namespace dtcc
{
	template <class T>
	class singleton : private boost::noncopyable
	{
	public:
		//! access to the unique instance
		static T& instance();
	protected:
		singleton() {}
	};

	template <class T>
	T& singleton<T>::instance()
	{
		static boost::shared_ptr<T> instance_;

		if (!instance_)
			instance_ = boost::shared_ptr<T>(new T);
		return *instance_;
	}
}

#endif
