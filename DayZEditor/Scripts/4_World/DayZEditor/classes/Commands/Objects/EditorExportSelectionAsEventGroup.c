[RegisterEditorCommand(EditorExportSelectionAsEventGroup)]
class EditorExportSelectionAsEventGroup: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		auto selected_objects = GetEditor().GetSelectedObjects().GetValueArray();
		if (selected_objects.Count() == 0) {
			return false;
		}
		
		vector position = data.param1.GetPosition();
		vector orientation = data.param1.GetOrientation();
		vector mat[4];
		data.param1.GetTransform(mat);
		float angle = orientation[0];
		
		string event_line = "<!-- Spawn -->\n<event name=\"EditorExport\">\n";
		event_line += "\t<zone smin=\"0\" smax=\"0\" dmin=\"1\" dmax=\"1\" r=\"20\" />\n";
		event_line += string.Format("\t<pos x=\"%1\" z=\"%2\" a=\"%3\" y=\"%4\" group=\"EditorGroup0\"/>\n", position[0], position[2], 0, position[1]);
		event_line += "</event>";
		
		event_line += "\n\n<!-- Group -->\n";
		event_line += "<group name=\"EditorGroup0\">\n";
		
		// manual line for the source object
		event_line += string.Format("\t<child type=\"%1\" deloot=\"1\" lootmax=\"3\" lootmin=\"1\" x=\"%2\" z=\"%3\" a=\"%4\" y=\"%5\"/>\n", data.param1.GetType(), 0, 0, angle, 0);
		for (int i = 0; i < selected_objects.Count(); i++) {
			if (!selected_objects[i] || !selected_objects[i].GetWorldObject()) {
				continue;
			}
			
			if (selected_objects[i] == data.param1) {
				continue;
			}
			
			//vector mat2[4];
			//selected_objects[i].GetTransform(mat2);
			
			vector pos2 = selected_objects[i].GetPosition() - data.param1.GetPosition();
			float angle2 = selected_objects[i].GetOrientation()[0];
			
			//vector resultmat[4];
			//Math3D.MatrixInvMultiply4(mat2, mat, resultmat);
			//float angle2 = Math3D.MatrixToAngles(resultmat)[0];
			//vector pos2 = resultmat[3];
			event_line += string.Format("\t<child type=\"%1\" deloot=\"1\" lootmax=\"3\" lootmin=\"1\" x=\"%2\" z=\"%3\" a=\"%4\" y=\"%5\"/>\n", selected_objects[i].GetType(), pos2[0], pos2[2], angle2, pos2[1]);
		}
		
		event_line += "</group>";
				
		GetGame().CopyToClipboard(event_line);
		
		/*
		<!-- Spawn -->
		<event name="EditorExport">
		    <zone smin="0" smax="0" dmin="1" dmax="1" r="20" />
		    <pos x="11826.267578125" z="16667.583984375" a="0" y="9.481478691101074" group="Test_1"/>
		</event>
		<!-- Group -->
		<group name="Test_1">
		    <child type="Land_CDAJA2_CamoNet_NATO" spawnsecondary="false" lootmax="3" lootmin="1" x="0" z="0" a="248.22341918945312" y="0"/>
		    <child type="StaticObj_Misc_SupplyBox2_DE" deloot="1" lootmax="3" lootmin="1" x="2.1533203125" z="-2.82421875" a="183.76808166503906" y="-0.3164787292480469"/>
		    <child type="StaticObj_Misc_SupplyBox2_DE" deloot="1" lootmax="3" lootmin="1" x="0.3212890625" z="1.3046875" a="272.637451171875" y="-0.3168954849243164"/>
		    <child type="Land_Roadblock_Table" deloot="1" lootmax="3" lootmin="1" x="5.1396484375" z="-0.59765625" a="327.5442695617676" y="-0.6588754653930664"/>
		    <child type="Land_Roadblock_Table" deloot="1" lootmax="3" lootmin="1" x="3.4228515625" z="0.275390625" a="59.23755645751953" y="-0.7224740982055664"/>
		    <child type="Land_Roadblock_WoodenCrate" deloot="1" lootmax="3" lootmin="1" x="7.0361328125" z="0.451171875" a="75.39347839355469" y="-0.6734199523925781"/>
		    <child type="Land_CDAJA2_HMMWVUA" deloot="1" lootmax="3" lootmin="1" x="4.103515625" z="7.24609375" a="201.74673461914062" y="0.872899055480957"/>
		    <child type="Land_Container_1Moh_DE" deloot="1" lootmax="3" lootmin="1" x="8.7646484375" z="-3.515625" a="219.6826934814453" y="0.23990440368652344"/>
		    <child type="Land_CDAJ_STT_Canted_DE" deloot="1" lootmax="3" lootmin="1" x="5.583984375" z="1.875" a="156.62030029296875" y="1.2573814392089844"/>
		    <child type="Land_CDAJ_Marker_MissionCommo" deloot="1" lootmax="3" lootmin="1" x="2.2724609375" z="-0.291015625" a="359.99949797656154" y="-1.1622543334960938"/>
		</group>*/
		
		
		return true;
	}
	
	protected override void Update(float dt)
	{
		SetCanExecute(GetEditor().GetSelectedObjects().Count() > 0);
	}
	
	override string GetName()
	{
		return "Copy as Event Group";
	}
}