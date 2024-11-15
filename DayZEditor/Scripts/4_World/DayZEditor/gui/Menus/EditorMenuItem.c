class EditorMenuItem: ScriptView
{
	static const LinearColor DEFAULT_COLOR = 0xff24282e;

	protected ImageWidget Icon;
	protected TextWidget Label, Shortcut;
	
	void EditorMenuItem()
	{
	
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

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		return true;
	}
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/Layouts/menus/EditorMenuItem.layout";
	}
}

class EditorMenuItemCommand: EditorMenuItem
{	
	protected EditorCommand m_Command;

	void EditorMenuItemCommand(notnull EditorCommand editor_command)
	{
		m_Command = editor_command;
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

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		m_Command.Execute(this, CommandArgs());

		return true;
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
	protected EditorCommand m_Command;
	protected Widget Expand;
	
	void EditorMenuItemCategory(string label, EditorMenu child_menu, Symbols icon, EditorCommand editor_command = null)
	{
		m_ChildMenu = child_menu;
		m_ChildMenu.GetLayoutRoot().Show(false);
		Expand.Show(m_ChildMenu != null);
		m_Command = editor_command;
		string symbol_icon;
		if (m_Command) {
			symbol_icon = m_Command.GetIcon();
			if (m_Command.GetSymbol()) {
				symbol_icon = m_Command.GetSymbol().Regular();
			}
			
			Shortcut.SetText(string.Format("(%1)", editor_command.GetShortcutString()));
			Shortcut.Show(editor_command.GetShortcutString() != string.Empty);
		}
		
		if (icon) {
			symbol_icon = icon.Regular();
		}
		
		Icon.Show(symbol_icon != string.Empty);
		Label.SetText(label);
		Icon.LoadImageFile(0, symbol_icon);
		Icon.SetImage(0);
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
		m_ChildMenu.GetLayoutRoot().SetPos(x1 + sx1, y1);
		m_ChildMenu.GetLayoutRoot().Show(true);
		return super.OnMouseEnter(w, x, y);
	}	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_ChildMenu.GetLayoutRoot().Show(enterW == m_ChildMenu.GetLayoutRoot().FindAnyWidget(enterW.GetName()));	//enter_widget && !enter_widget.IsInherited(ButtonWidget)
		
		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_Command) {
			m_Command.Execute(this, CommandArgs());
		}

		return true;
	}
}

class EditorOpenRecentListItem: EditorMenuItem
{	
	// todo: EditorFile struct like in EditorSaveFile
	protected string m_File;
	void EditorOpenRecentListItem(string file_path)
	{	
		m_File = file_path;
		Label.SetText(file_path);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorOpenCommand open_command = EditorOpenCommand.Cast(GetEditor().CommandManager[EditorOpenCommand]);	

		GetEditor().LoadSaveData(open_command.ImportFile(m_File), true);
		GetEditor().SetSaveFile(m_File);
		
		return true;
	}
}

class EditorPreferencesListItem: EditorMenuItem
{	
	protected string m_Setting;
	void EditorPreferencesListItem(string setting)
	{		
		m_Setting = setting;
		Label.SetText(setting);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		delete EditorHud.CurrentMenu;
		EditorPreferencesCommand preferences_command = EditorPreferencesCommand.Cast(GetEditor().CommandManager[EditorPreferencesCommand]);
		preferences_command.OpenPreferences(m_Setting);
		
		return true;
	}
}

