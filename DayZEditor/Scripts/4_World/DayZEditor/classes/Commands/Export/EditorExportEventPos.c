class EditorExportEventPos : EditorCommand
{

	string SerializeEvents()
	{
		string xml = string.Format("<!---events.xml-->\n");
		xml += string.Format("<event name=\"StaticMyEvent\">\n");
		xml += string.Format("    <nominal>1</nominal>\n");
		xml += string.Format("    <min>1</min>\n");
		xml += string.Format("    <max>1</max>\n");
		xml += string.Format("    <lifetime>5400</lifetime>\n");
		xml += string.Format("    <restock>0</restock>\n");
		xml += string.Format("    <saferadius>500</saferadius>\n");
		xml += string.Format("    <distanceradius>1000</distanceradius>\n");
		xml += string.Format("    <cleanupradius>1000</cleanupradius>\n");
		xml += string.Format("    <flags deletable=\"1\" init_random=\"0\" remove_damaged=\"0\" />\n");
		xml += string.Format("    <position>fixed</position>\n");
		xml += string.Format("    <limit>child</limit>\n");
		xml += string.Format("    <active>1</active>\n");
		xml += string.Format("    <children />\n");
		xml += "</event>\n";
		return xml;
	}
	string SerializeCfgeventspawns(vector centerpos)
	{
		string xml = string.Format("<!---cfgeventspawns.xml-->\n");
		xml += string.Format("<event name=\"StaticMyEvent\">\n");
		xml += string.Format("		<pos x=\"%1\" y=\"%2\" z=\"%3\" a=\"0\" group=\"MyEventGroup\" />\n", centerpos[0], centerpos[1], centerpos[2]);
		xml += "</event>\n";
		return xml;
	}
	string SerializeEventGroups(vector centerpos)
	{
		EditorObjectMap editor_objects = m_Editor.GetSelectedObjects();
		string xml = string.Format("<!---cfgeventgroups.xml-->\n");
		xml += string.Format("<group name=\"MyEventGroup\">\n");
		foreach (int id, EditorObject editor_object: editor_objects) {
			vector position = editor_object.GetPosition() - centerpos;
			vector orientation = editor_object.GetOrientation();
			if (orientation[0] < 0)
			{
				orientation[0] = orientation[0] + 360;
			}
			xml += string.Format("        <child type=\"%1\" deloot=\"0\" lootmax=\"5\" lootmin=\"0\" x=\"%2\" y=\"%3\" z=\"%4\" a=\"%5\" />\n", editor_object.GetType(), position[0], position[1], position[2], orientation[0]);
		}
		xml += "</group>\n";
		return xml;
	}

	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EntityAI> data = Param1<EntityAI>.Cast(GetData());
		if (!data || !data.param1)
		{
			return false;
		}
		vector centerpos = data.param1.GetPosition();
		GetGame().CopyToClipboard(SerializeEvents() + SerializeCfgeventspawns(centerpos) + SerializeEventGroups(centerpos));
		GetEditor().GetEditorHud().CreateNotification("Event Data copied to clipboard");

		return true;
	}

	override string GetName()
	{
		return "Copy Event To Clipboard";
	}
}