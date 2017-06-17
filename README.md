# dtccClient
a service/console bundle written in C++ to download and store intraday trading data disseminated by the Depository Trust & Clearing Corporation (DTCC).

The solution include two applications. The console can be used to retrieve the historical data from the dtcc. The service is used for intraday polling.

The solution use Boost, ZLIB. Boost::Spirit is used for message parsing. Boost::Asio is used for communications. 

The console project can be easily ported to any plateform providing a C++11 compatible compiler (I've successfully compiled the project with MSVC 15 and intel compiler v17). I'll try to convert the Windows service in a daemon anytime soon.

Edit 06/12/2017. I added an experimental branch to play around with the datastax connectors.

