class DialogButton: ScriptView
{	
	protected ButtonWidget Button;	
	protected string m_ExecuteAction;
	
	void DialogButton(Widget parent = null, string label = "", string callback = "")
	{
		Trace("DialogButton"); 
		Button.SetText(label);
		m_ExecuteAction = callback;
	}
	
	void ~DialogButton()
	{
		Trace("~DialogButton");
	}
	
	bool ButtonExecute(ButtonCommandArgs args)
	{
		Trace("ButtonExecute");
		g_Script.CallFunction(GetParent(), m_ExecuteAction, null, this);
		return true;
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/DialogButton.layout";
	}
}