#pragma once

#include "QMainWindow.h"

#define mainWindow (UIMainWindow::instance)

//////////////////////////////////////////////////////////////////////////////////
class UIMainWindow : public QMainWindow {
	Q_OBJECT

public:
	static UIMainWindow* instance;

	UIMainWindow( QWidget* parent = nullptr );
	~UIMainWindow();

	/////////////////////////////////////////
	void start();

	/////////////////////////////////////////
	virtual void closeEvent( QCloseEvent* event ) override;

	/// @brief  イベントフィルター
	bool eventFilter( QObject* obj, QEvent* event );

	template<class T>
	void showDialog() {
		$::showDialog<T>( this );
	}

signals:
	void signal_start();
	void signal_closeWindow();

	void addTreeItem( QTreeWidgetItem* item );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};
