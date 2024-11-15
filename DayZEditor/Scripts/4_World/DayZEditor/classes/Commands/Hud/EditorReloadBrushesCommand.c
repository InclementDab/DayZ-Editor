class EditorReloadBrushesCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.GetEditorHud().ReloadBrushes(m_Editor.GetSettings().EditorBrushFile);
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_RELOAD_BRUSHES";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.ARROWS_ROTATE;
	}
}