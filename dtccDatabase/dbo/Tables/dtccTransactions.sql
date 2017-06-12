﻿CREATE TABLE [dbo].[dtccTransactions] (
    [DISSEMINATION_ID]                                  BIGINT           NOT NULL,
    [ORIGINAL_DISSEMINATION_ID]                         BIGINT           NULL,
    [ACTION]                                            TINYINT          NOT NULL,
    [EXECUTION_TIMESTAMP]                               DATETIME2 (0)    NULL,
    [CLEARED]                                           BIT              NOT NULL,
    [INDICATION_OF_COLLATERALIZATION]                   TINYINT          NULL,
    [INDICATION_OF_END_USER_EXCEPTION]                  BIT              NULL,
    [INDICATION_OF_OTHER_PRICE_AFFECTING_TERM]          BIT              NOT NULL,
    [BLOCK_TRADES_AND_LARGE_NOTIONAL_OFFFACILITY_SWAPS] BIT              NULL,
    [EXECUTION_VENUE]                                   BIT              NULL,
    [EFFECTIVE_DATE]                                    DATE             NULL,
    [END_DATE]                                          DATE             NULL,
    [DAY_COUNT_CONVENTION]                              VARCHAR (50)     NULL,
    [SETTLEMENT_CURRENCY]                               CHAR (10)        NULL,
    [ASSET_CLASS]                                       TINYINT          NOT NULL,
    [SUBASSET_CLASS_FOR_OTHER_COMMODITY]                VARCHAR (50)     NULL,
    [TAXONOMY]                                          VARCHAR (250)    NOT NULL,
    [PRICE_FORMING_CONTINUATION_DATA]                   TINYINT          NOT NULL,
    [UNDERLYING_ASSET_1]                                NVARCHAR (100)   NULL,
    [UNDERLYING_ASSET_2]                                NVARCHAR (100)   NULL,
    [PRICE_NOTATION_TYPE]                               VARCHAR (50)     NULL,
    [PRICE_NOTATION]                                    NUMERIC (38, 10) NULL,
    [ADDITIONAL_PRICE_NOTATION_TYPE]                    VARCHAR (50)     NULL,
    [ADDITIONAL_PRICE_NOTATION]                         NUMERIC (38, 10) NULL,
    [NOTIONAL_CURRENCY_1]                               VARCHAR (50)     NOT NULL,
    [NOTIONAL_CURRENCY_2]                               VARCHAR (50)     NULL,
    [ROUNDED_NOTIONAL_AMOUNT_1]                         NUMERIC (38, 10) NULL,
    [ROUNDED_NOTIONAL_AMOUNT_1_PLUS]                    BIT              NULL,
    [ROUNDED_NOTIONAL_AMOUNT_2]                         NUMERIC (38, 10) NULL,
    [ROUNDED_NOTIONAL_AMOUNT_2_PLUS]                    BIT              NULL,
    [PAYMENT_FREQUENCY_1]                               VARCHAR (50)     NULL,
    [PAYMENT_FREQUENCY_2]                               VARCHAR (50)     NULL,
    [RESET_FREQUENCY_1]                                 VARCHAR (50)     NULL,
    [RESET_FREQUENCY_2]                                 VARCHAR (50)     NULL,
    [EMBEDED_OPTION]                                    VARCHAR (50)     NULL,
    [OPTION_STRIKE_PRICE]                               NUMERIC (38, 10) NULL,
    [OPTION_TYPE]                                       TINYINT          NULL,
    [OPTION_FAMILY]                                     TINYINT          NULL,
    [OPTION_CURRENCY]                                   CHAR (3)         NULL,
    [OPTION_PREMIUM]                                    NUMERIC (38, 10) NULL,
    [OPTION_LOCK_PERIOD]                                DATE             NULL,
    [OPTION_EXPIRATION_DATE]                            DATE             NULL,
    [PRICE_NOTATION2_TYPE]                              VARCHAR (50)     NULL,
    [PRICE_NOTATION2]                                   NUMERIC (38, 10) NULL,
    [PRICE_NOTATION3_TYPE]                              VARCHAR (50)     NULL,
    [PRICE_NOTATION3]                                   NUMERIC (38, 10) NULL,
    [FILE_DATE]                                         DATE             NOT NULL,
    CONSTRAINT [PK_dtcc_transactions] PRIMARY KEY CLUSTERED ([DISSEMINATION_ID] ASC),
    CONSTRAINT [FK_dtcc_transactions_asset_class] FOREIGN KEY ([ASSET_CLASS]) REFERENCES [dbo].[dtccAssetType] ([id]),
    CONSTRAINT [FK_dtcc_transactions_indication_of_collateralization] FOREIGN KEY ([INDICATION_OF_COLLATERALIZATION]) REFERENCES [dbo].[dtccCollateralization] ([id]),
    CONSTRAINT [FK_dtccTransactions_dtccAction] FOREIGN KEY ([ACTION]) REFERENCES [dbo].[dtccAction] ([id]),
    CONSTRAINT [FK_dtccTransactions_dtccOptionFamily] FOREIGN KEY ([OPTION_FAMILY]) REFERENCES [dbo].[dtccOptionFamily] ([id]),
    CONSTRAINT [FK_dtccTransactions_dtccOptionType] FOREIGN KEY ([OPTION_TYPE]) REFERENCES [dbo].[dtccOptionType] ([id]),
    CONSTRAINT [FK_dtccTransactions_dtccPriceFormingContinuationData] FOREIGN KEY ([PRICE_FORMING_CONTINUATION_DATA]) REFERENCES [dbo].[dtccPriceFormingContinuationData] ([id])
);

