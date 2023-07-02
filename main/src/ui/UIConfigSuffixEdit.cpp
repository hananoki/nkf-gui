#include "UIConfigSuffixEdit.h"
#include "ui_UIConfigSuffixEdit.h"

//////////////////////////////////////////////////////////////////////////////////
class UIConfigSuffixEdit::Impl : public Ui_UIConfigSuffixEdit {
public:

	UIConfigSuffixEdit* self;

	class Item : public QListWidgetItem {
	public:
		Item( QString t ) : QListWidgetItem() {
			setText( t );
			setCheckState( Qt::Checked );
		}
	};

	/////////////////////////////////////////
	Impl( UIConfigSuffixEdit* _self ) :
		self( _self ) {

		//UIFileList::instance = self;

		//char buffer[1024];
		//DWORD ret;
		//GetNkfVersionSafeA( buffer, 1024 /*in TCHARs*/, &ret /*in TCHARs*/ );
		//label->setText( $$("nkf - %1").arg( buffer ) );

		setupUi( self );

		label_message->hide();

		QRegularExpression rx( R"(^\w*)" );
		lineEdit->setValidator( new QRegularExpressionValidator( rx, self ) );

		for( auto& f : config.suffixes ) {
			listWidget->addItem( new Item( f ) );
		}


		$PushButton::clicked( button_p, [&]( bool ) {
			auto t = lineEdit->text();
			if( t.isEmpty() ) {
				UIStatusBar::warning( tr( u8"入力文字が空です" ) );
				return;
			}
			config.suffixes.append( t );
			listWidget->addItem( new Item( t ) );
		} );

		$PushButton::clicked( button_m, [&]( bool ) {
			int index = listWidget->currentRow();
			if( index < 0 ) {
				UIStatusBar::warning( tr( u8"アイテムを選択してください" ) );
				return;
			}
			auto* item = listWidget->takeItem( index );
			config.suffixes.removeOne( item->text() );
		} );

		$LineEdit::textChanged( lineEdit, [&]( const QString& text ) {
			bool b = 0 <= config.suffixes.indexOf( text );
			label_message->setVisible( b );
			button_p->setEnabled( !b );
		} );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIConfigSuffixEdit::UIConfigSuffixEdit( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {
}


/////////////////////////////////////////
UIConfigSuffixEdit::~UIConfigSuffixEdit() {
}

