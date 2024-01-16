enum EditorShortcutKeyType
{
	PRESS,
	DOUBLE,
	HOLD
};

class EditorCommand: RelayCommand
{	
	protected Editor m_Editor;
	protected ref Param m_Param;
	string Text;

	override bool Execute(Class sender, CommandArgs args) 
	{
		super.Execute(sender, args);
		if (EditorHud.CurrentMenu) {
			delete EditorHud.CurrentMenu;		
		}
		
		if (!m_Editor) {
			m_Editor = GetEditor();
		}
		
		// Needs to be since we do ShowDialog alot
		if (!m_Editor) {
			Error("EditorCommand::Editor was null!");
			return true;
		} 
	
		return false;
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
	
	void SetData(Param param)
	{
		m_Param = param;
	}
	
	Param GetData()
	{
		return m_Param;
	}
	
	// Good default to have, makes sense in XMLs
	string GetShortcut()
	{
		return ClassName();
	}
	
	EditorShortcutKeyType GetShortcutType()
	{
		return EditorShortcutKeyType.PRESS;
	}
}