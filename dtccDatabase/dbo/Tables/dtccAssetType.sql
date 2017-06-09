CREATE TABLE [dbo].[dtccAssetType] (
    [id]          TINYINT      NOT NULL,
    [type]        CHAR (2)     NOT NULL,
    [description] VARCHAR (50) NULL,
    CONSTRAINT [PK_asset_class] PRIMARY KEY CLUSTERED ([id] ASC)
);

