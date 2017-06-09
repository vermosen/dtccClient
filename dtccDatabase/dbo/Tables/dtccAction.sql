CREATE TABLE [dbo].[dtccAction] (
    [id]          TINYINT      IDENTITY (1, 1) NOT NULL,
    [type]        NCHAR (10)   NOT NULL,
    [description] VARCHAR (50) NULL,
    CONSTRAINT [PK_dtccAction] PRIMARY KEY CLUSTERED ([id] ASC)
);

