class EditorExportEventPos: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EntityAI> data = Param1<EntityAI>.Cast(GetData());
		if (!data || !data.param1) {
			return false;
		}
		
		string event_data = "<event name=\"DefaultEvent\"\n";
		event_data += "	<zone smin=\"0\" smax=\"0\" dmin=\"1\" dmax=\"2\" s=\"20\">\n";
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: editor_objects) {
			if (editor_object.GetWorldObject() == data.param1) {
				continue;
			}
			
			vector position = data.param1.GetPosition();
			position = position - editor_object.GetPosition();			
			event_data += string.Format("	<pos x=\"%1\" y=\"%2\" z=\"%3\" a=\"%4\" group=\"%5\"\n", position[0], position[1], position[2], editor_object.GetAngle(), editor_object.GetType());			
		}
		
		event_data += "</event>";
		
		GetGame().CopyToClipboard(event_data);
		GetEditor().GetEditorHud().CreateNotification("Event Data copied to clipboard");
		
		return true;
	}
	
	
	override string GetName() 
	{
		return "Copy Event To Clipboard";
	}
}