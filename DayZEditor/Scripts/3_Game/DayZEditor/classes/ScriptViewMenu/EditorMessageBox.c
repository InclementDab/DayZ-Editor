class EditorMessageBoxButton: ScriptView
{
	protected ref ScriptCaller m_OnClick;
	protected DialogResult m_DialogResult;
	ButtonWidget Button;
	void EditorMessageBoxButton(DialogResult result, ScriptCaller on_click)
	{
		m_DialogResult = result;
		m_OnClick = on_click;
		Button.SetText(DialogBase.GetDialogResultText(result));
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_OnClick) {
			m_OnClick.Invoke(this);
		}

		return super.OnClick(w, x, y, button);
	}

	DialogResult GetResult()
	{
		return m_DialogResult;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/MessageBoxButton.layout";
	}
}

class EditorMessageBoxController: ViewController
{
	ref ObservableCollection<ref EditorMessageBoxButton> Buttons = new ObservableCollection<ref EditorMessageBoxButton>(this);
}

class EditorMessageBox: EditorModal
{
	protected ref ScriptCaller m_Callback;
	RichTextWidget Text;

	protected ref map<ButtonWidget, DialogResult> m_ButtonMap = new map<ButtonWidget, DialogResult>();
	protected EditorMessageBoxController m_TemplateController;

	void EditorMessageBox(string content, MessageBoxButtons buttons, ScriptCaller callback)
	{
		Text.SetText(content);
		m_Callback = callback;
		m_TemplateController = EditorMessageBoxController.Cast(m_Controller);
		switch (buttons) {
			case MessageBoxButtons.OK: {
				AddButton(DialogResult.OK);
				break;
			}
			
			case MessageBoxButtons.OKCancel: {
				AddButton(DialogResult.OK);
				AddButton(DialogResult.Cancel);
				break;
			}
			
			case MessageBoxButtons.AbortRetryIgnore: {
				AddButton(DialogResult.Abort);
				AddButton(DialogResult.Retry);
				AddButton(DialogResult.Ignore);
				break;
			}
			
			case MessageBoxButtons.YesNoCancel: {
				AddButton(DialogResult.Yes);
				AddButton(DialogResult.No);
				AddButton(DialogResult.Cancel);
				break;
			}
			
			case MessageBoxButtons.YesNo: {
				AddButton(DialogResult.Yes);
				AddButton(DialogResult.No);
				break;
			}			
			
			case MessageBoxButtons.RetryCancel: {
				AddButton(DialogResult.Retry);
				AddButton(DialogResult.Cancel);
				break;
			}
		}
	}
	
	protected void AddButton(DialogResult result)
	{
		m_TemplateController.Buttons.Insert(new EditorMessageBoxButton(result, ScriptCaller.Create(OnClickedButton)));
	}

	protected void OnClickedButton(EditorMessageBoxButton button)
	{
		if (m_Callback) {
			m_Callback.Invoke(button.GetResult());
		}

		Delete();
	}

	override string GetTitle()
	{
		return "Editor Message";
	}

	override typename GetControllerType()
	{
		return EditorMessageBoxController;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/MessageBox.layout";
	}
}