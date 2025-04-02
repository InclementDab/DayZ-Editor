class EditorMenuItem: ScriptView
{
	static const LinearColor DEFAULT_COLOR = 0xff24282e;

	protected EditorMenu m_ParentMenu;
	protected EditorCommand m_Command;
	protected ImageWidget Icon;
	protected TextWidget Label, Shortcut;
	
	void EditorMenuItem(notnull EditorMenu editor_menu, EditorCommand editor_command = null)
	{
		m_ParentMenu = editor_menu;
		m_Command = editor_command;
		if (m_Command) {
			string symbol_icon = m_Command.GetIcon();
			if (m_Command.GetSymbol()) {
				symbol_icon = m_Command.GetSymbol().Regular();
			}
			
			Icon.Show(symbol_icon != string.Empty);
			Label.SetText(m_Command.GetName());
			Shortcut.SetText(string.Format("(%1)", editor_command.GetShortcutString()));
			Shortcut.Show(editor_command.GetShortcutString() != string.Empty);
			Icon.LoadImageFile(0, symbol_icon);
			Icon.SetImage(0);
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		m_LayoutRoot.SetColor(GetEditor().GetSettings().HighlightColor);
		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_LayoutRoot.SetColor(DEFAULT_COLOR);

		return super.OnMouseLeave(w, enterW, x, y);
	}

	protected void CloseMenu()
	{
		if (m_ParentMenu) {
			m_ParentMenu.Delete();
		}
	}

	EditorMenu GetParentMenu()
	{
		return m_ParentMenu;
	}

	EditorCommand GetCommand()
	{
		return m_Command;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_Command) {
			m_Command.Execute(this, CommandArgs());
		}

		if (ClosesAfterInteraction()) {
			CloseMenu();
		}

		return true;
	}

	bool ClosesAfterInteraction()
	{
		return true;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenuItem.layout";
	}
}

class EditorMenuItemDivider: EditorMenuItem
{	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenuItemDivider.layout";
	}
}

class EditorMenuItemCategory: EditorMenuItem
{
	protected ref EditorMenu m_ChildMenu;
	protected typename m_ChildMenuType;
	protected Widget Expand;
	
	void EditorMenuItemCategory(notnull EditorMenu editor_menu, EditorCommand editor_command = null, string label = string.Empty, typename child_menu_type = EMPTY_TYPENAME, Symbols icon = string.Empty)
	{
		m_ChildMenuType = child_menu_type;
		if (!m_ChildMenuType.IsInherited(EditorMenu)) {
			ErrorEx("ChildMenuType must inherit from EditorMenu");
		}
		
		Expand.Show(m_ChildMenuType != EMPTY_TYPENAME);
		Label.SetText(label);
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_Command && !m_Command.CanExecute()) {
			return true;
		}
		
		// Setting this here because menu root needs to be moved before we know where we are
		float x1, y1;
		float sx1, sy1;
		m_LayoutRoot.GetScreenPos(x1, y1);
		m_LayoutRoot.GetScreenSize(sx1, sy1);
		
		m_ChildMenu = EditorMenu.Cast(m_ChildMenuType.Spawn());
		m_ChildMenu.GetLayoutRoot().SetPos(x1 + sx1, y1);
		m_ChildMenu.GetLayoutRoot().Show(true);
		return super.OnMouseEnter(w, x, y);
	}	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (enterW != m_ChildMenu.GetLayoutRoot().FindAnyWidget(enterW.GetName())) {
			delete m_ChildMenu;
		}
				
		return super.OnMouseLeave(w, enterW, x, y);
	}
}

class EditorOpenRecentListItem: EditorMenuItem
{	
	// todo: EditorFile struct like in EditorSaveFile
	protected string m_File;
	void EditorOpenRecentListItem(notnull EditorMenu editor_menu, EditorCommand editor_command = null, string file_path = string.Empty)
	{	
		m_File = file_path;
		Label.SetText(file_path);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorOpenCommand open_command = EditorOpenCommand.Cast(GetEditor().CommandManager[EditorOpenCommand]);	

		GetEditor().LoadSaveData(open_command.ImportFile(m_File), true);
		GetEditor().SetSaveFile(m_File);
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
}

class EditorPreferencesListItem: EditorMenuItem
{	
	protected string m_Setting;
	void EditorPreferencesListItem(notnull EditorMenu editor_menu, EditorCommand editor_command = null, string setting = string.Empty)
	{		
		m_Setting = setting;
		Label.SetText(setting);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		delete EditorHud.CurrentMenu;
		EditorPreferencesCommand preferences_command = EditorPreferencesCommand.Cast(GetEditor().CommandManager[EditorPreferencesCommand]);
		preferences_command.OpenPreferences(m_Setting);
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
}

