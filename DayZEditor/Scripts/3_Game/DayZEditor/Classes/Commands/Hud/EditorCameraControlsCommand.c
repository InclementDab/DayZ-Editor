class EditorCameraControlsCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		EditorCameraDialog camera_dialog(GetName());
		camera_dialog.ShowDialog();
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CAMERA";
	}
		
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:camera_alt";
	}
}