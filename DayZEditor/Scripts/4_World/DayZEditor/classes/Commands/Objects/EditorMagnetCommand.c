class EditorMagnetCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "MagnetButton") {
			m_Editor.MagnetMode = !m_Editor.MagnetMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.MagnetMode");
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_MAGNET_MODE";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_1 };
	}
}