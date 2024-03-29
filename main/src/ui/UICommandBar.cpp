﻿#include "UICommandBar.h"
#include "ui_UICommandBar.h"

#include "UIMainWindow.h"
#include <magic_enum.hpp>



//////////////////////////////////////////////////////////////////////////////////
class UICommandBar::Impl : public Ui_UICommandBar {
public:

	UICommandBar* self;

	/////////////////////////////////////////
	Impl( UICommandBar* _self ) :
		self( _self ) {

		setupUi( self );

		$ComboBox::addItems( comboBox, $enums::names<nkf::ECharacterCode>() );
		$ComboBox::addItems( comboBox_2, $enums::names<nkf::ELineFeedCode>() );

		comboBox->bind( &config.characterCode );
		comboBox_2->bind( &config.lineFeedCode );

		checkBox->bind( &config.ignoreFlag );
		checkBox_2->bind( &config.ignoreAsciiFlag );
		checkBox_BOM->bind( &config.BOM );
	}


	/////////////////////////////////////////
	void setup() {

		// すべて選択
		$PushButton::click( button_all, [&]() {
			emit self->selectFileFlag( true );
		} );

		// すべて解除
		$PushButton::click( button_uncheck, [&]() {
			emit self->selectFileFlag( false );
		} );

		// 変換
		$PushButton::click( button_convert, [&]() {
			emit self->buttonConvert();
		} );

#pragma region 変更があったら描画更新
		connect( checkBox, &HCheckBox::stateChanged, [&]() {
			emit self->viewStateChange();
		} );

		connect( checkBox_2, &HCheckBox::stateChanged, [&]() {
			emit self->viewStateChange();
		} );

		connect( checkBox_BOM, &HCheckBox::stateChanged, [&]() {
			emit self->viewStateChange();
		} );

		// 文字コード選択ボックス
		connect( comboBox, &HComboBox::currentTextChanged, [&]() {
			emit self->viewStateChange();
		} );
#pragma endregion

	}
};



//////////////////////////////////////////////////////////////////////////////////
UICommandBar::UICommandBar( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

	connect(
		mainWindow,
		&UIMainWindow::signal_start,
		std::bind( &Impl::setup, impl.get() ) );
}


/////////////////////////////////////////
UICommandBar::~UICommandBar() {
}
