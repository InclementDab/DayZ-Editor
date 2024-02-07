class ConfigObjectEntry: ConfigObject
{
	[ConfigEntryAttribute("scope")]
	int Scope;
	
	[ConfigEntryAttribute("model")]
	string Model;
}

class Sandbox: TreeNode
{
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
	
	// structure of actual mission files
	static const string MISSION = "Mission";
	TreeNode GetMission()
	{
		return this[MISSION];
	}
	
	static const string EDITORS = "Editors";
	TreeNode GetEditors()
	{
		return this[EDITORS];
	}
		
	static const string COMMANDS = "Commands";
	TreeNode GetCommand(string uuid)
	{
		return this[COMMANDS][uuid];
	}
	
	static const string TOOLS = "Tools";
	TreeNode GetTool(string uuid)
	{
		return this[TOOLS][uuid];
	}
	
	static const string DZ = "DZ";
	TreeNode GetStaticObjects()
	{
		return this[DZ];
	}
	
	static const string VEHICLES = "Vehicles";
	TreeNode GetDynamicObjects()
	{
		return this[VEHICLES];
	}
	
	static const string WEAPONS = "Weapons";
	TreeNode GetWeapons()
	{
		return this[WEAPONS];
	}
	
	static const string SCRIPTED = "Scripted";
	TreeNode GetScripted()
	{
		return this[SCRIPTED];
	}
	
	void Sandbox(string uuid, string display_name, Symbols icon)
	{		
		Add(new FileNode(MISSION, "Mission", Symbols.FOLDER, "$mission:"));
		Add(new TreeNode(DZ, "DZ", Symbols.FOLDER));
		Add(new TreeNode(VEHICLES, "CfgVehicles", Symbols.FOLDER));
		Add(new TreeNode(WEAPONS, "CfgWeapons", Symbols.FOLDER));
		Add(new TreeNode(SCRIPTED, "Scripted Entities", Symbols.FOLDER));
		
		Add(new TreeNode(EDITORS, "Editors", Symbols.PEOPLE_SIMPLE));
		Add(new TreeNode(COMMANDS, "Commands", Symbols.COMMAND));
		Add(new TreeNode(TOOLS, "Tools", Symbols.TOOLBOX));
		
		this[COMMANDS].Add(new AfterlifeToggle("Afterlife", "View Hidden", Symbols.GHOST));
		this[COMMANDS].Add(new AddLayerCommand("AddLayer", "Add Layer", Symbols.LAYER_PLUS));
		this[COMMANDS].Add(new SetLayerActiveCommand("SetLayerActive", "Set Layer Active", string.Empty));
		this[COMMANDS].Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT));
		this[COMMANDS].Add(new PianoCommand("Piano", "Drop Piano", Symbols.PIANO));
		this[COMMANDS].Add(new BoxCommand("Box", "Box Selection", Symbols.SQUARE_DASHED));
		this[COMMANDS].Add(new CommandNode("Camera", "Camera", Symbols.CAMERA));
		this[COMMANDS].Add(new EllipseCommand("Ellipse", "Ellipse Selection", Symbols.CIRCLE_DASHED));
		this[COMMANDS].Add(new CommandNode("Copy", "Copy", Symbols.COPY));
		this[COMMANDS].Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS));
		this[COMMANDS].Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH));
		this[COMMANDS].Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE));
		this[COMMANDS].Add(new LassoCommand("Lasso", "Lasso Select", Symbols.LASSO));
		this[COMMANDS].Add(new CommandNode("Lock", "Lock", Symbols.LOCK));
		this[COMMANDS].Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET));
		this[COMMANDS].Add(new CommandNode("New", "New", Symbols.FILE));
		this[COMMANDS].Add(new CommandNode("Open", "Open", Symbols.FOLDER_OPEN));
		this[COMMANDS].Add(new CommandNode("Paste", "Paste", Symbols.PASTE));
		this[COMMANDS].Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT));
		this[COMMANDS].Add(new RenameCommand("Rename", "Rename", string.Empty));
		this[COMMANDS].Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN));
		this[COMMANDS].Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK));
		this[COMMANDS].Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK));
		this[COMMANDS].Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT));
		this[COMMANDS].Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN));
		this[COMMANDS].Add(new WeatherToggle("Weather", "Weather", Symbols.SUN));
		this[COMMANDS].Add(new CursorToggle("Cursor", "Toggle Cursor", Symbols.ARROW_POINTER));
		this[COMMANDS].Add(new HudToggle("Hud", "Toggle Hud", Symbols.EYE));
		this[COMMANDS].Add(new MapToggle("Map", "Toggle Map", Symbols.MAP));
		this[COMMANDS].Add(new ControlToggle("Control", "Toggle Control", Symbols.PERSON_PREGNANT));
		this[COMMANDS].Add(new CreateFolder("CreateFolder", "Create Folder", Symbols.FOLDER_PLUS));
		this[COMMANDS].Add(new CommandNode("Console", "Toggle Console", Brands.XBOX));
		
		this[TOOLS].Add(new TranslateTool("Translate", "Translation Mode", Symbols.UP_DOWN_LEFT_RIGHT));
		this[TOOLS].Add(new RotateTool("Rotate", "Rotation Mode", Symbols.ROTATE));
		this[TOOLS].Add(new ScaleTool("Scale", "Scale Mode", Symbols.ARROWS_MAXIMIZE));			
		
#ifndef WORKBENCH
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
					this[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE_XML, mission_file));
					break;
				}
				
				case ".json": {
					this[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE_CODE, mission_file));
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
					this[MISSION].Add(new FileNode(mission_file, mission_file, Symbols.FILE, mission_file));
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
				TreeNode current = this[DZ];
				array<string> p3d_split = {};
				p3d.Split(Directory.PATH_SEPERATOR, p3d_split);
				for (int k = 1; k < p3d_split.Count() - 1; k++) {
					//Print(p3d_split[k]);
					if (!current[p3d_split[k]]) {
						current[p3d_split[k]] = new TreeNode(p3d_split[k], p3d_split[k], Symbols.FOLDER);
					}
					
					//Print(string.Format("%1:%2", current.GetUUID(), p3d_split[k]));
					current = current[p3d_split[k]];
				}
				
				//PrintFormat("[%1] registering as %2", p3d, p3d_split[p3d_split.Count() - 1]);
				current[p3d_split[p3d_split.Count() - 1]] = new PlaceableNode(p3d, p3d_split[p3d_split.Count() - 1], Symbols.TRIANGLE);
			}
		}
		
		Print(string.Format("%1 nodes/second", (float)j / ((float)GetGame().GetTime() - t) * 1000.0 ));

		foreach (Param3<typename, string, string> scripted_instance: RegisterScriptedEntity.Instances) {
			this[SCRIPTED].Add(new PlaceableNode(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}	
#endif
	}
		
	static bool IsForbiddenItem(string model)
	{
		
		
		//! Everything is fine... I hope... :pain:
		return false;
	}
}