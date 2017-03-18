#ifndef ABSTRACT_FACTORY_HPP
#define ABSTRACT_FACTORY_HPP

#include <map>
#include <boost/shared_ptr.hpp>

// an abstract factory class with self registration
namespace dtcc
{
	template <typename C, typename K>
	class abstractFactory
	{

	protected:
		typedef K key_type;
		typedef std::map<key_type, C*(*)()> map_type;

	public:
		static boost::shared_ptr<C> createInstance(const K & key)
		{
			typename map_type::iterator it = getMap()->find(key);

			if (it == getMap()->end())
				throw std::runtime_error("unknown key found !");
			else
				return boost::shared_ptr<C>(it->second());
		}

	protected:
		static boost::shared_ptr<map_type> getMap()
		{
			if (!map_) { map_ = boost::shared_ptr<map_type>(new map_type); }
			return map_;
		}

		static boost::shared_ptr<map_type> map_;

	};

	// create function
	template <typename C, typename T> C * create() { return new T; }

	//registration struct
	template <typename C, typename K, typename T>
	struct registerType : abstractFactory<C, K>
	{
		registerType(const K & key)
		{
			abstractFactory<C, K>::getMap()->insert(std::make_pair(key, &create<C, T>));
		}
	};
}
#endif
