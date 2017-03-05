#ifndef UTILS_CONVERTER_HPP_
#define UTILS_CONVERTER_HPP_

namespace dtcc
{
	namespace util
	{
		// non-copy converter
		// see http://stackoverflow.com/questions/8703047/how-to-convert-a-byte-array-into-double-in-c
		// TODO: find a way to use this one...
		template<typename T1, typename T2>
		union converter
		{
			static_assert(sizeof(T1) == sizeof(T2), "size is incorrect");
			T1 t1_;
			T2 t2_;
		};
	}
}

#endif
