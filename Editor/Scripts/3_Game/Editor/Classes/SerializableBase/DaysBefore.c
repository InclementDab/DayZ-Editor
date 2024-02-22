class ConfigObjectEntry: ConfigObject
{
	[ConfigEntryAttribute("scope")]
	int Scope;
	
	[ConfigEntryAttribute("model")]
	string Model;
}

modded class DaysBefore
{
	static const string MISSION = "Mission";
	static const string EDITORS = "Editors";
	static const string COMMANDS = "Commands";
	static const string TOOLS = "Tools";
	static const string DZ = "DZ";
	static const string VEHICLES = "Vehicles";
	static const string WEAPONS = "Weapons";
	static const string SCRIPTED = "Scripted";

	static const ref array<string> FORBIDDEN_ITEMS = { 
		"TestObject",
		"ItemOptics", 
		"AKM_TESTBED", 
		"Red9", 
		"QuickieBow", 
		"LargeTentBackPack", 
		"SurvivorMale_Base", 
		"SurvivorFemale_Base", 
		"Land_VASICore", 
		"FlagCarrierCore"
	};
	
	void DaysBefore(string path)
	{
		Root.Add(new FileNode(MISSION, "Mission", Symbols.FOLDER, LinearColor.WHITE, "$mission:"));
		Root.Add(new NamedNode(DZ, "DZ", Symbols.FOLDER, LinearColor.WHITE));
		Root.Add(new NamedNode(VEHICLES, "CfgVehicles", Symbols.FOLDER, LinearColor.WHITE));
		Root.Add(new NamedNode(WEAPONS, "CfgWeapons", Symbols.FOLDER, LinearColor.WHITE));
		Root.Add(new NamedNode(SCRIPTED, "Scripted Entities", Symbols.FOLDER, LinearColor.WHITE));
		
		Root.Add(new NamedNode(EDITORS, "Editors", Symbols.PEOPLE_SIMPLE, LinearColor.WHITE));
		Root.Add(new NamedNode(COMMANDS, "Commands", Symbols.COMMAND, LinearColor.WHITE));
		Root.Add(new NamedNode(TOOLS, "Tools", Symbols.TOOLBOX, LinearColor.WHITE));
		
		Root[COMMANDS].Add(new AfterlifeToggle("Afterlife", "View Hidden", Symbols.GHOST, LinearColor.WHITE));
		Root[COMMANDS].Add(new AddLayerCommand("AddLayer", "Add Layer", Symbols.LAYER_PLUS, LinearColor.WHITE));
		Root[COMMANDS].Add(new SetLayerActiveCommand("SetLayerActive", "Set Layer Active", string.Empty, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT, LinearColor.WHITE));
		Root[COMMANDS].Add(new PianoCommand("Piano", "Drop Piano", Symbols.PIANO, LinearColor.WHITE));
		Root[COMMANDS].Add(new BoxCommand("Box", "Box Selection", Symbols.SQUARE_DASHED, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Camera", "Camera", Symbols.CAMERA, LinearColor.WHITE));
		Root[COMMANDS].Add(new EllipseCommand("Ellipse", "Ellipse Selection", Symbols.CIRCLE_DASHED, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Copy", "Copy", Symbols.COPY, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS, LinearColor.WHITE));
		Root[COMMANDS].Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE, LinearColor.WHITE));
		Root[COMMANDS].Add(new LassoCommand("Lasso", "Lasso Select", Symbols.LASSO, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Lock", "Lock", Symbols.LOCK, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("New", "New", Symbols.FILE, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Open", "Open", Symbols.FOLDER_OPEN, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Paste", "Paste", Symbols.PASTE, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT, LinearColor.WHITE));
		Root[COMMANDS].Add(new RenameCommand("Rename", "Rename", string.Empty, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN, LinearColor.WHITE));
		Root[COMMANDS].Add(new WeatherToggle("Weather", "Weather", Symbols.SUN, LinearColor.WHITE));
		Root[COMMANDS].Add(new CursorToggle("Cursor", "Toggle Cursor", Symbols.ARROW_POINTER, LinearColor.WHITE));
		Root[COMMANDS].Add(new HudToggle("Hud", "Toggle Hud", Symbols.EYE, LinearColor.WHITE));
		Root[COMMANDS].Add(new MapToggle("Map", "Toggle Map", Symbols.MAP, LinearColor.WHITE));
		Root[COMMANDS].Add(new ControlToggle("Control", "Toggle Control", Symbols.PERSON_PREGNANT, LinearColor.WHITE));
		Root[COMMANDS].Add(new CreateFolder("CreateFolder", "Create Folder", Symbols.FOLDER_PLUS, LinearColor.WHITE));
		Root[COMMANDS].Add(new CommandNode("Console", "Toggle Console", Brands.XBOX, LinearColor.WHITE));
		
		Root[TOOLS].Add(new TranslateTool("Translate", "Translation Mode", Symbols.UP_DOWN_LEFT_RIGHT, LinearColor.WHITE));
		Root[TOOLS].Add(new RotateTool("Rotate", "Rotation Mode", Symbols.ROTATE, LinearColor.WHITE));
		Root[TOOLS].Add(new ScaleTool("Scale", "Scale Mode", Symbols.ARROWS_MAXIMIZE, LinearColor.WHITE));
		
#ifndef COMPONENT_SYSTEM
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(CFG_VEHICLESPATH); i++) {
			string name;
	        GetGame().ConfigGetChildName(CFG_VEHICLESPATH, i, name);
			if (FORBIDDEN_ITEMS.Find(name) != -1) {
				continue;
			}
			
			ConfigObjectEntry entry = ConfigObjectEntry(string.Format("%1 %2", CFG_VEHICLESPATH, name));
			Print(entry.Scope);
			Print(entry.GetName());
			
			//array<string> full_path = {};
			//GetGame().ConfigGetFullPath(string.Format("%1 %2", CFG_VEHICLESPATH, name), full_path);
			//this[category].Add(new PlaceableNode(name, name, this[category].GetIcon()));
	    }
		
#ifdef SERVER
		array<string> mission_files = Directory.EnumerateFiles("$mission:");
		foreach (File mission_file: mission_files) {
			switch (mission_file.GetExtension()) {
				case ".xml": {
					Root[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE_XML, LinearColor.WHITE, mission_file));
					break;
				}
				
				case ".json": {
					Root[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE_CODE, LinearColor.WHITE, mission_file));
					break;
				}
				
				/*
				case string.Format(".%1", GetDayZGame().GetWorldNameEx(false)):
				case string.Empty: {
					array<string> files = Directory.EnumerateFiles(mission_file);
					foreach (string f: files) {
						string file_formatted = f;
						file_formatted.Replace(m_File, "");
						file_formatted.Replace("$mission:\\", "");
						file_formatted.Replace("\\", "");
						this[FILES].Add(new FileNode(file_formatted, file_formatted, Symbols.FILE, f));
					}
					
					break;
				}*/
				
				default: {
					Root[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE, LinearColor.WHITE, mission_file));
					break;
				}
			}
		}
#endif
		
		
		float t = GetGame().GetTime();
		for (int j = 0; j < 473; j++) {
			array<string> p3d_files = Directory.EnumerateFiles("DZ\\" + DayZGame.P3D_DIRECTORIES[j], "*.p3d");
			p3d_files.Debug();
			foreach (string p3d: p3d_files) {
				Node current = Root[DZ];
				array<string> p3d_split = {};
				p3d.Split(Directory.PATH_SEPERATOR, p3d_split);
				for (int k = 1; k < p3d_split.Count() - 1; k++) {
					//Print(p3d_split[k]);
					if (!current[p3d_split[k]]) {
						current[p3d_split[k]] = new NamedNode(p3d_split[k], p3d_split[k], Symbols.FOLDER, LinearColor.WHITE);
					}
					
					//Print(string.Format("%1:%2", current.GetUUID(), p3d_split[k]));
					current = current[p3d_split[k]];
				}
				
				//PrintFormat("[%1] registering as %2", p3d, p3d_split[p3d_split.Count() - 1]);
				current[p3d_split[p3d_split.Count() - 1]] = new PlaceableNode(p3d, p3d_split[p3d_split.Count() - 1], Symbols.TRIANGLE, LinearColor.WHITE);
			}
		}
		
		Print(string.Format("%1 nodes/second", (float)j / ((float)GetGame().GetTime() - t) * 1000.0 ));

		foreach (Param3<typename, string, string> scripted_instance: RegisterScriptedEntity.Instances) {
			Root[SCRIPTED].Add(new PlaceableNode(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3, LinearColor.WHITE));
		}	
#endif
	}
	
	// structure of actual mission files
	static Node GetMission()
	{
		return Root[MISSION];
	}
	
	static Node GetEditors()
	{
		return Root[EDITORS];
	}
	
	static EditorNode GetEditor()
	{
		return EditorNode.Cast(Root[EDITORS][GetGame().GetUserManager().GetTitleInitiator().GetUid()]);
	}
		
	static Node GetCommand(string uuid)
	{
		return Root[COMMANDS][uuid];
	}
	
	static Node GetTool(string uuid)
	{
		return Root[TOOLS][uuid];
	}
	
	static Node GetStaticObjects()
	{
		return Root[DZ];
	}
	
	static Node GetDynamicObjects()
	{
		return Root[VEHICLES];
	}
	
	static Node GetWeapons()
	{
		return Root[WEAPONS];
	}
	
	static Node GetScripted()
	{
		return Root[SCRIPTED];
	}
}