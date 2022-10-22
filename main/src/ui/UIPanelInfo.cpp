#include "UIPanelInfo.h"
#include "ui_UIPanelInfo.h"
#include "revinfo.inc"

//////////////////////////////////////////////////////////////////////////////////
class UIPanelInfo::Impl : public Ui_UIPanelInfo {
public:

	UIPanelInfo* self;

	QMap<EConfigItem, QWidget*> map;

	/////////////////////////////////////////
	Impl( UIPanelInfo* _self ) :
		self( _self ) {

		setupUi( self );

		//textBrowser->setMark

		connect( label, &HClickableLabel::clicked, [&]() {
			QMessageBox msgbox;
			msgbox.aboutQt( self, tr( u8"Qt‚É‚Â‚¢‚Ä" ) );
		} );

		label_rev->setText( $$( "revision: %1" ).arg( revno ) );
	}
};


//////////////////////////////////////////////////////////////////////////////////
UIPanelInfo::UIPanelInfo( QWidget* parent ) :
	QWidget( parent ),
	impl( new Impl( this ) ) {

}


/////////////////////////////////////////
UIPanelInfo::~UIPanelInfo() {

}
