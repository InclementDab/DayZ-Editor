#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorUnlockAllCommand)]
#endif
class EditorUnlockAllCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		EditorObjectMap selected_objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject selected_object: selected_objects) {		
			GetEditor().UnlockObject(selected_object);
		}
		
		return true;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_LSHIFT, KeyCode.KC_L };
	}
	
	override string GetName() 
	{
		return "Unlock All";
	}

	override Symbols GetSymbol()
	{
		return Symbols.KEY_SKELETON;
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.GOLD;
	}
}