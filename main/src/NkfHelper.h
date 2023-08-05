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

	enum class ELineFeedCode : int {
		LF,
		CRLF,
		CR,
	};


	/**
	 * @brief  変換を行います
	 * @param  filePath  処理するファイル
	 * @param  cc        文字コード
	 * @param  lfc       改行コード
	 * @param  enableBOM BOMの有効化フラグ、Unicodeで有効
	 */
	bool convert( const QString& filePath, nkf::ECharacterCode cc, nkf::ELineFeedCode lfc, bool enableBOM  );

	ECharacterCode getCharacterCode( const QString& filePath );

	const QString& toString( ECharacterCode  code );
}

