class EditorTooltip: CursorView
{
	TextWidget Name, Description;

	void EditorTooltip(Symbols cursor, string name, string desc)
	{
		Name.SetText(name);
		Description.SetText(desc);
	}
		
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Tooltip.layout";
	}
}