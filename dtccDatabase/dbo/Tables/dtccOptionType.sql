CREATE TABLE [dbo].[dtccOptionType] (
    [id]          TINYINT      IDENTITY (1, 1) NOT NULL,
    [type]        CHAR (2)     NOT NULL,
    [description] VARCHAR (50) NULL,
    CONSTRAINT [PK_dtccOptionType] PRIMARY KEY CLUSTERED ([id] ASC)
);

