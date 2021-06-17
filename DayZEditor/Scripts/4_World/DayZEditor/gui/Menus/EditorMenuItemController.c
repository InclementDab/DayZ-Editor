class EditorMenuItemController: ViewController
{
	string LabelText;
	string IconPath;
	string ShortcutText;
	
	ref EditorMenu ChildMenu;
	
	void ~EditorMenuItemController()
	{
		delete ChildMenu;
	}
}