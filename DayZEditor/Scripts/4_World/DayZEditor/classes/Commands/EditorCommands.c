class EditorCommand: RelayCommand
{	
	protected Editor m_Editor;	
	protected ref Param m_Param;
	string Text;

	override bool Execute(Class sender, CommandArgs args) 
	{
		EditorLog.Trace("EditorCommand::Execute");
		super.Execute(sender, args);
		if (EditorHud.CurrentMenu) {
			delete EditorHud.CurrentMenu;		
		}
		
		if (!m_Editor) {
			m_Editor = GetEditor();
		}
		
		// Needs to be since we do ShowDialog alot
		if (!m_Editor) {
			EditorLog.Error("EditorCommand::Editor was null!");
			return true;
		} 
	
		return false;
	}
	
	override void CanExecuteChanged(bool state) 
	{
		//EditorLog.Trace("CanExecuteChanged: %1 - %2", state.ToString(), m_ViewBinding.GetLayoutRoot().GetName());
		
		if (m_ViewBinding) {
			Widget root = m_ViewBinding.GetLayoutRoot();
			if (state) {
				root.SetAlpha(1);
			} else {
				root.SetAlpha(0.15);
			}
			
			root.Enable(state);			
		}		
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
		map<int, ref array<string>> input_strings = InputUtils.GetComboButtonNamesFromInput(GetShortcut(), EInputDeviceType.MOUSE_AND_KEYBOARD);
		foreach (int _, array<string> key_strings: input_strings) {
			for (int i = 0; i < key_strings.Count(); i++) {
				result += key_strings[i];
				if (i != key_strings.Count() - 1) {
					result += "+";
				}
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
}