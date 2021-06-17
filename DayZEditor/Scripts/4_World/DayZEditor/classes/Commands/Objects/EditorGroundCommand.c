class EditorGroundCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "GroundButton") {
			m_Editor.GroundMode = !m_Editor.GroundMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.GroundMode");
		}
	}
	
	override string GetName() 
	{
		return "Toggle Ground Mode";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_2 };
	}
}