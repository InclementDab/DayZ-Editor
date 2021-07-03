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
		
		thread Call(sender, args);
		
		return true;
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
	
	// Abstracts
	protected void Call(Class sender, CommandArgs args);
	
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