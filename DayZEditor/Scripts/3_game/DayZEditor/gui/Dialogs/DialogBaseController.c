class DialogBaseController: Controller
{
	string Title;
		
	ref ObservableCollection<ref ScriptView> DialogContentData = new ObservableCollection<ref ScriptView>("DialogContentData", this);
	ref ObservableCollection<ref DialogButton> DialogButtonData = new ObservableCollection<ref DialogButton>("DialogButtonData", this);
		
	void ~DialogBaseController()
	{
		delete DialogContentData;
		delete DialogButtonData;
	}
}