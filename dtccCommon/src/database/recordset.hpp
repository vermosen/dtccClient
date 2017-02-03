/*
* recordset.hpp
*
*  Created on: Jul 3, 2016
*      Author: vermosen
*/

#ifndef RECORDSET_HPP_
#define RECORDSET_HPP_

#include <vector>

#include "connector.hpp"

namespace dtcc
{
	namespace database
	{
		/*
		* K is the key, T is the record
		*/
		template <typename K, typename T>
		class recordset
		{
		public:
			typedef K keyType;
			typedef T recordType;
		public:
			recordset(boost::shared_ptr<connector> conn) : connector_(conn) {}
			virtual ~recordset() {};

			// implement crud operations
			virtual std::vector<keyType> insert(const std::vector<recordType> & record) = 0;
			virtual void update(const recordType & record) = 0;
			virtual void remove(const recordType & record) = 0;
			virtual void select(std::vector<recordType> & ret, const std::string & filter) = 0;
			virtual void select(const std::vector<keyType> & keys, std::vector<recordType> & ret) = 0;

		protected:
			boost::shared_ptr<connector> connector_;
		};
	}
}
#endif /* RECORDSET_HPP_ */