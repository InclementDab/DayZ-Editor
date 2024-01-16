class EditorToggleSimulationCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		if (m_Editor.IsPlacing()) {
			return false;
		}
		
		// if we are controlling the player
		EntityAI current_control = EntityAI.Cast(m_Editor.GetCurrentControl());
		if (current_control) {
			current_control.DisableSimulation(!current_control.GetIsSimulationDisabled());
			return true;
		}
		
		array<EditorObject> selected_objects = EditorObject.GetSelectedEditorObjects();
		if (selected_objects.Count() == 0) {
			return false;
		}
		
		foreach (int id, EditorObject editor_object: selected_objects) {
			EntityAI entity = EntityAI.Cast(editor_object.GetWorldObject());
			if (!entity) {
				continue;
			}
			
			entity.DisableSimulation(!entity.GetIsSimulationDisabled());
		}
		
		return true;
	}
	
	override string GetName() 
	{
		return "Toggle Simulation";
	}
}