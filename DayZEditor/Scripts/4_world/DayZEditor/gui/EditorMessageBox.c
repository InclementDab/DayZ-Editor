

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

class EditorDialogController: Controller
{
	string Title;
	string Caption;
	
	ref ObservableCollection<ref ScriptView> DialogContentData = new ObservableCollection<ref ScriptView>("DialogContentData", this);
	ref ObservableCollection<ref ScriptView> DialogButtonData = new ObservableCollection<ref ScriptView>("DialogButtonData", this);
	
	void ~EditorDialogController()
	{
		delete DialogContentData;
		delete DialogButtonData;
	}
}

class EditorDialogBase: EditorScriptView
{
	protected EditorDialogController m_EditorDialogController;
	
	void EditorDialogBase(Widget parent = null)
	{
		EditorLog.Trace("EditorDialog");

		m_EditorDialogController = EditorDialogController.Cast(m_Controller);

		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
		
		EditorUIManager.CurrentDialog = this;
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	private void ~EditorDialogBase()
	{
		EditorLog.Trace("~EditorDialog");
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}
	
	void CloseDialog()
	{
		EditorLog.Trace("EditorDialog::CloseDialog");
		delete this;
	}
	
	void AddContent(ScriptView content)
	{
		m_EditorDialogController.DialogContentData.Insert(content);
	}
	
	void AddButton(ScriptView button)
	{
		m_EditorDialogController.DialogButtonData.Insert(button);
	}
		
	override typename GetControllerType() {
		return EditorDialogController;
	}	
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialog.layout";
	}
}

// todo: move this to 3_Game and add it to the EditorLog.Error so dialogs show :)
class EditorMessageBox: EditorDialogBase
{
	protected EditorMessageBoxResult m_CurrentResult = EditorMessageBoxResult.None;
	protected GridSpacerWidget ButtonGrid;
	
	
	private void EditorMessageBox(Widget parent = null, string title = "", string caption = "", EditorMessageBoxButtons buttons = EditorMessageBoxButtons.OK)
	{
		m_EditorDialogController.Title = title;
		m_EditorDialogController.NotifyPropertyChanged("Title");
		m_EditorDialogController.Caption = caption;
		m_EditorDialogController.NotifyPropertyChanged("Caption");
		
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
		
		AddButton(dialog_button);
	}

	static EditorMessageBoxResult Show(string title, string caption, EditorMessageBoxButtons buttons = EditorMessageBoxButtons.OK)
	{
		EditorMessageBox message_box = new EditorMessageBox(null, title, caption, buttons);
		
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
			
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorMessageBox.layout";
	}
}