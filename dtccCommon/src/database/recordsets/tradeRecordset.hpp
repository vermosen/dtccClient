/*
* aletheiaDatabase.hpp
*
*  Created on: Jul 3, 2016
*      Author: vermosen
*/

#ifndef DATABASE_RECORDSETS_TRADE_HPP_
#define DATABASE_RECORDSETS_TRADE_HPP_

#include <soci/soci.h>

#include "database/recordset.hpp"
#include "database/records/tradeRecord.hpp"

namespace dtcc
{
	namespace database
	{
		class tradeRecordset : public recordset<int64_t, tradeRecord>
		{
		public:
			tradeRecordset(boost::shared_ptr<connector> conn);

			virtual std::vector<keyType> insert(const std::vector<recordType> & records);
			virtual void update(const recordType & record);
			virtual void remove(const recordType & record);
			virtual void select(std::vector<recordType> & ret, const std::string & filter);
			virtual void select(const std::vector<keyType> & keys, std::vector<recordType> & ret);
		};
	}
}


#endif /* DATABASE_RECORDSETS_TRADE_HPP_ */