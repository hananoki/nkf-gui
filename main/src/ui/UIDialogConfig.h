#pragma once

#include <QDialog>

//////////////////////////////////////////////////////////////////////////////////
class UIDialogConfig : public QDialog {
	Q_OBJECT

public:
	explicit UIDialogConfig( QWidget* parent = nullptr );
	~UIDialogConfig();

	//virtual void showEvent( QShowEvent* event );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

