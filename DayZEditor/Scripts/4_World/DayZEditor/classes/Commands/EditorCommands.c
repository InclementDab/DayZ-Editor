class EditorCommand: RelayCommand
{	
	protected Editor m_Editor;	
	protected ref Param m_Param;
	string Text;
	
	void EditorCommand()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(Update);
	}
	
	protected void Update(float dt)
	{
	}

	override bool Execute(Class sender, CommandArgs args) 
	{
		super.Execute(sender, args);
		
		if (!m_Editor) {
			m_Editor = GetEditor();
		}
		
		if (!m_Editor) {
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
	
	Symbols GetSymbol()
	{
		return string.Empty;
	}
	
	LinearColor GetColor()
	{
		return GetEditor().GetSettings().SelectionColor;
	}
	
	bool IsToggled()
	{
		return false;
	}
		
	string GetShortcutString() 
	{
		if (GetShortcut()) {
			return GetShortcut().GetString();
		}
		
		return string.Empty;
	}
	
	void SetData(Param param)
	{
		m_Param = param;
	}
	
	Param GetData()
	{
		return m_Param;
	}
	
	ShortcutKeys GetShortcut();
}