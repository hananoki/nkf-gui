#pragma once

#include <QMap>
#include "Enums.h"
#include "NkfHelper.h"

using TMap = QMap<QString, QString>;

#define CONFIG_VALUE(type,name) type name;
#define CONFIG_VALUE_INIT(type,name,ini) type name = ini;

#define CONFIG_VALUE_LIST \
	CONFIG_VALUE( QPoint, pos ) \
	CONFIG_VALUE( QSize, size ) \
	CONFIG_VALUE( bool, ignoreFile ) \
	CONFIG_VALUE( EConfigItem, lastEConfigItem ) \
	CONFIG_VALUE( QStringList, ignorePaths ) \
	CONFIG_VALUE( QString, characterCode ) \
	CONFIG_VALUE( QString, fileTreeHeader ) \
	CONFIG_VALUE( QStringList, suffixes ) \
	CONFIG_VALUE( QStringList, lastInput )

class Config :public QObject {
	Q_OBJECT
public:
	CONFIG_VALUE_LIST

	static void save();
	static void load();

	QString suffixFilter();
	bool hasSuffix( QString filePath );

	nkf::ECharacterCode getCharacterCode();

	bool containsIgnorePath( QString folderPath );

	void addIgnorePath( QString folderPath );

signals:
	void signal_changeIgnorePath();
};

extern Config config;
