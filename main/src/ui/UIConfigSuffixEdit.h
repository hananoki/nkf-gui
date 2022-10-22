#pragma once

#include <QWidget>
#include "IConfigPage.h"

class UIConfigSuffixEdit : public QWidget {
	Q_OBJECT

public:
	explicit UIConfigSuffixEdit( QWidget* parent = nullptr );
	~UIConfigSuffixEdit();


private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

