

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

class EditorMessageBoxController: Controller
{
	string Title_Text;
	string Caption_Text;
	
	ref ObservableCollection<ref EditorDialogButton> ButtonList = new ObservableCollection<ref EditorDialogButton>("ButtonList", this);
	
	void ~EditorMessageBoxController()
	{
		delete ButtonList;
	}
}

class EditorMessageBox: EditorScriptView
{
	protected EditorMessageBoxResult m_CurrentResult = EditorMessageBoxResult.None;
	
	private void EditorMessageBox(Widget parent = null, string title = "", string caption = "", EditorMessageBoxButtons buttons = EditorMessageBoxButtons.OK)
	{
		switch (buttons) {
			
			case EditorMessageBoxButtons.OK: {
				AddButton(EditorMessageBoxResult.OK);
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
		
	private void OK()
	{
		EditorLog.Trace("EditorMessageBox::OK");
		m_CurrentResult = EditorMessageBoxResult.OK;
	}
	
	private void AddButton(EditorMessageBoxResult result)
	{
		EditorDialogButton dialog_button = new EditorDialogButton();
		dialog_button.SetParent(this);
		dialog_button.SetLabel(typename.EnumToString(EditorMessageBoxResult, result));
		dialog_button.SetExecuteFunctionString(typename.EnumToString(EditorMessageBoxResult, result));
		
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