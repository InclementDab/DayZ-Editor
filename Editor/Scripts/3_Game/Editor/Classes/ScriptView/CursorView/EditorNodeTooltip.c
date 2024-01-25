class EditorNodeTooltip: CursorView
{
	TextWidget Name, Description;

	void EditorNodeTooltip(Symbols cursor, string name, string desc)
	{
		Name.SetText(name);
		Description.SetText(desc);
	}
		
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Tooltip.layout";
	}
}