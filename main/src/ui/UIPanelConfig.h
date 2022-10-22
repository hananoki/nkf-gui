#pragma once

#include <QWidget>


class UIPanelConfig : public QWidget {
	Q_OBJECT

public:
	explicit UIPanelConfig( QWidget* parent = nullptr );
	~UIPanelConfig();

	virtual void showEvent( QShowEvent* event );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

