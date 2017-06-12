#ifndef DTCC_TEST_CASSANDRA_CPP_
#define DTCC_TEST_CASSANDRA_CPP_

#include "cassandra.hpp"

#include <boost/iostreams/filtering_stream.hpp>

namespace testSuite
{
	boost::shared_ptr<dtcc::database::cassandra::connector> cassandra::cnx_;

	void cassandra::connect()
	{
		BOOST_TEST_MESSAGE("Testing boost compression using zlib...");

		try
		{
			std::string expected = "hello world";
			std::vector<std::string> nodes = { "127.0.0.1" };
			auto cluster = boost::shared_ptr<dtcc::database::cassandra::cluster>(new dtcc::database::cassandra::cluster(nodes));
			cnx_.reset(new dtcc::database::cassandra::connector());
			cnx_->setCluster(cluster);
			cnx_->connect();

			BOOST_TEST_MESSAGE("Testing boost compression using zlib...");
		}
		catch (const std::exception & ex)
		{

			BOOST_ERROR("\n Failed to connect the database: " << ex.what());
		}
 	}

	void cassandra::createTable()
	{
		std::string ks = "test";
		std::string tbl = "test";

		std::stringstream ss;

		ss  << "CREATE KEYSPACE IF NOT EXISTS" << ks
			<< "WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 1 }"
			<< "AND DURABLE_WRITES = true;";

		ss.str(""); ss.clear();

		ss  << "CREATE TABLE IF NOT EXISTS " << ks << "." << tbl
			<< "("
			<< "	DISSEMINATION_ID                                    BIGINT PRIMARY KEY	,"
			<< "	ORIGINAL_DISSEMINATION_ID                           BIGINT				,"
			<< "	ACTION                                              TINYINT				,"
			<< "	EXECUTION_TIMESTAMP                                 TIMESTAMP			,"
			<< "	CLEARED                                             BOOLEAN				,"
			<< "	INDICATION_OF_COLLATERALIZATION                     TINYINT				,"
			<< "	INDICATION_OF_END_USER_EXCEPTION                    BOOLEAN				,"
			<< "	INDICATION_OF_OTHER_PRICE_AFFECTING_TERM            BOOLEAN				,"
			<< "	BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS   BOOLEAN				,"
			<< "	EXECUTION_VENUE                                     BOOLEAN				,"
			<< "	EFFECTIVE_DATE                                      DATE				,"
			<< "	END_DATE                                            DATE				,"
			<< "	DAY_COUNT_CONVENTION                                ASCII				,"
			<< "	SETTLEMENT_CURRENCY                                 ASCII				,"
			<< "	ASSET_CLASS                                         TINYINT				,"
			<< "	SUBASSET_CLASS_FOR_OTHER_COMMODITY                  ASCII				,"
			<< "	TAXONOMY                                            ASCII				,"
			<< "	PRICE_FORMING_CONTINUATION_DATA                     TINYINT,"
			<< "	UNDERLYING_ASSET_1                                  ASCII,"
			<< "	UNDERLYING_ASSET_2                                  ASCII,"
			<< "	PRICE_NOTATION_TYPE                                 ASCII,"
			<< "	PRICE_NOTATION                                      DECIMAL,"
			<< "	ADDITIONAL_PRICE_NOTATION_TYPE                      ASCII,"
			<< "	ADDITIONAL_PRICE_NOTATION                           DECIMAL,"
			<< "	NOTIONAL_CURRENCY_1                                 ASCII,"
			<< "	NOTIONAL_CURRENCY_2                                 ASCII,"
			<< "	ROUNDED_NOTIONAL_AMOUNT_1                           DECIMAL,"
			<< "	ROUNDED_NOTIONAL_AMOUNT_1_PLUS                      BOOLEAN,"
			<< "	ROUNDED_NOTIONAL_AMOUNT_2                           DECIMAL,"
			<< "	ROUNDED_NOTIONAL_AMOUNT_2_PLUS                      BOOLEAN,"
			<< "	PAYMENT_FREQUENCY_1                                 ASCII,"
			<< "	PAYMENT_FREQUENCY_2                                 ASCII,"
			<< "	RESET_FREQUENCY_1                                   ASCII,"
			<< "	RESET_FREQUENCY_2                                   ASCII,"
			<< "	EMBEDED_OPTION                                      ASCII,"
			<< "	OPTION_STRIKE_PRICE                                 DECIMAL,"
			<< "	OPTION_TYPE                                         TINYINT,"
			<< "	OPTION_FAMILY                                       TINYINT,"
			<< "	OPTION_CURRENCY                                     ASCII,"
			<< "	OPTION_PREMIUM                                      DECIMAL,"
			<< "	OPTION_LOCK_PERIOD                                  DATE,"
			<< "	OPTION_EXPIRATION_DATE                              DATE,"
			<< "	PRICE_NOTATION2_TYPE                                ASCII,"
			<< "	PRICE_NOTATION2                                     DECIMAL,"
			<< "	PRICE_NOTATION3_TYPE                                ASCII,"
			<< "	PRICE_NOTATION3                                     DECIMAL,"
			<< "	FILE_DATE                                           DATE"
			<< ")"
			<< "WITH COMPRESSION = { 'sstable_compression': '' };";

		cnx_->execute(ss.str());
	}

	void cassandra::deleteTable()
	{
		std::string ks = "test";
		std::string tbl = "test";

		std::stringstream ss;

		ss  << "DROP TABLE IF EXISTS "
			<< ks << "." << tbl;;

		cnx_->execute(ss.str());
	}

	void cassandra::disconnect()
	{
		try
		{
			cnx_->close();
		}
		catch (const std::exception & ex)
		{
			BOOST_ERROR("\n Failed to disconnect the database: " << ex.what());
		}
	}

	boost::unit_test_framework::test_suite * cassandra::suite()
	{
		boost::unit_test_framework::test_suite * suite = BOOST_TEST_SUITE("compression Tests");
		suite->add(BOOST_TEST_CASE(&cassandra::connect));
		suite->add(BOOST_TEST_CASE(&cassandra::createTable));
		suite->add(BOOST_TEST_CASE(&cassandra::deleteTable));
		suite->add(BOOST_TEST_CASE(&cassandra::disconnect));
		return suite;
	}
}
#endif
