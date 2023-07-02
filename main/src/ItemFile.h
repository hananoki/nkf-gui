﻿#pragma once

//////////////////////////////////////////////////////////////////////////////////
class ItemFile : public QTreeWidgetItem {
public:
	QString fullPath;
	nkf::ECharacterCode code;

	/////////////////////////////////////////
	ItemFile( const QString& _fullPath );

	/////////////////////////////////////////
	void updateItemState( bool read = false );

	/////////////////////////////////////////
	QString fileName();

	/////////////////////////////////////////
	QString folderPath();

	/////////////////////////////////////////
	bool showInExplorer();

	/////////////////////////////////////////
	bool openFile();
};