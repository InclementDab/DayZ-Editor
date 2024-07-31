class EditorBrushToggleCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		ButtonCommandArgs button_args = ButtonCommandArgs.Cast(args);
		
		switch (button_args.GetMouseButton()) {
			
			case 0: {
				if (!m_Editor.GetEditorHud()) {
					break;
				}

				EditorHudToolbarController toolbar_controller = m_Editor.GetEditorHud().GetTemplateController().GetToolbarController();
				bool button_state = button_args.GetButtonState();
				button_args.Source.FindAnyWidget("BrushToggleButtonText").SetPos(button_state * 1, button_state * 1);

				if (button_state) {
					m_Editor.SetBrush(EditorBrush.Create(toolbar_controller.BrushTypeBoxData[toolbar_controller.BrushTypeSelection]));
				} else {
					m_Editor.SetBrush(null);
				}
				
				m_Editor.CommandManager[EditorBrushRadiusCommand].SetCanExecute(button_state);
				m_Editor.CommandManager[EditorBrushDensityCommand].SetCanExecute(button_state);
				m_Editor.CommandManager[EditorBrushWidthCommand].SetCanExecute(button_state);
				
				break;
			}
			
			case 1: {
				m_Editor.CommandManager[EditorBrushPropertiesCommand].Execute(this, args);
				break;
			}
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_TOGGLE_BRUSH";
	}
}