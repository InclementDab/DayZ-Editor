
class EditorListItemController: Controller
{
	string Label = "IF_YOU_SEE_THIS_ITS_BROKEN";
	string Icon;
	
	string CollapseIcon = "set:dayz_editor_gui image:eye_open";
	string ExpandIcon = "set:dayz_editor_gui image:eye_disabled";

	ref ObservableCollection<ref EditorListItem> ChildListItems = new ObservableCollection<ref EditorListItem>(this);
}