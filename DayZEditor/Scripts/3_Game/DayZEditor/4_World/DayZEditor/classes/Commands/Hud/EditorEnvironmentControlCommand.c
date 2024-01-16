class EditorEnvironmentControlCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorEnvironmentDialog dialog = new EditorEnvironmentDialog(GetName());
		DialogResult result = dialog.ShowDialog();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_ENVIRONMENT";
	}
	
	override string GetIcon() 
	{
		return "set:dayz_gui image:icon_sun";
	}
}