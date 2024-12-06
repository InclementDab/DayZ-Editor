#ifndef COMPONENT_SYSTEM
[RegisterEditorCommand(EditorLightningCommand)]
#endif
class EditorLightningCommand: EditorCommand
{
    protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		if (GetEditor().IsPlacing()) {
			GetEditor().CancelPlacing();
		}
		
        GetEditor().LightningMode = !GetEditor().LightningMode;
		return true;
	}
	
	override string GetName() 
	{
		return "Lightning";
	}

    override LinearColor GetColor()
    {
        return LinearColor.YELLOW;
    }

    override bool IsToggled()
    {
        return GetEditor().LightningMode;
    }
}