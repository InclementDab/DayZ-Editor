class EditorExportEventPos: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EntityAI> data = Param1<EntityAI>.Cast(GetData());
		if (!data || !data.param1) {
			return false;
		}
		
		string event_data = string.Format("<group name=\"%1\">\n", data.param1.GetType());
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: editor_objects) {			
			vector position = data.param1.GetPosition();
			position = position - editor_object.GetPosition();
			event_data += string.Format("	<child type=\"%5\" deloot=\"0\" lootmax=\"5\" lootmin=\"0\" x=\"%1\" y=\"%2\" z=\"%3\" a=\"%4\" />\n", position[0], position[1], position[2], editor_object.GetAngle(), editor_object.GetType());			
		}
		
		event_data += "</group>";
		
		GetGame().CopyToClipboard(event_data);
		GetEditor().GetEditorHud().CreateNotification("Event Data copied to clipboard");
		
		return true;
	}
	
	
	override string GetName() 
	{
		return "Copy Event To Clipboard";
	}
}