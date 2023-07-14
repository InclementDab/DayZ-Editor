class EditorGroundCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "GroundButton") {
			m_Editor.GroundMode = !m_Editor.GroundMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.GroundMode");
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_GROUND_MODE";
	}
}