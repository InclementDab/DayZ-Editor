class DialogButton: ScriptView
{
	ButtonWidget Button;	
	protected string m_Callback;
	
	void DialogButton(string label, string callback)
	{
		Button.SetText(label);
		m_Callback = callback;
	}
		
	bool ButtonExecute(ButtonCommandArgs args)
	{
		Trace("ButtonExecute");
		g_Script.CallFunction(GetParent(), m_Callback, null, this);
		return true;
	}
			
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/DialogButton.layout";
	}
}