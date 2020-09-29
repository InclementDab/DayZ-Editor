

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
	
	void EditorMessageBoxButton(Widget parent = null, string label = "", string callback = "", EditorMessageBoxResult result = EditorMessageBoxResult.OK)
	{
		ButtonResult = result;
		m_ExecuteAction = callback;
		DialogButton.SetText(label);
	}
	
	bool DialogButtonExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("DialogButtonExecute");
		g_Script.CallFunction(GetParent(), m_ExecuteAction, null, this);
		return true;
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout";
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
		EditorMessageBoxButton dialog_button = new EditorMessageBoxButton(ButtonGrid, typename.EnumToString(EditorMessageBoxResult, result), "OnDialogResult", result);
		dialog_button.SetParent(this);
		
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