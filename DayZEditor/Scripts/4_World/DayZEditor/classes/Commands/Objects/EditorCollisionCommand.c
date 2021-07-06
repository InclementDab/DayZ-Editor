class EditorCollisionCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		if (!button_args || button_args.Source.GetName() != "CollisionButton") {
			m_Editor.CollisionMode = !m_Editor.CollisionMode;
			m_Editor.GetEditorHud().GetTemplateController().GetToolbarController().NotifyPropertyChanged("m_Editor.CollisionMode");
		}
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_COLLISION_MODE";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_4 };
	}
}
