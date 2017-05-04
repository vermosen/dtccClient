#include "registrable/derived.hpp"

namespace testSuite
{
	dtcc::registerType<base, std::string, derived, char>
		derived::register_(std::string("DERIVED"));
}
