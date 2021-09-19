class EditorListItemController: ViewController
{
	string Label = "IF_YOU_SEE_THIS_ITS_BROKEN";
	string Icon;
	
	string CollapseIcon = "set:dayz_editor_gui image:eye_open";
	// gormirns too dumb to implement this right now : Gormirn hide eye
	string ExpandIcon = "set:dayz_editor_gui image:eye_disabled";
	
	bool Favorite;

	ref ObservableCollection<ref EditorListItem> ChildListItems = new ObservableCollection<ref EditorListItem>(this);
}