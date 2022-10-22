#pragma once

#include <QWidget>

class Item_File;
class Item_Folder;
using FileFunc = std::function<void( Item_File* item )>;
using FolderFunc = std::function<void( Item_Folder* item )>;

class UIPanelMain : public QWidget {
	Q_OBJECT

public:
	explicit UIPanelMain( QWidget* parent = nullptr );
	~UIPanelMain();

	/// @brief  イベントフィルター
	bool eventFilter( QObject* obj, QEvent* event );
	
	static void traverseItems( FileFunc func );
	static void traverseItems( FolderFunc func );

signals:
	void addTreeItem( QTreeWidgetItem* item );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

