#pragma once


//////////////////////////////////////////////////////////////////////////////////
class ActionOpenFolder : public QAction {
public:
	ActionOpenFolder( HTreeWidget* tw );
	static ActionOpenFolder* create( HTreeWidget* p ) { return new ActionOpenFolder( p ); }
};


//////////////////////////////////////////////////////////////////////////////////
class ActionFolderPathCopy : public QAction {
public:
	ActionFolderPathCopy( HTreeWidget* tw );
	static ActionFolderPathCopy* create( HTreeWidget* p ) { return new ActionFolderPathCopy( p ); }
};


//////////////////////////////////////////////////////////////////////////////////
class ActionFolderAddIgnore : public QAction {
public:
	ActionFolderAddIgnore( HTreeWidget* tw );
	static ActionFolderAddIgnore* create( HTreeWidget* p ) { return new ActionFolderAddIgnore( p ); }
};

