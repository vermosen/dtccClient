CREATE TABLE [dbo].[dtccPriceFormingContinuationData] (
    [id]          TINYINT      IDENTITY (1, 1) NOT NULL,
    [type]        NCHAR (20)   NOT NULL,
    [description] VARCHAR (50) NULL,
    CONSTRAINT [PK_dtccPriceFormingContinuationData] PRIMARY KEY CLUSTERED ([id] ASC)
);

