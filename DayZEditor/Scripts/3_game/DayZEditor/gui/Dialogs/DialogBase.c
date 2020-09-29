class DialogBase: ScriptView
{
	// Private members
	private DialogResult m_DialogResult = DialogResult.None;
	
	protected DialogBaseController m_DialogBaseController;
	
	void DialogBase(Widget parent = null, string title = "", string caption = "")
	{
		Class.CastTo(m_DialogBaseController, m_Controller);
		
		m_DialogBaseController.Title = title;
		m_DialogBaseController.Caption = caption;
		
		// Update all properties
		m_DialogBaseController.NotifyPropertyChanged();
	}
	
	DialogResult ShowDialog()
	{
		Trace("ShowDialog");
		//m_LayoutRoot.Show(true);
		while (m_DialogResult == DialogResult.None) {
			Sleep(1);
		}
		
		Log("DialogResult: %1", typename.EnumToString(DialogResult, m_DialogResult));
		return m_DialogResult;
	}
	
	void CloseDialog(DialogResult dialog_result = DialogResult.Cancel)
	{
		Trace("CloseDialog");
		m_DialogResult = dialog_result;
		delete this;
	}
	
	void AddContent(ScriptView content)
	{
		m_DialogBaseController.DialogContentData.Insert(content);
	}
	
	void AddButton(ScriptView button)
	{
		m_DialogBaseController.DialogButtonData.Insert(button);
	}
	
	override typename GetControllerType() {
		return DialogBaseController;
	}	
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/DialogBase.layout";
	}
}
