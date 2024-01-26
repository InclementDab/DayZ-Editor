enum ShortcutKeyType
{
	PRESS,
	DOUBLE,
	HOLD,
	TOGGLE
};

class Command: Managed
{
	protected bool m_Executed = GetDefaultState();
	string DisplayName;
	Symbols Icon;
	
	ref ScriptInvoker OnExecute = new ScriptInvoker();
	
	void Execute(bool state) 
	{
		m_Executed = state;
		OnExecute.Invoke(m_Executed);
	}
	
	bool IsExecuted()
	{
		return m_Executed;
	}
			
	bool CanExecute()
	{
		return true;
	}
	
	bool GetDefaultState()
	{
		return false;
	}
		
	// Good default to have, makes sense in XMLs
	string GetShortcut()
	{
		return ClassName();
	}
	
	string GetShortcutString() 
	{
		string result;
		UAInput inp = GetUApi().GetInputByName(GetShortcut());
		for (int i = 0; i < inp.BindKeyCount(); i++) { 
			if (inp.CheckBindDevice(i, EInputDeviceType.MOUSE_AND_KEYBOARD)) {
				string button_name = GetUApi().GetButtonName(inp.GetBindKey(i));
				button_name.Replace("Left ", "");
				button_name.Replace("Right ", "R");
				
				result += button_name;
			}
			
			if (i != inp.BindKeyCount() - 1) {
				result += " + ";
			}
		}
		
		return result;
	}
	
	ShortcutKeyType GetShortcutType()
	{
		return ShortcutKeyType.PRESS;
	}
}