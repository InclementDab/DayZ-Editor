class EditorResetAlignmentCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		data.param1.SetOrientation(vector.Zero);
		data.param1.Update();
		return true;
	}
	
	override string GetName()
	{
		return "Reset Alignment";
	}
}