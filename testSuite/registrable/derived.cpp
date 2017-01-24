#include "registrable/derived.hpp"

namespace testSuite
{
	dtcc::registerType<base, std::string, derived>
		derived::register_(std::string("DERIVED"));
}
