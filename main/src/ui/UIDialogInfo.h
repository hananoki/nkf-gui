#pragma once

#include <QDialog>

//////////////////////////////////////////////////////////////////////////////////
class UIDialogInfo : public QDialog {
	Q_OBJECT

public:
	explicit UIDialogInfo( QWidget* parent = nullptr );
	~UIDialogInfo();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

