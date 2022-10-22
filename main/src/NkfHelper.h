#pragma once

namespace nkf {
	enum class ECharacterCode : int {
		SJIS = 0,
		EUC = 1,
		JIS = 2,
		UTF8 = 3,
		UTF16LE = 4,
		UTF16BE = 5,
		ASCII = 10,
	};
	bool convertUTF8( const QString& filePath );
	ECharacterCode getCharacterCode( const QString& filePath );

	const QString& toString( ECharacterCode  code );
}

