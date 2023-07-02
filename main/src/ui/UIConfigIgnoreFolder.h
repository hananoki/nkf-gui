#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UIConfigIgnoreFolder : public QWidget {
	Q_OBJECT

public:
	explicit UIConfigIgnoreFolder( QWidget* parent = nullptr );
	~UIConfigIgnoreFolder();

	/// @brief  イベントフィルター
	bool eventFilter( QObject* obj, QEvent* event );

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

