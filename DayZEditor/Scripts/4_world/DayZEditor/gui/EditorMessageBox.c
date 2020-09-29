

enum EditorMessageBoxButtons
{
	OK,
	OKCancel,
	AbortRetryIgnore,
	YesNoCancel,
	YesNo,
	RetryCancel
};

enum EditorMessageBoxResult
{
	None,
	OK,
	Cancel,
	Abort,
	Retry,
	Ignore,
	Yes,
	No
};

class EditorMessageBoxButton: EditorScriptView
{	
	protected ButtonWidget DialogButton;
	
	EditorMessageBoxResult ButtonResult;
	
	protected string m_ExecuteAction;
	
	void EditorMessageBoxButton(Widget parent = null, EditorMessageBoxResult result = EditorMessageBoxResult.OK)
	{
		ButtonResult = result;
	}
	
	bool DialogButtonExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("DialogButtonExecute");
		
		g_Script.CallFunction(GetParent(), m_ExecuteAction, null, this);
		
		return true;
	}
		
	void SetExecuteFunctionString(string function)
	{
		m_ExecuteAction = function;
	}
	
	void SetLabel(string label)
	{
		DialogButton.SetText(label);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout";
	}
}

class EditorMessageBoxController: Controller
{
	string Title_Text;
	string Caption_Text;
	
	ref ObservableCollection<ref EditorMessageBoxButton> ButtonList = new ObservableCollection<ref EditorMessageBoxButton>("ButtonList", this);
	
	void ~EditorMessageBoxController()
	{
		delete ButtonList;
	}
}

class EditorMessageBox: EditorScriptView
{
	protected EditorMessageBoxResult m_CurrentResult = EditorMessageBoxResult.None;
	
	protected GridSpacerWidget ButtonGrid;
	
	private void EditorMessageBox(Widget parent = null, string title = "", string caption = "", EditorMessageBoxButtons buttons = EditorMessageBoxButtons.OK)
	{
		switch (buttons) {
			
			case EditorMessageBoxButtons.OK: {
				AddButton(EditorMessageBoxResult.OK);
				break;
			}
			
			case EditorMessageBoxButtons.OKCancel: {
				AddButton(EditorMessageBoxResult.OK);
				AddButton(EditorMessageBoxResult.Cancel);
				break;
			}			
			
			case EditorMessageBoxButtons.AbortRetryIgnore: {
				AddButton(EditorMessageBoxResult.Abort);
				AddButton(EditorMessageBoxResult.Retry);
				AddButton(EditorMessageBoxResult.Ignore);
				break;
			}			
			
			case EditorMessageBoxButtons.YesNoCancel: {
				AddButton(EditorMessageBoxResult.Yes);
				AddButton(EditorMessageBoxResult.No);
				AddButton(EditorMessageBoxResult.Cancel);
				break;
			}			
			
			case EditorMessageBoxButtons.YesNo: {
				AddButton(EditorMessageBoxResult.Yes);
				AddButton(EditorMessageBoxResult.No);
				break;
			}			
			
			case EditorMessageBoxButtons.RetryCancel: {
				AddButton(EditorMessageBoxResult.Retry);
				AddButton(EditorMessageBoxResult.Cancel);
				break;
			}
		}
		
		EditorMessageBoxController controller = EditorMessageBoxController.Cast(m_Controller);
		controller.Title_Text = title;
		controller.NotifyPropertyChanged("Title_Text");
		controller.Caption_Text = caption;
		controller.NotifyPropertyChanged("Caption_Text");
	}
	
	private void ~EditorMessageBox()
	{
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}
	
	private EditorMessageBoxResult GetResult()
	{
		return m_CurrentResult;
	}
	
	private void OnDialogResult(EditorMessageBoxButton button)
	{
		m_CurrentResult = button.ButtonResult;
	}
	
	private void AddButton(EditorMessageBoxResult result)
	{
		EditorMessageBoxButton dialog_button = new EditorMessageBoxButton(ButtonGrid, result);
		dialog_button.SetParent(this);
		dialog_button.SetLabel(typename.EnumToString(EditorMessageBoxResult, result));
		dialog_button.SetExecuteFunctionString("OnDialogResult");
		
		EditorMessageBoxController controller = EditorMessageBoxController.Cast(m_Controller);
		controller.ButtonList.Insert(dialog_button);
	}

	
	static EditorMessageBoxResult Show(string title, string caption, EditorMessageBoxButtons buttons = EditorMessageBoxButtons.OK)
	{
		EditorMessageBox message_box = new EditorMessageBox(null, title, caption, buttons);
		message_box.ShowDialog();
		
		EditorMessageBoxResult result = EditorMessageBoxResult.None;
		while (message_box && message_box.GetResult() == EditorMessageBoxResult.None) {
			Sleep(1);
		}
		
		if (!message_box) {
			return result;
		}
		
		result = message_box.GetResult();
		message_box.CloseDialog();
		return result;
	}
	
	DialogResult ShowDialog()
	{
		EditorLog.Trace("EditorDialog::Show");
		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
		
		EditorUIManager.CurrentDialog = this;
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
		m_LayoutRoot.Show(true);
		
		// todo
		return DialogResult.OK;
	}
	
	void CloseDialog()
	{
		EditorLog.Trace("EditorDialog::Close");
		delete this;
	}
	
	override typename GetControllerType() {
		return EditorMessageBoxController;
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorMessageBox.layout";
	}
	
}