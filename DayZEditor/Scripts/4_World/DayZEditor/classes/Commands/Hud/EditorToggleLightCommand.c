#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorToggleLightCommand)]
#endif
class EditorToggleLightCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
				
		EditorCamera camera = m_Editor.GetCamera();
		camera.SetLightState(!camera.GetLightState());	
		return true;
	}
	
	override string GetName() 
	{
		return "Toggle Light";
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LSHIFT, KeyCode.KC_L };
	}
	
	override LinearColor GetColor()
	{
		return LinearColor.GOLDENROD;
	}

	override Symbols GetSymbol()
	{
		return Symbols.FLASHLIGHT;
	}
	
	override bool IsToggled()
	{
		return GetEditor().GetCamera() && GetEditor().GetCamera().GetLightState();
	}
}