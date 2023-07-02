#include "UIDialogInfo.h"
#include "ui_UIDialogInfo.h"
#include "revinfo.inc"

//////////////////////////////////////////////////////////////////////////////////
class UIDialogInfo::Impl : public Ui_UIDialogInfo {
public:

	UIDialogInfo* self;


	/////////////////////////////////////////
	Impl( UIDialogInfo* _self ) :
		self( _self ) {

		setupUi( self );

		connect( label, &HClickableLabel::clicked, [&]() {
			QMessageBox msgbox;
			msgbox.aboutQt( self, tr( u8"Qtについて" ) );
		} );

		label_rev->setText( $$( "revision: %1" ).arg( revno ) );

		textBrowser->setMarkdown( fs::readAllText(":/res/markdown/about.md"));
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIDialogInfo::UIDialogInfo( QWidget* parent ) :
	QDialog( parent, Qt::Tool ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
UIDialogInfo::~UIDialogInfo() {

}
