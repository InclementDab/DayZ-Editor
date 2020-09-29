class DialogBaseController: Controller
{
	string Title;
	string Caption;
	
	ref ObservableCollection<ref ScriptView> DialogContentData = new ObservableCollection<ref ScriptView>("DialogContentData", this);
	ref ObservableCollection<ref ScriptView> DialogButtonData = new ObservableCollection<ref ScriptView>("DialogButtonData", this);
	
	void ~DialogBaseController()
	{
		delete DialogContentData;
		delete DialogButtonData;
	}
}