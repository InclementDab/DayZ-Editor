class EditorReloadBrushesCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().BrushTypeBoxData.Clear();
		m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().ReloadBrushes(m_Editor.Settings.EditorBrushFile);
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_RELOAD_BRUSHES";
	}
}