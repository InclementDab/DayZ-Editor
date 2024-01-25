class EditorServer: EditorNode
{
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	static const int DEFAULT_ENTITY_COUNT = 512;
	
	void EditorServer(string uuid, string display_name, Symbols icon)
	{		
		// Load all default categories and placements
		EditorNode edited_objects = new EditorNode("EditedObjects", "Edited Objects", Symbols.OBJECT_GROUP);
		edited_objects.Add(new EditorNode("PlacedObjects", "Placed Objects", Symbols.HAND));
		edited_objects.Add(new EditorNode("BrushedObjects", "Brushed Objects",Symbols.BRUSH));
		edited_objects.Add(new EditorNode("HiddenObjects", "Hidden Objects", Symbols.HIPPO));
		Add(edited_objects);

		EditorNode placeable_objects = new EditorNode("PlaceableObjects", "Placeable Objects", Symbols.ADDRESS_BOOK);
		placeable_objects.Add(new EditorNode("Unknown", "Unknown", Symbols.CHESS_QUEEN));
		placeable_objects.Add(new EditorNode("Plants", "Plants", Symbols.TREE));
		placeable_objects.Add(new EditorNode("Rocks", "Rocks", Symbols.HILL_ROCKSLIDE));
		placeable_objects.Add(new EditorNode("Clutter", "Clutter", Symbols.TRASH));
		placeable_objects.Add(new EditorNode("Structures", "Structures", Symbols.HOUSE));
		placeable_objects.Add(new EditorNode("Wrecks", "Wrecks", Symbols.CAR_BURST));
		placeable_objects.Add(new EditorNode("AI", "AI", Symbols.PERSON));
		placeable_objects.Add(new EditorNode("Water", "Water", Symbols.WATER));
		placeable_objects.Add(new EditorNode("Vehicles", "Vehicles", Symbols.CAR));
		placeable_objects.Add(new EditorNode("StaticObjects", "Static Objects", Symbols.OBJECT_INTERSECT));
		placeable_objects.Add(new EditorNode("DynamicObjects", "Dynamic Objects", Symbols.SHIRT));
		placeable_objects.Add(new EditorNode("ScriptedObjects", "Scripted Objects", Symbols.CODE));
		Add(placeable_objects);
		
		EditorNode brushes = new EditorNode("Brushes", "Brushes", Symbols.BRUSH);
		brushes.Add(new BetulaPendula_Brush("BetulaPendula_Brush", "Betula Pendula", Symbols.TREES));
		brushes.Add(new LightningBrush("LightningBrush", "Lightning Brush", Symbols.BOLT));
		Add(brushes);
		
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
					
		string category = "Unknown";
		// handle config objects
		foreach (string path: config_paths) {
			for (int i = 0; i < GetGame().ConfigGetChildrenCount(path); i++) {
				string type;
		        GetGame().ConfigGetChildName(path, i, type);
				if (GetGame().ConfigGetInt(path + " " + type + " scope") < 2) {
					continue;
				}
				
				if (GetDayZGame().IsForbiddenItem(type)) {
					continue;
				}
				
				array<string> full_path = {};
				GetGame().ConfigGetFullPath(path + " " + type, full_path);
				
				category = "Unknown";
				if ((full_path.Find("Weapon_Base") != -1) || (full_path.Find("Inventory_Base")) != -1) {
					category = "DynamicObjects";
				} else if (full_path.Find("HouseNoDestruct") != -1) {
					category = "Structures";
				} else if (full_path.Find("Car") != -1) {
					category = "Vehicles";
				} else if (full_path.Find("Man") != -1 || (full_path.Find("DZ_LightAI"))) {
					category = "AI";
				}
				
				this["PlaceableObjects"][category].Add(new EditorPlaceable(type, type, Symbols.BUILDING));
		    }
		}
		
		array<string> paths = { "DZ\\plants", "DZ\\plants_bliss", "DZ\\rocks", "DZ\\rocks_bliss" };
		foreach (string model_path: paths) {
			array<ref CF_File> files = {};
			if (!CF_Directory.GetFiles(model_path + "\\*", files, FindFileFlags.ARCHIVES)) {
				continue;
			}
				
			foreach (CF_File file: files) {		
				if (!file || file.GetExtension() != ".p3d") {
					continue;
				}
				
				string model_name;
				array<string> items = {};
				string full_path_p3d = file.GetFullPath();
				full_path_p3d.Replace("/", "\\");
				full_path_p3d.Split("\\", items);
				if (items.Count() != 0) {
					model_name = items[items.Count() - 1];
				}
				
				category = "StaticObjects";
				if ((items.Find("tree") != -1) || (items.Find("bush") != -1)) {
					category = "Plants";
				} else if (items.Find("clutter") != -1) {
					category = "Clutter";
				} else if (items.Find("rocks") != -1) {
					category = "Rocks";
				}
			
				this["PlaceableObjects"][category].Add(new EditorPlaceable(file.GetFullPath(), model_name, Symbols.CIRCLE_C));
			}
		}

		foreach (Param3<typename, string, string> scripted_instance: RegisterEditorObject.Instances) {
			this["PlaceableObjects"]["ScriptedObjects"].Add(new EditorPlaceable(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}
	}
}