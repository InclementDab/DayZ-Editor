class EditorContextMenu: EditorMenu
{
	protected float m_X, m_Y;
		
	void EditorContextMenu(float x, float y)
	{
		m_X = x; m_Y = y;
		
		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
	}
	
	void ~EditorContextMenu()
	{
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}
	
	void UpdatePosition()
	{		
		float x = m_X;
		float y = m_Y;
		int sx, sy;
		GetScreenSize(sx, sy);
		
		float lw, lh;
		m_LayoutRoot.GetScreenSize(lw, lh);
		
		// bounds XY to size of screen
		if (x > sx - lw) {
			x -= lw;
		}
		
		// I have to do this shit because of Race conditions.... wowzers
		float menu_height;
		for (int i = 0; i < m_TemplateController.MenuItems.Count(); i++) {
			if (!m_TemplateController.MenuItems[i]) {
				continue;
			}
			
			float llw, llh;
			m_TemplateController.MenuItems[i].GetLayoutRoot().GetScreenSize(llw, llh);
			menu_height += llh;
		}	
		
		if (y > sy - menu_height) {
			y -= menu_height;
		}
		
		m_LayoutRoot.SetPos(Math.Clamp(x, 0, sx - lw), Math.Clamp(y, 0, sy - lh));		
	}
	
	override void AddMenuItem(EditorMenuItem menu_item)
	{
		super.AddMenuItem(menu_item);
		UpdatePosition();
	}
}