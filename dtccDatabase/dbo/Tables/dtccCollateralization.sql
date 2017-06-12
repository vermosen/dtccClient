CREATE TABLE [dbo].[dtccCollateralization] (
    [id]                    TINYINT      NOT NULL,
    [collateratization_key] CHAR (2)     NOT NULL,
    [description]           VARCHAR (50) NULL,
    CONSTRAINT [PK_indication_of_collateralization] PRIMARY KEY CLUSTERED ([id] ASC)
);

