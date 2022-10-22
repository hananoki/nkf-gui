#pragma once

#include <QWidget>

class HFramelessWindow;

//////////////////////////////////////////////////////////////////////////////////
class UIMainCaption : public QWidget {
	Q_OBJECT

public:
	explicit UIMainCaption( QWidget* parent = nullptr );
	~UIMainCaption();

	void attach( HFramelessWindow* dialog );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};
