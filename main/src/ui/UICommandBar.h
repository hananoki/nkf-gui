#pragma once

#include <QWidget>

//////////////////////////////////////////////////////////////////////////////////
class UICommandBar : public QWidget {
	Q_OBJECT

public:
	explicit UICommandBar( QWidget* parent = nullptr );
	~UICommandBar();

signals:
	// 全て選択、全て解除
	void selectFileFlag(bool);

	// 表示用フィルター
	void viewStateChange();

	// 変換ボタン
	void buttonConvert();

private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

