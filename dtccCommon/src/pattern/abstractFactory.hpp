#ifndef abstract_factory_hpp
#define abstract_factory_hpp

#include <map>
#include <boost/shared_ptr.hpp>

// an abstract factory class 
// with self management functionalities
namespace dtcc
{
	template <typename C, typename K>
	class factory
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
		static map_type * getMap()
		{
			if (!map_) { map_ = new map_type; }
			return map_;
		}

		static map_type * map_;

	};

	// create function
	template <typename C, typename T> C * create() { return new T; }

	//registration struct
	template <typename C, typename K, typename T>
	struct registerType : factory<C, K>
	{
		registerType(const K & key)
		{
			factory<C, K>::getMap()->insert(std::make_pair(key, &create<C, T>));
		}
	};
}
#endif
