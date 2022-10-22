#pragma once

#include <QWidget>

class UICommandBar : public QWidget {
	Q_OBJECT

public:
	explicit UICommandBar( QWidget* parent = nullptr );
	~UICommandBar();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

