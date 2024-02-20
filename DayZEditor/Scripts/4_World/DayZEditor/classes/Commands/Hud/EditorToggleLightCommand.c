class EditorToggleLightCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
				
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "CameraLightButton") {
			m_Editor.CameraLight = !m_Editor.CameraLight;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.CameraLight");
		}
				
		return true;
	}
	
	override string GetName() 
	{
		return "Toggle Light";
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_5 };
	}
}