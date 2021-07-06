class EditorReloadBrushesCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().BrushTypeBoxData.Clear();
		m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().ReloadBrushes(m_Editor.Settings.EditorBrushFile);
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_RELOAD_BRUSHES";
	}
}