class EditorSnapCommand: EditorCommand
{	
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "SnapButton") {
			m_Editor.SnappingMode = !m_Editor.SnappingMode;
			m_Editor.GetEditorHud().GetTemplateController().NotifyPropertyChanged("m_Editor.SnappingMode");
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_SNAPPING_MODE";
	}
}