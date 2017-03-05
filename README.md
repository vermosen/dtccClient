# dtccClient
a service/console bundle written in C++ to download and store intraday trading data disseminated by the Depository Trust & Clearing Corporation (DTCC).

The console program can be used to retrieve the historical data from the dtcc. The service is used for intraday polling.

The solution use Boost, cUrl, ZLIB, SOCI. The console project can be easily ported to any plateform providing a C++11 compatible compiler (I've successfully compiled the project with MSVC 15 and intel compiler). 

The parsing engine is based on Boost::spirit.

I aim at adapting the boost::asio engine from the aleitha project, so a multi-threaded engine can be used for the intraday retrieval service.
