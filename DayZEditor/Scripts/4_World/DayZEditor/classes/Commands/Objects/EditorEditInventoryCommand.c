class EditorEditInventoryCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EntityAI> data = Param1<EntityAI>.Cast(GetData());
		if (!data || !data.param1) {
			return false;
		}
		
		m_Editor.SetActive(false);
		m_Editor.StartInventoryEditor(data.param1);
		return true;
	}
	
	override string GetName()
	{
		return "Edit Inventory";
	}
}