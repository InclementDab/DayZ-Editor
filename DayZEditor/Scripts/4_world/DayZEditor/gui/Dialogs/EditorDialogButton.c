class EditorDialogButton: EditorScriptView
{	
	protected ButtonWidget DialogButton;
	protected ref ScriptInvoker m_ExecuteInvoker = new ScriptInvoker();
	
	protected string m_ExecuteAction;
	
	void EditorDialogButton(Widget parent = null)
	{
		EditorLog.Trace("EditorDialogButton"); 
	}
	
	void ~EditorDialogButton()
	{
		EditorLog.Trace("~EditorDialogButton");
		delete m_ExecuteInvoker;
	}
	
	bool DialogButtonExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("DialogButtonExecute");
		
		if (m_ExecuteAction == string.Empty) {
			m_ExecuteInvoker.Invoke();
		} else {
			g_Script.Call(GetParent(), m_ExecuteAction, args);
		}
		return true;
	}
	
	void SetExecuteFunction(func function)
	{
		m_ExecuteInvoker.Insert(function);
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