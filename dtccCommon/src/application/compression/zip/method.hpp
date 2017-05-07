#ifndef ZIP_METHOD_HPP_
#define ZIP_METHOD_HPP_

#include <stdint.h>

namespace dtcc
{
	namespace zip
	{
		enum method : uint16_t
		{
			noCompression				= 0,
			shrunk						= 1,
			reducedWCompressionFactor1	= 2,
			reducedWCompressionFactor2	= 3,
			reducedWCompressionFactor3	= 4,
			reducedWCompressionFactor4	= 5,
			imploded					= 6,
			deflated					= 8,
			enhancedDeflated			= 9,
			PKWareDCLImploded			= 10,
			compressedUsingBZIP2		= 12,
			LZMA						= 14,
			compressedUsingIBMTERSE		= 18,
			IBMLZ77z					= 19,
			PPMdVersionIRev1			= 98,
			reserved
		};
	}
}

#endif