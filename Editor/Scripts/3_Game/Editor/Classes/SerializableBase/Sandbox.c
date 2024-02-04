class Sandbox: TreeNode
{
	// structure of actual mission files
	static const string FILES = "Files";
	static const string EDITORS = "Editors";
	static const string COMMANDS = "Commands";
	static const string TOOLS = "Tools";
	
	static const string STRUCTURES = "Structures";	
	static const string WEAPONS = "Weapons";
	static const string MAN = "Man";
	static const string AI = "AI";
	static const string VEHICLES = "Vehicles";
	static const string ITEMS = "Items";
	static const string SCRIPTED = "Scripted";
	
	void Sandbox(string uuid, string display_name, Symbols icon)
	{		
		Add(new FileNode(FILES, "Files", Symbols.FOLDER_USER, uuid));
		Add(new TreeNode(EDITORS, "Editors", Symbols.PEOPLE_SIMPLE));
		Add(new TreeNode(COMMANDS, "Commands", Symbols.COMMAND));
		Add(new TreeNode(TOOLS, "Tools", Symbols.TOOLBOX));
		Add(new TreeNode(SCRIPTED, "Scripted Objects", Symbols.CODE));
		
		Add(new TreeNode(WEAPONS, "Weapons", Symbols.GUN));
		Add(new TreeNode(STRUCTURES, "Structures", Symbols.HOUSE));
		Add(new TreeNode(MAN, "People", Symbols.PERSON));
		Add(new TreeNode(AI, "AI", Symbols.COW));
		Add(new TreeNode(VEHICLES, "Vehicles", Symbols.CAR));
		Add(new TreeNode(ITEMS, "Items", Symbols.COUCH));
				
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
		this[COMMANDS].Add(new CommandNode("Console", "Toggle Console", Symbols.XBOX));
		
		this[TOOLS].Add(new TranslateTool("Translate", "Translation Mode", Symbols.UP_DOWN_LEFT_RIGHT));
		this[TOOLS].Add(new RotateTool("Rotate", "Rotation Mode", Symbols.ROTATE));
		this[TOOLS].Add(new ScaleTool("Scale", "Scale Mode", Symbols.ARROWS_MAXIMIZE));	
		
		float t = GetGame().GetTime();
		for (int j = 0; j < 50; j++) {
			array<string> p3d_files = Directory.EnumerateFiles("DZ\\" + DayZGame.P3D_DIRECTORIES[j], "*.p3d");
			foreach (string p3d: p3d_files) {
				TreeNode current = this;
				array<string> p3d_split = {};
				p3d.Split(Directory.PATH_SEPERATOR, p3d_split);
				for (int k = 0; k < p3d_split.Count() - 1; k++) {
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
		
		// handle config objects
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
		foreach (string path: config_paths) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type;
		        GetGame().ConfigGetChildName(path, i, type);
				if (GetGame().ConfigGetInt(string.Format("%1 %2 scope", path, type)) < 2 || GetDayZGame().IsForbiddenItem(type)) {
					continue;
				}
				
				array<string> full_path = {};
				GetGame().ConfigGetFullPath(string.Format("%1 %2", path, type), full_path);
				
				string category = "idk bro u tell me";
				if (full_path.Find("Weapon_Base") != -1) {
					category = WEAPONS;
				} else if (full_path.Find("HouseNoDestruct") != -1) {
					category = STRUCTURES;
				} else if (full_path.Find("Car") != -1) {
					category = VEHICLES;
				} else if (full_path.Find("Man") != -1) {
					category = MAN;
				} else if (full_path.Find("DZ_LightAI")) {
					category = AI; 
				} else if ((full_path.Find("Inventory_Base")) != -1) {
					category = ITEMS;
				} else PrintFormat("Couldnt find a home for %1, %2", type, path);
				
				this[category].Add(new PlaceableNode(type, type, this[category].GetIcon()));
		    }
		}	
		
		foreach (Param3<typename, string, string> scripted_instance: RegisterScriptedEntity.Instances) {
			this[SCRIPTED].Add(new PlaceableNode(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}	
	}
	
	TreeNode GetEditors()
	{
		return this[EDITORS];
	}
	
	TreeNode GetCommand(string uuid)
	{
		return this[COMMANDS][uuid];
	}
}