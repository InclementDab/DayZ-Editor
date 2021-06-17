class EditorMenuController: Controller
{
	ref ObservableCollection<ref EditorMenuItem> MenuItems = new ObservableCollection<ref EditorMenuItem>(this);
	
	void ~EditorMenuController()
	{
		delete MenuItems;
	}
}