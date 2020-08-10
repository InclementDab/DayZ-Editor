class EditorContextMenu: UILinkedObject
{
	
	void EditorContextMenu()
	{
		Print("EditorContextMenu");
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w.GetName()) {
			
			
			case "SetTargetButton": {
				Editor.SetCameraTarget(m_EditorObject.GetWorldObject());
				m_Root.Show(false);
				return true;
			}
		}
		
		
		return false;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		m_Root.Show(false);
		return true;
	}
	
}
