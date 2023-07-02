#pragma once

//////////////////////////////////////////////////////////////////////////////////
class ItemFolder : public HTreeWidgetItem {
public:
	QString fullPath;

	ItemFolder( const QString& _fullPath );

	

	/////////////////////////////////////////
	void openFolder();

	/////////////////////////////////////////
	void updateVisible();
};

