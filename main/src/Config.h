#pragma once

#include <QMap>
#include "IConfigChanged.h"
#include "NkfHelper.h"

#define CONFIG_VALUE(type,name) type name;
#define CONFIG_VALUE_INIT(type,name,ini) type name = ini;

#define CONFIG_VALUE_LIST \
	CONFIG_VALUE( QPoint, pos ) \
	CONFIG_VALUE( QSize, size ) \
	CONFIG_VALUE( bool, ignoreFlag ) \
	CONFIG_VALUE( bool, ignoreAsciiFlag ) \
	CONFIG_VALUE( bool, BOM ) \
	CONFIG_VALUE( int, lastConfigRow ) \
	CONFIG_VALUE( QStringList, ignorePaths ) \
	CONFIG_VALUE( QString, characterCode ) \
	CONFIG_VALUE( QString, lineFeedCode ) \
	CONFIG_VALUE( QString, fileTreeHeader ) \
	CONFIG_VALUE( QStringList, suffixes ) \
	CONFIG_VALUE( QStringList, lastInput )


class Config : public QObject, public IConfigChanged {
	Q_OBJECT
public:
	CONFIG_VALUE_LIST

	static void save();
	static void load();

	virtual void changedValue() override;

	QString suffixFilter() const;

	bool hasSuffix( QString filePath ) const;

	/**
	 * @brief  文字コードを取得する
	 * @return ECharacterCode
	 */
	nkf::ECharacterCode getCharacterCode() const;


	nkf::ELineFeedCode getLineFeedCode() const;

	bool containsIgnorePath( QString folderPath ) const;

	void addIgnorePath( QString folderPath );

signals:
	void viewStateChange();
};

extern Config config;
