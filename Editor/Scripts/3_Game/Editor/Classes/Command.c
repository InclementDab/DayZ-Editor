enum ShortcutKeyType
{
	PRESS,
	DOUBLE,
	HOLD,
	TOGGLE
};

class Command: Managed
{
	void Execute(bool state) 
	{
	}
			
	bool CanExecute()
	{
		return true;
	}
	
	string GetName() 
	{
		return string.Empty;
	}
	
	string GetIcon() 
	{
		return string.Empty;
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
	
	
	// Good default to have, makes sense in XMLs
	string GetShortcut()
	{
		return ClassName();
	}
	
	ShortcutKeyType GetShortcutType()
	{
		return ShortcutKeyType.PRESS;
	}
}