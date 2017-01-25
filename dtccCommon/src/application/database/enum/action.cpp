#include "application/database/enum/action.hpp"

namespace dtcc
{
	namespace database
	{
		std::ostream & operator << (std::ostream &out, const action&value)
		{
			switch (value)
			{
			case action::new_:
				out << "NEW";
				break;
			case action::cancel:
				out << "CANCEL";
				break;
			case action::correct:
				out << "CORRECT";
				break;
			}

			return out;
		}
	}
}
