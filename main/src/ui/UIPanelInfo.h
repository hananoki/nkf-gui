#pragma once

#include <QWidget>

class UIPanelInfo : public QWidget
{
	Q_OBJECT
	
public:
	explicit UIPanelInfo(QWidget *parent = nullptr);
	~UIPanelInfo();
	
private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

