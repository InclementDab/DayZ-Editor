class EditorEntityTooltip: ScriptView
{	
	ItemPreviewWidget Item;
	
	void EditorEntityTooltip(notnull EntityAI entity)
	{
		Item.SetItem(entity);
		Item.SetView(entity.GetViewIndex());
	}
			
	override string GetLayoutFile() 
	{
		return "Editor\\GUI\\layouts\\ObjectTooltip.layout";
	}
}