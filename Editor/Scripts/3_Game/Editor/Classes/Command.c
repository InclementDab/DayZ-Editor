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
	protected string m_DisplayName;
	protected Symbols m_Icon;
	
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
	
	void SetDisplayName(string display_name)
	{
		m_DisplayName = display_name;
	}
	
	string GetDisplayName() 
	{
		return m_DisplayName;
	}
	
	void SetIcon(Symbols icon)
	{
		m_Icon = icon;
	}
	
	Symbols GetIcon()
	{
		return m_Icon;
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