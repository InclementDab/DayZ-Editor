class EditorToggleSimulationCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		if (m_Editor.IsPlacing()) {
			return false;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		if (selected_objects.Count() == 0) {
			return false;
		}
		
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.Simulate = !editor_object.Simulate;
			editor_object.PropertyChanged("Simulate");
		}
		
		return true;
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_K };
	}
	
	override string GetName() 
	{
		return "Toggle Simulation";
	}
}