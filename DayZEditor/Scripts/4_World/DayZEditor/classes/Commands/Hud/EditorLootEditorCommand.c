class EditorLootEditorCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		Print(GetEditor().GetObjectManager().CurrentSelectedItem);
		if (GetEditor().GetObjectManager().CurrentSelectedItem)
			m_Editor.EditLootSpawns(GetEditor().GetObjectManager().CurrentSelectedItem);
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_LOOT_EDITOR";
	}
}