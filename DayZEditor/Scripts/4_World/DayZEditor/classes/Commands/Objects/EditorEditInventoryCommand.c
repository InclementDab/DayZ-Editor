class EditorEditInventoryCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param2<EntityAI, EditorObject> data = Param2<EntityAI, EditorObject>.Cast(GetData());
		if (!data || !data.param1) {
			return false;
		}
		
		m_Editor.StartInventoryEditor(data.param1, data.param2.GetData());
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_EDIT_INVENTORY";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LMENU, KeyCode.KC_T };
	}
}