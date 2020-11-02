class EditorContextMenu: EditorMenu
{
	void EditorContextMenu(float x, float y)
	{
		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
		
		int sx, sy;
		GetScreenSize(sx, sy);
		
		float lw, lh;
		m_LayoutRoot.GetScreenSize(lw, lh);
		
		// bounds XY to size of screen
		if (x > sx - lw) {
			x -= lw;
		}
		
		// 300 is a dummy number since i cant get the full size of the context menu
		if (y > sy - 300) {
			y -= 300;
		}
				
		m_LayoutRoot.SetPos(Math.Clamp(x, 0, sx - lw), Math.Clamp(y, 0, sy - lh));
	}
	
	void ~EditorContextMenu()
	{
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}
}

class EditorPlacedContextMenu: EditorContextMenu
{
	void EditorPlacedContextMenu(float x, float y) 
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		AddMenuButton(m_Editor.CommandManager.CutCommand);
		AddMenuButton(m_Editor.CommandManager.CopyCommand);
		AddMenuButton(m_Editor.CommandManager.PasteCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.ShowCommand);
		AddMenuButton(m_Editor.CommandManager.HideCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.DeleteCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.ObjectPropertiesCommand);
	}
}

class EditorPlaceableContextMenu: EditorContextMenu
{
	void EditorPlaceableContextMenu(float x, float y)
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		AddMenuButton(m_Editor.CommandManager.AddToFavoritesCommand);
		AddMenuButton(m_Editor.CommandManager.LootEditorCommand);
	}
}