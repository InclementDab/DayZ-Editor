

class EditorMenuController: Controller
{
	ref ObservableCollection<ref EditorMenuItem> MenuItems = new ObservableCollection<ref EditorMenuItem>(this);
	
	void ~EditorMenuController()
	{
		delete MenuItems;
	}
}

class EditorMenu: EditorScriptView
{	
	protected EditorMenuController m_EditorMenuController;	
	
	void EditorMenu()
	{
		EditorLog.Trace("EditorMenu");
		m_EditorMenuController = EditorMenuController.Cast(GetController());
	}
		
	void ~EditorMenu() 
	{
		EditorLog.Trace("~EditorMenu");
	}
	
	void AddMenuCategory(string label, typename child_menu)
	{
		if (child_menu.IsInherited(EditorMenu)) {
			AddMenuCategory(label, child_menu.Spawn());
		}
	}
	
	void AddMenuCategory(string label, EditorMenu child_menu)
	{
		AddMenuItem(new EditorMenuItemCategory(label, child_menu));
	}
			
	void AddMenuDivider()
	{	
		AddMenuItem(new EditorMenuItemDivider());
	}

	void AddMenuButton(EditorCommand editor_command)
	{
		AddMenuItem(new EditorMenuItemCommand(editor_command));
	}

	void AddMenuItem(EditorMenuItem menu_item)
	{		
		m_EditorMenuController.MenuItems.Insert(menu_item);
	}
		
	void RemoveMenuItem(EditorMenuItem menu_item)
	{
		m_EditorMenuController.MenuItems.Remove(m_EditorMenuController.MenuItems.Find(menu_item));
	}
			
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
	
	override typename GetControllerType() {
		return EditorMenuController;
	}
}

class EditorFileMenu: EditorMenu
{
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddMenuButton(m_Editor.CommandManager.NewCommand);
		AddMenuButton(m_Editor.CommandManager.OpenCommand);
		AddMenuButton(m_Editor.CommandManager.SaveCommand);
		AddMenuButton(m_Editor.CommandManager.SaveAsCommand);
		AddMenuButton(m_Editor.CommandManager.CloseCommand);
		AddMenuDivider();
		AddMenuCategory("Import", EditorImportMenu);
		AddMenuCategory("Export", EditorExportMenu);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.ExitCommand);
	}
}

class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
				
		AddMenuButton(m_Editor.CommandManager.UndoCommand);
		AddMenuButton(m_Editor.CommandManager.RedoCommand);
		AddMenuDivider();
		AddMenuCategory("Edit", EditorClipboardMenu);
		AddMenuCategory("Transform Mode", EditorTransformModeMenu);
		AddMenuDivider();
		
		AddMenuButton(m_Editor.CommandManager.ScriptEditorCommand);
	}
}

class EditorTransformModeMenu: EditorMenu
{
	void EditorTransformModeMenu()
	{
		EditorLog.Trace("EditorTransformModeMenu");
		
		AddMenuButton(m_Editor.CommandManager.MagnetCommand);
		AddMenuButton(m_Editor.CommandManager.GroundCommand);
		AddMenuButton(m_Editor.CommandManager.SnapCommand);
		AddMenuButton(m_Editor.CommandManager.CollisionCommand);
	}
}

class EditorExportMenu: EditorMenu
{
	void EditorExportMenu()
	{
		EditorLog.Trace("EditorExportMenu");
		
		AddMenuButton(m_Editor.CommandManager.ExportToInitFile);
		AddMenuButton(m_Editor.CommandManager.ExportToExpansion);
		AddMenuButton(m_Editor.CommandManager.ExportToTerrainBuilder);
		AddMenuButton(m_Editor.CommandManager.ExportToVPP);
	}
}

class EditorImportMenu: EditorMenu
{
	void EditorImportMenu()
	{
		EditorLog.Trace("EditorImportMenu");
		
		AddMenuButton(m_Editor.CommandManager.ImportFromExpansionCommand);
		AddMenuButton(m_Editor.CommandManager.ImportFromTerrainBuilderCommand);
		AddMenuButton(m_Editor.CommandManager.ImportFromVPPCommand);
		AddMenuButton(m_Editor.CommandManager.ImportFromCOM);
	}
}

class EditorClipboardMenu: EditorMenu
{
	void EditorClipboardMenu()
	{
		EditorLog.Trace("EditorClipboardMenu");
		
		AddMenuButton(m_Editor.CommandManager.CutCommand);
		AddMenuButton(m_Editor.CommandManager.CopyCommand);
		AddMenuButton(m_Editor.CommandManager.PasteCommand);	
	}
}

class EditorViewMenu: EditorMenu
{	
	void EditorViewMenu()
	{
		EditorLog.Trace("EditorViewMenu");

		AddMenuButton(m_Editor.CommandManager.CameraControlsCommand);
	}
}

class EditorEditorMenu: EditorMenu
{
	void EditorEditorMenu()
	{
		EditorLog.Trace("EditorEditorMenu");
		
		AddMenuButton(m_Editor.CommandManager.ReloadHudCommand);
		AddMenuButton(m_Editor.CommandManager.ReloadBrushesCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.EnvironmentControlCommand);
		AddMenuButton(m_Editor.CommandManager.PreferencesCommand);
		AddMenuDivider();
		AddMenuButton(m_Editor.CommandManager.DonateCommand);
		AddMenuButton(m_Editor.CommandManager.HelpCommand);
	}
}

class EditorContextMenu: EditorMenu
{
	
	void EditorContextMenu(float x, float y)
	{
		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
		
		m_LayoutRoot.SetPos(x, y);
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
		AddMenuButton(m_Editor.CommandManager.LootEditorCommand);
		AddMenuButton(m_Editor.CommandManager.ObjectPropertiesCommand);
	}
}




