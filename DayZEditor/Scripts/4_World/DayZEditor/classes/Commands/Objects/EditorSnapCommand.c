class EditorSnapCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "SnapButton") {
			m_Editor.SnappingMode = !m_Editor.SnappingMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.SnappingMode");
		}
	}
	
	override string GetName() 
	{
		return "Toggle Snapping";
	}

	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_3 };
	}
}