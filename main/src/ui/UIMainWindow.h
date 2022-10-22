#pragma once

#include "HFramelessWindow.h"


#define qtWindow (UIMainWindow::instance)

class UIMainWindow : public HFramelessWindow {
	Q_OBJECT

public:
	static UIMainWindow* instance;

	UIMainWindow( QWidget* parent = nullptr );
	~UIMainWindow();

	/////////////////////////////////////////
	void start();

	/////////////////////////////////////////
	virtual void closeEvent( QCloseEvent* event ) override;

	/////////////////////////////////////////
	static void changePanel( EPanelMode panelMode );

signals:
	void signal_start();
	void signal_closeWindow();
	void signal_changePanel( EPanelMode panelMode );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};
