from cassandra.cluster import Cluster
import logging

log = logging.getLogger()
log.setLevel('DEBUG')
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter("%(asctime)s [%(levelname)s] %(name)s: %(message)s"))
log.addHandler(handler)

try:

    keyspace    = "dtcc"
    table       = "transactions"
    cluster     = Cluster(['127.0.0.1']) 
    session     = cluster.connect()

    session.execute("""
            CREATE KEYSPACE IF NOT EXISTS %s
            WITH REPLICATION = { 'class' : 'SimpleStrategy', 'replication_factor' : 1 }
            AND DURABLE_WRITES = true;
        """ % keyspace)

    log.info("created keyspace")

    session.set_keyspace(keyspace)

    session.execute(
    """
            CREATE TABLE IF NOT EXISTS %s.%s 
            (
                DISSEMINATION_ID                                    BIGINT PRIMARY KEY  ,
                ORIGINAL_DISSEMINATION_ID                           BIGINT              ,
                ACTION                                              TINYINT             ,
                EXECUTION_TIMESTAMP                                 TIMESTAMP           ,
                CLEARED                                             BOOLEAN             ,
                INDICATION_OF_COLLATERALIZATION                     TINYINT             ,
                INDICATION_OF_END_USER_EXCEPTION                    BOOLEAN             ,
                INDICATION_OF_OTHER_PRICE_AFFECTING_TERM            BOOLEAN             ,
                BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS   BOOLEAN             ,
                EXECUTION_VENUE                                     BOOLEAN             ,
                EFFECTIVE_DATE                                      DATE                ,
                END_DATE                                            DATE                ,
                DAY_COUNT_CONVENTION                                ASCII               ,
                SETTLEMENT_CURRENCY                                 ASCII               ,
                ASSET_CLASS                                         TINYINT             ,
                SUBASSET_CLASS_FOR_OTHER_COMMODITY                  ASCII               ,
                TAXONOMY                                            ASCII               ,
                PRICE_FORMING_CONTINUATION_DATA                     TINYINT             ,
                UNDERLYING_ASSET_1                                  ASCII               ,
                UNDERLYING_ASSET_2                                  ASCII               ,
                PRICE_NOTATION_TYPE                                 ASCII               ,
                PRICE_NOTATION                                      DECIMAL             ,
                ADDITIONAL_PRICE_NOTATION_TYPE                      ASCII               ,
                ADDITIONAL_PRICE_NOTATION                           DECIMAL             ,
                NOTIONAL_CURRENCY_1                                 ASCII               ,
                NOTIONAL_CURRENCY_2                                 ASCII               ,
                ROUNDED_NOTIONAL_AMOUNT_1                           DECIMAL             ,
                ROUNDED_NOTIONAL_AMOUNT_1_PLUS                      BOOLEAN             ,
                ROUNDED_NOTIONAL_AMOUNT_2                           DECIMAL             ,
                ROUNDED_NOTIONAL_AMOUNT_2_PLUS                      BOOLEAN             ,
                PAYMENT_FREQUENCY_1                                 ASCII               ,
                PAYMENT_FREQUENCY_2                                 ASCII               ,
                RESET_FREQUENCY_1                                   ASCII               ,
                RESET_FREQUENCY_2                                   ASCII               ,
                EMBEDED_OPTION                                      ASCII               ,
                OPTION_STRIKE_PRICE                                 DECIMAL             ,
                OPTION_TYPE                                         TINYINT             ,
                OPTION_FAMILY                                       TINYINT             ,
                OPTION_CURRENCY                                     ASCII               ,
                OPTION_PREMIUM                                      DECIMAL             ,
                OPTION_LOCK_PERIOD                                  DATE                ,
                OPTION_EXPIRATION_DATE                              DATE                ,
                PRICE_NOTATION2_TYPE                                ASCII               ,
                PRICE_NOTATION2                                     DECIMAL             ,
                PRICE_NOTATION3_TYPE                                ASCII               ,
                PRICE_NOTATION3                                     DECIMAL             ,
                FILE_DATE                                           DATE
            ) 
            WITH COMPRESSION={'sstable_compression': ''};
    """ % (keyspace, table))
    
    cluster.shutdown()

except Exception as ex:
    log.error("An exception of type {0} occurred. Arguments:\n{1!r}".format(type(ex).__name__, ex.args))
    pass
