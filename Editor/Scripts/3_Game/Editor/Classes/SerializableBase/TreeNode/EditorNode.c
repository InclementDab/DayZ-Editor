/*
                    Euler sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/

class EditorColors
{
	private void EditorColors();
	private void ~EditorColors();
	
	static const int SELECT = ARGB(255, 7, 111, 146);
	static const int BACKGROUND_1 = ARGB(255, 30, 30, 25);
	static const int HIDDEN_1 = ARGB(255, 131, 131, 149);
}

class EditorNode: TreeNode
{
	static const string P3D_DIRECTORIES[473] = {"data","rocks","rocks_bliss","animals\\bos_taurus","animals\\bos_taurus_fem","animals\\canis_lupus","animals\\capra_hircus","animals\\capra_hircus_fem","animals\\capreolus_capreolus","animals\\capreolus_capreolus_fem","animals\\cervus_elaphus","animals\\cervus_elaphus_fem","animals\\gallus_gallus_domesticus","animals\\gallus_gallus_domesticus_feminam","animals\\ovis_aries","animals\\ovis_aries_fem","animals\\sus_domesticus","animals\\sus_scrofa","animals_bliss\\ursus_arctos","characters\\backpacks","characters\\belts","characters\\bodies","characters\\glasses","characters\\gloves","characters\\headgear","characters\\heads","characters\\masks","characters\\pants","characters\\proxies","characters\\shoes","characters\\tops","characters\\vests","characters\\zombies","data\\data","data\\proxies","gear\\books","gear\\camping","gear\\consumables","gear\\containers","gear\\cooking","gear\\crafting","gear\\cultivation","gear\\drinks","gear\\food","gear\\medical","gear\\navigation","gear\\optics","gear\\radio","gear\\tools","gear\\traps","plants\\bush","plants\\clutter","plants\\tree","plants_bliss\\building","plants_bliss\\Bush","plants_bliss\\Clutter","plants_bliss\\Tree","structures\\ruins","structures\\walls","structures_bliss\\ruins","structures_bliss\\walls","vehicles\\parts","water\\ponds","water\\Streambed","water\\Streams","water_bliss\\ponds","water_bliss\\river","weapons\\ammunition","weapons\\explosives","weapons\\projectiles","weapons\\proxies","animals\\Data\\Geom","characters\\belts\\proxies","characters\\headgear\\proxies","characters\\heads\\anim","characters\\masks\\proxy","characters\\vests\\proxy","data\\data\\Destruct","data\\data\\penetration","gear\\camping\\proxies","gear\\cooking\\proxy","gear\\traps\\proxy","plants\\tree\\cutted_cut","plants\\tree\\cutted_stump","structures\\furniture\\Bathroom","structures\\furniture\\Beds","structures\\furniture\\Cases","structures\\furniture\\folding_beds","structures\\furniture\\generalstore","structures\\furniture\\Hospital_transport_bed","structures\\furniture\\Prison","structures\\furniture\\radar_equipment","structures\\furniture\\School_equipment","structures\\furniture\\Various","structures\\industrial\\CementWorks","structures\\industrial\\CoalPlant","structures\\industrial\\Construction","structures\\industrial\\Containers","structures\\industrial\\Dams","structures\\industrial\\Farms","structures\\industrial\\Garages","structures\\industrial\\Harbour","structures\\industrial\\Houses","structures\\industrial\\Mines","structures\\industrial\\Misc","structures\\industrial\\Pipes","structures\\industrial\\Power","structures\\industrial\\Quarry","structures\\industrial\\Sawmills","structures\\industrial\\Sheds","structures\\industrial\\Smokestacks","structures\\industrial\\Tanks","structures\\military\\airfield","structures\\military\\houses","structures\\military\\improvised","structures\\military\\Misc","structures\\military\\tisy","structures\\Proxy_BuildingParts\\AirCondition","structures\\Proxy_BuildingParts\\Barricading","structures\\Proxy_BuildingParts\\GasMeterExt","structures\\Proxy_BuildingParts\\House","structures\\Proxy_BuildingParts\\ladders","structures\\rail\\Bridges","structures\\rail\\Houses","structures\\rail\\Misc","structures\\rail\\Tracks","structures\\residential\\Bus","structures\\residential\\firedep","structures\\residential\\HouseBlocks","structures\\residential\\houses","structures\\residential\\medical","structures\\residential\\misc","structures\\residential\\Offices","structures\\residential\\Platforms","structures\\residential\\Police","structures\\residential\\Schools","structures\\residential\\sheds","structures\\residential\\stores","structures\\residential\\Tenements","structures\\roads\\Bridges","structures\\roads\\Decals","structures\\roads\\Panels","structures\\roads\\Parts","structures\\roads\\Sidewalks","structures\\roads\\Tunnels","structures\\signs\\billboards","structures\\signs\\Directional","structures\\signs\\Misc","structures\\signs\\Settlements","structures\\signs\\tourist","structures\\signs\\Traffic","structures\\specific\\Airfields","structures\\specific\\cableway","structures\\specific\\Camps","structures\\specific\\Castles","structures\\specific\\Cemeteries","structures\\specific\\churches","structures\\specific\\DeadBodies","structures\\specific\\FuelStation","structures\\specific\\Hotel","structures\\specific\\Lunapark","structures\\specific\\prison","structures\\specific\\Radio","structures\\specific\\Skilift","structures\\specific\\Statues","structures\\specific\\towers","structures\\wrecks\\Aircraft","structures\\wrecks\\ships","structures\\wrecks\\Trains","structures\\wrecks\\Vehicles","structures_bliss\\industrial\\Construction","structures_bliss\\industrial\\DieselPowerPlant","structures_bliss\\industrial\\Farms","structures_bliss\\industrial\\Garages","structures_bliss\\industrial\\Harbour","structures_bliss\\industrial\\Houses","structures_bliss\\industrial\\Misc","structures_bliss\\industrial\\Pipes","structures_bliss\\industrial\\Power","structures_bliss\\industrial\\Sheds","structures_bliss\\industrial\\Smokestacks","structures_bliss\\industrial\\WindPowerPlant","structures_bliss\\military\\Bunkers","structures_bliss\\military\\houses","structures_bliss\\military\\Misc","structures_bliss\\military\\training","structures_bliss\\rail\\Misc","structures_bliss\\rail\\Tracks","structures_bliss\\residential\\Bus","structures_bliss\\residential\\Houses","structures_bliss\\residential\\Medical","structures_bliss\\residential\\misc","structures_bliss\\residential\\Police","structures_bliss\\residential\\Slums","structures_bliss\\residential\\Stores","structures_bliss\\roads\\Bridges","structures_bliss\\roads\\Decals","structures_bliss\\roads\\Misc","structures_bliss\\roads\\Panels","structures_bliss\\roads\\Parts","structures_bliss\\roads\\Sidewalks","structures_bliss\\roads\\Sinkhole","structures_bliss\\ruins\\proxy","structures_bliss\\signs\\Directional","structures_bliss\\signs\\Misc","structures_bliss\\signs\\Settlements","structures_bliss\\signs\\tourist","structures_bliss\\specific\\Airfields","structures_bliss\\specific\\Castles","structures_bliss\\specific\\cultural","structures_bliss\\specific\\forest","structures_bliss\\specific\\FuelStation","structures_bliss\\specific\\Towers","structures_bliss\\specific\\Tribune","structures_bliss\\underground\\Corridor","structures_bliss\\underground\\entrance","structures_bliss\\underground\\Floor","structures_bliss\\underground\\Stairs","structures_bliss\\underground\\storage","structures_bliss\\underground\\Terrain","structures_bliss\\underground\\Tunnel","structures_bliss\\underground\\Water","structures_bliss\\wrecks\\Ships","structures_bliss\\wrecks\\Vehicles","surfaces\\data\\roadway","vehicles\\wheeled\\civiliansedan","vehicles\\wheeled\\hatchback_02","vehicles\\wheeled\\offroadhatchback","vehicles\\wheeled\\offroad_02","vehicles\\wheeled\\proxies","vehicles\\wheeled\\sedan_02","vehicles\\wheeled\\transitbus","vehicles\\wheeled\\truck_01","vehicles\\wheeled\\truck_02","vehicles\\wheeled\\van_01","water\\ponds\\moss","water\\Streambed\\misc","water\\Streambed\\Proxy","weapons\\archery\\bow_pvc","weapons\\archery\\bow_quickie","weapons\\archery\\bow_recurve","weapons\\archery\\crossbow","weapons\\attachments\\light","weapons\\attachments\\magazine","weapons\\attachments\\muzzle","weapons\\attachments\\optics","weapons\\attachments\\support","weapons\\attachments\\underslung","weapons\\firearms\\ak101","weapons\\firearms\\ak74","weapons\\firearms\\AKM","weapons\\firearms\\aug","weapons\\firearms\\B95","weapons\\firearms\\cz527","weapons\\firearms\\cz550","weapons\\firearms\\cz61","weapons\\firearms\\fal","weapons\\firearms\\famas","weapons\\firearms\\Izh18","weapons\\firearms\\M14","weapons\\firearms\\m16a2","weapons\\firearms\\m249","weapons\\firearms\\M4","weapons\\firearms\\mini_uzi","weapons\\firearms\\mosin9130","weapons\\firearms\\MP5","weapons\\firearms\\ots14","weapons\\firearms\\pkm","weapons\\firearms\\pm73rak","weapons\\firearms\\pp19","weapons\\firearms\\repeater","weapons\\firearms\\Ruger1022","weapons\\firearms\\scout","weapons\\firearms\\SKS","weapons\\firearms\\speargun","weapons\\firearms\\ssg82","weapons\\firearms\\svd","weapons\\firearms\\Trumpet","weapons\\firearms\\UMP45","weapons\\firearms\\vss","weapons\\firearms\\winchester70","weapons\\launchers\\gp25","weapons\\launchers\\law","weapons\\launchers\\m203","weapons\\launchers\\m79","weapons\\launchers\\rpg7","weapons\\melee\\blade","weapons\\melee\\blunt","weapons\\melee\\powered","weapons\\nonlethal\\dartgun","weapons\\nonlethal\\shockpistol","weapons\\pistols\\1911","weapons\\pistols\\CZ75","weapons\\pistols\\de","weapons\\pistols\\derringer","weapons\\pistols\\flaregun","weapons\\pistols\\fnx45","weapons\\pistols\\glock","weapons\\pistols\\LongHorn","weapons\\pistols\\magnum","weapons\\pistols\\mkii","weapons\\pistols\\p1","weapons\\pistols\\pb6p9","weapons\\pistols\\pmm","weapons\\pistols\\Red9","weapons\\shotguns\\Izh18Shotgun","weapons\\shotguns\\Izh43","weapons\\shotguns\\Mp133","weapons\\shotguns\\Remington_r12","weapons\\shotguns\\saiga","data\\data\\ParticleEffects\\CraterLong","data\\data\\ParticleEffects\\Excretion","data\\data\\ParticleEffects\\Hit_Leaves","data\\data\\ParticleEffects\\News","data\\data\\ParticleEffects\\Pstone","data\\data\\ParticleEffects\\Shard","data\\data\\ParticleEffects\\Universal","data\\data\\ParticleEffects\\WallPart","structures\\furniture\\Bathroom\\Basin_A","structures\\furniture\\Bathroom\\bath","structures\\furniture\\Bathroom\\SINK","structures\\furniture\\Bathroom\\toilet_b_02","structures\\furniture\\Cases\\almara","structures\\furniture\\Cases\\case_a","structures\\furniture\\Cases\\case_bedroom_a","structures\\furniture\\Cases\\case_bedroom_b","structures\\furniture\\Cases\\case_cans_b","structures\\furniture\\Cases\\case_d","structures\\furniture\\Cases\\Dhangar_borwnskrin","structures\\furniture\\Cases\\Dhangar_borwnskrin_old","structures\\furniture\\Cases\\library_a","structures\\furniture\\Cases\\lobby_case","structures\\furniture\\Cases\\locker","structures\\furniture\\Cases\\metalcase","structures\\furniture\\Cases\\metalcrate","structures\\furniture\\Cases\\metalcrate_02","structures\\furniture\\Cases\\sarcophagus","structures\\furniture\\Cases\\Skrin_bar","structures\\furniture\\Chairs\\Church_chair","structures\\furniture\\Chairs\\ch_mod_c","structures\\furniture\\Chairs\\ch_mod_h","structures\\furniture\\Chairs\\ch_office_B","structures\\furniture\\Chairs\\hospital_bench","structures\\furniture\\Chairs\\kitchen_chair_a","structures\\furniture\\Chairs\\lobby_chair","structures\\furniture\\Chairs\\office_chair","structures\\furniture\\Chairs\\postel_manz_kov","structures\\furniture\\Chairs\\postel_panelak1","structures\\furniture\\Chairs\\SofaCorner","structures\\furniture\\Chairs\\Sofa_leather","structures\\furniture\\Chairs\\vojenska_palanda","structures\\furniture\\decoration\\box_c","structures\\furniture\\decoration\\bucket","structures\\furniture\\decoration\\carpet_big","structures\\furniture\\decoration\\Flowers","structures\\furniture\\decoration\\lekarnicka","structures\\furniture\\decoration\\piano","structures\\furniture\\Eletrical_appliances\\pc","structures\\furniture\\Eletrical_appliances\\phonebox","structures\\furniture\\Eletrical_appliances\\pokladna","structures\\furniture\\Eletrical_appliances\\radio_b","structures\\furniture\\Eletrical_appliances\\tv_a","structures\\furniture\\Eletrical_appliances\\vending_machine","structures\\furniture\\Eletrical_appliances\\washing_machine","structures\\furniture\\kitchen\\case_sink_a","structures\\furniture\\kitchen\\dkamna_uhli","structures\\furniture\\kitchen\\Fireplace_grill","structures\\furniture\\kitchen\\fridge","structures\\furniture\\kitchen\\KitchenDesk","structures\\furniture\\kitchen\\Kitchenstove_Elec","structures\\furniture\\Lighting\\fluor_lamp","structures\\furniture\\Lighting\\hangar_lamp","structures\\furniture\\Lighting\\Light_BathRoom","structures\\furniture\\Lighting\\light_kitchen_03","structures\\furniture\\Lighting\\luxury_lamp","structures\\furniture\\Lighting\\Luxury_light","structures\\furniture\\medical\\medical_table","structures\\furniture\\medical\\patient_monitor","structures\\furniture\\Tables\\conference_table_a","structures\\furniture\\Tables\\kitchen_table_a","structures\\furniture\\Tables\\lobby_counter","structures\\furniture\\Tables\\lobby_table","structures\\furniture\\Tables\\office_desk","structures\\furniture\\Tables\\office_table_a","structures\\furniture\\Tables\\stul_kuch1","structures\\furniture\\Tables\\table_drawer","structures\\furniture\\Tables\\table_umakart","structures\\industrial\\CementWorks\\Proxy","structures\\industrial\\CoalPlant\\Proxy","structures\\industrial\\Construction\\Proxy","structures\\industrial\\Farms\\Proxy","structures\\industrial\\Houses\\Proxy","structures\\industrial\\Mines\\Proxy","structures\\industrial\\Misc\\Proxy","structures\\industrial\\Sawmills\\Proxy","structures\\industrial\\Sheds\\Proxy","structures\\military\\airfield\\Proxy","structures\\military\\houses\\Proxy","structures\\military\\improvised\\Proxy","structures\\military\\tisy\\Proxy","structures\\Proxy_BuildingParts\\LightningCond\\LightningConductorBottom","structures\\Proxy_BuildingParts\\LightningCond\\LightningConductorConnection","structures\\Proxy_BuildingParts\\LightningCond\\LightningConductorCorner","structures\\Proxy_BuildingParts\\LightningCond\\LightningConductorMiddle","structures\\Proxy_BuildingParts\\LightningCond\\LightningConductorTop","structures\\Proxy_BuildingParts\\Roof\\AirCondition","structures\\Proxy_BuildingParts\\Roof\\AntennaBigRoof","structures\\Proxy_BuildingParts\\Roof\\AntennaSmallRoof","structures\\residential\\firedep\\Proxy","structures\\residential\\houses\\Proxy","structures\\residential\\medical\\Proxy","structures\\residential\\Offices\\Proxy","structures\\residential\\Schools\\Proxy","structures\\residential\\Tenements\\Proxy","structures\\roads\\Panels\\Proxy","structures\\specific\\Airfields\\Proxy","structures\\specific\\churches\\Proxy","structures\\specific\\Hotel\\Proxy","structures\\specific\\Lunapark\\Proxy","structures\\specific\\prison\\Proxy","structures\\specific\\Radio\\Proxy","structures\\specific\\Statues\\Proxy","structures\\wrecks\\ships\\Proxy","structures\\wrecks\\Vehicles\\Proxy","structures_bliss\\industrial\\DieselPowerPlant\\Proxy","structures_bliss\\industrial\\Power\\Proxy","structures_bliss\\military\\houses\\Proxy","structures_bliss\\residential\\Houses\\Proxy","structures_bliss\\specific\\Castles\\Proxy","structures_bliss\\specific\\cultural\\Proxy","structures_bliss\\specific\\Towers\\Proxy","structures_bliss\\underground\\Corridor\\Proxy","structures_bliss\\underground\\entrance\\proxy","structures_bliss\\underground\\Floor\\Proxy","structures_bliss\\underground\\storage\\proxy","structures_bliss\\underground\\Water\\Proxy","structures_bliss\\wrecks\\Vehicles\\proxies","vehicles\\wheeled\\civiliansedan\\proxy","vehicles\\wheeled\\civiliansedan\\wreck","vehicles\\wheeled\\data\\damagewheel","vehicles\\wheeled\\hatchback_02\\Proxy","vehicles\\wheeled\\offroadhatchback\\proxy","vehicles\\wheeled\\offroad_02\\proxy","vehicles\\wheeled\\sedan_02\\proxy","vehicles\\wheeled\\transitbus\\proxy","vehicles\\wheeled\\transitbus\\proxy_big","vehicles\\wheeled\\truck_01\\proxy","vehicles\\wheeled\\truck_02\\proxy","vehicles\\wheeled\\van_01\\proxy","weapons\\firearms\\aug\\proxy","weapons\\firearms\\ssg82\\proxy","weapons\\launchers\\m79\\proxy","weapons\\pistols\\magnum\\proxy","structures\\furniture\\decoration\\WallBoards\\picture_a","structures\\furniture\\decoration\\WallBoards\\picture_a_02","structures\\furniture\\decoration\\WallBoards\\picture_a_03","structures\\furniture\\decoration\\WallBoards\\picture_a_04","structures\\furniture\\decoration\\WallBoards\\picture_a_05","structures\\furniture\\decoration\\WallBoards\\picture_b","structures\\furniture\\decoration\\WallBoards\\picture_b_02","structures\\furniture\\decoration\\WallBoards\\picture_c","structures\\furniture\\decoration\\WallBoards\\picture_c_02","structures\\furniture\\decoration\\WallBoards\\picture_d","structures\\furniture\\decoration\\WallBoards\\picture_d_02","structures\\furniture\\decoration\\WallBoards\\picture_e","structures\\furniture\\decoration\\WallBoards\\picture_e_02","structures\\furniture\\decoration\\WallBoards\\picture_f","structures\\furniture\\decoration\\WallBoards\\picture_f_02","structures\\furniture\\decoration\\WallBoards\\picture_g","structures\\furniture\\decoration\\WallBoards\\picture_g_02","structures\\furniture\\decoration\\WallBoards\\wall_board","structures\\furniture\\decoration\\WallBoards\\wall_board_02","structures\\furniture\\decoration\\WallBoards\\wall_board_03"};
	
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	static const int DEFAULT_ENTITY_COUNT = 512;
	
	protected PlayerIdentity m_Identity;
	protected DayZPlayer m_Player;
	
	protected vector m_CursorNormal = vector.Aside;
	protected EditorCamera m_Camera;
	protected ref EditorHud	m_Hud;

	protected ref array<TreeNode> m_SelectedNodes = {};
		
	protected ref map<string, TreeNode> m_CommandShortcutMap = new map<string, TreeNode>();
		
	static const string EDITS = "Layers";
		static const string BRUSHED = "Brushed";
		static const string HIDDEN = "Hidden";
	
	static const string COMMANDS = "Commands";
		static const string TOOLS = "Tools";
	
	static const string MENUS = "Menus";
	static const string HISTORY = "UndoRedo";
	static const string PLACEABLES = "Placeables";
	static const string DZ = "DZ";
	static const string BRUSHES = "Brushes";
	static const string PLACING = "Placing";
	static const string RECYCLE = "Recycle";
	
	TAnimGraphVariable Speed;
	
	void EditorNode(string uuid, string display_name, Symbols icon, PlayerIdentity identity, DayZPlayer player) 
	{
		m_Identity = identity;
		m_Player = player;	
		
		// Load all default categories and placements
		TreeNode edited_objects = new IndestructableFolderNode(EDITS, "Layers", Symbols.LAYER_GROUP);
		edited_objects.Add(new IndestructableFolderNode(BRUSHED, "Brushed", Symbols.PAINTBRUSH));
		edited_objects.Add(new IndestructableFolderNode(HIDDEN, "Hidden", Symbols.PAINTBRUSH));
		
		// default layer for now
		edited_objects.AddState(TreeNodeState.ACTIVE);
		Add(edited_objects);
						
		TreeNode commands = new TreeNode(COMMANDS, "Commands", Symbols.COMMAND);		
		commands.Add(new AfterlifeToggle("Afterlife", "View Hidden", Symbols.GHOST));
		commands.Add(new AddLayerCommand("AddLayer", "Add Layer", Symbols.LAYER_PLUS));
		commands.Add(new SetLayerActiveCommand("SetLayerActive", "Set Layer Active", string.Empty));
		commands.Add(new CommandNode("Bolt", "Lightning Bolt", Symbols.BOLT));
		commands.Add(new PianoCommand("Piano", "Drop Piano", Symbols.PIANO));
		commands.Add(new BoxCommand("Box", "Box Selection", Symbols.SQUARE_DASHED));
		commands.Add(new CommandNode("Camera", "Camera", Symbols.CAMERA));
		commands.Add(new EllipseCommand("Ellipse", "Ellipse Selection", Symbols.CIRCLE_DASHED));
		commands.Add(new CommandNode("Copy", "Copy", Symbols.COPY));
		commands.Add(new CommandNode("Cut", "Cut", Symbols.SCISSORS));
		commands.Add(new DeleteCommand("Delete", "Delete", Symbols.TRASH));
		commands.Add(new CommandNode("Ground", "Ground Mode", Symbols.IMAGE_LANDSCAPE));
		commands.Add(new LassoCommand("Lasso", "Lasso Select", Symbols.LASSO));
		commands.Add(new CommandNode("Lock", "Lock", Symbols.LOCK));
		commands.Add(new CommandNode("Magnet", "Magnet", Symbols.MAGNET));
		commands.Add(new CommandNode("New", "New", Symbols.FILE));
		commands.Add(new CommandNode("Open", "Open", Symbols.FOLDER_OPEN));
		commands.Add(new CommandNode("Paste", "Paste", Symbols.PASTE));
		commands.Add(new CommandNode("Redo", "Redo", Symbols.ROTATE_RIGHT));
		commands.Add(new CommandNode("SaveAs", "Save As", Symbols.FLOPPY_DISK_PEN));
		commands.Add(new CommandNode("Save", "Save", Symbols.FLOPPY_DISK));
		commands.Add(new CommandNode("Snap", "Snapping Mode", Symbols.THUMBTACK));
		commands.Add(new CommandNode("Undo", "Undo", Symbols.ROTATE_LEFT));
		commands.Add(new CommandNode("Unlock", "Unlock", Symbols.LOCK_OPEN));
		commands.Add(new WeatherToggle("Weather", "Weather", Symbols.SUN));
		commands.Add(new CursorToggle("Cursor", "Toggle Cursor", Symbols.ARROW_POINTER));
		commands.Add(new HudToggle("Hud", "Toggle Hud", Symbols.EYE));
		commands.Add(new ControlToggle("Control", "Toggle Control", Symbols.PERSON_PREGNANT));
		commands.Add(new CreateFolder("CreateFolder", "Create Folder", Symbols.FOLDER_PLUS));
		
		TreeNode tools = new TreeNode(TOOLS, "Tools", Symbols.TOOLBOX);
		tools.Add(new TranslateTool("Translate", "Translation Mode", Symbols.UP_DOWN_LEFT_RIGHT));
		tools.Add(new RotateTool("Rotate", "Rotation Mode", Symbols.ROTATE));
		tools.Add(new ScaleTool("Scale", "Scale Mode", Symbols.ARROWS_MAXIMIZE));
		commands.Add(tools);
		
		Add(commands);
		
		TreeNode menus = new TreeNode(MENUS, "Menus", Symbols.SQUARE_LIST);
		CommandNode file_menu = new CommandNode("File", "File", Symbols.FILE_SPREADSHEET);
		file_menu.Add(commands["New"]);
		file_menu.Add(commands["Open"]);
		file_menu.Add(commands["Save"]);
		file_menu.Add(commands["SaveAs"]);
		menus.Add(file_menu);
		
		CommandNode edit_menu = new CommandNode("Edit", "Edit", Symbols.FILE_PEN);
		menus.Add(edit_menu);
		
		CommandNode view_menu = new CommandNode("View", "View", Symbols.BINOCULARS);
		menus.Add(view_menu);
		
		CommandNode mission_menu = new CommandNode("Mission", "Mission", Symbols.PERSON_RIFLE);
		menus.Add(mission_menu);
		
		Add(menus);
				
		TreeNode undo_redo = new TreeNode(HISTORY, "History", Symbols.CLOCK_ROTATE_LEFT);
		Add(undo_redo);
		
		TreeNode placeable_objects = new TreeNode(PLACEABLES, "Placeable Objects", Symbols.ADDRESS_BOOK);
		placeable_objects.Add(new TreeNode("Unknown", "Unknown", Symbols.CHESS_QUEEN));
		placeable_objects.Add(new TreeNode("Plants", "Plants", Symbols.TREE));
		placeable_objects.Add(new TreeNode("Rocks", "Rocks", Symbols.HILL_ROCKSLIDE));
		placeable_objects.Add(new TreeNode("Clutter", "Clutter", Symbols.TRASH));
		placeable_objects.Add(new TreeNode("Structures", "Structures", Symbols.HOUSE));
		placeable_objects.Add(new TreeNode("Wrecks", "Wrecks", Symbols.CAR_BURST));
		placeable_objects.Add(new TreeNode("AI", "AI", Symbols.PERSON));
		placeable_objects.Add(new TreeNode("Water", "Water", Symbols.WATER));
		placeable_objects.Add(new TreeNode("Vehicles", "Vehicles", Symbols.CAR));
		placeable_objects.Add(new TreeNode("StaticObjects", "Static Objects", Symbols.OBJECT_INTERSECT));
		placeable_objects.Add(new TreeNode("DynamicObjects", "Dynamic Objects", Symbols.SHIRT));
		placeable_objects.Add(new TreeNode("ScriptedObjects", "Scripted Objects", Symbols.CODE));
		Add(placeable_objects);
		
		TreeNode brushes = new TreeNode(BRUSHES, "Brushes", Symbols.BRUSH);
		brushes.Add(new BetulaPendula_Brush("BetulaPendula_Brush", "Betula Pendula", Symbols.TREES));
		brushes.Add(new LightningBrush("LightningBrush", "Lightning Brush", Symbols.BOLT));
		brushes.Add(commands["Piano"]);
		Add(brushes);
		
		Add(new TreeNode(PLACING, "Placing", Symbols.FIREPLACE));
		Add(new TreeNode(RECYCLE, "Recycle Bin", Symbols.BIN_RECYCLE));
		
		array<string> config_paths = { CFG_VEHICLESPATH, CFG_WEAPONSPATH };
		string category = "Unknown";
		// handle config objects
#ifndef WORKBENCH
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
				
				this[PLACEABLES][category].Add(new PlaceableNode(type, type, this[PLACEABLES][category].GetIcon()));
		    }
		}
		
		Add(new TreeNode(DZ, "DZ", Symbols.FOLDER));
		for (int j = 0; j < 473; j++) {
			array<string> p3d_files = Directory.EnumerateFiles("DZ\\" + P3D_DIRECTORIES[j], "*.p3d");
			foreach (string p3d: p3d_files) {
				TreeNode current = this;
				array<string> p3d_split = {};
				p3d.Split(Directory.PATH_SEPERATOR, p3d_split);
				for (int k = 0; k < p3d_split.Count(); k++) {
					if (k != p3d_split.Count() - 1) {
						if (!current[p3d_split[k]]) {
							current[p3d_split[k]] = new TreeNode(p3d_split[k], p3d_split[k], Symbols.FOLDER);
						}
						
						current = current[p3d_split[k]];
					} else {
						current[p3d_split[k]] = new PlaceableNode(p3d, p3d_split[k], Symbols.TRIANGLE);
					}
				}
			}
		}
		
		foreach (Param3<typename, string, string> scripted_instance: RegisterScriptedEntity.Instances) {
			this[PLACEABLES]["ScriptedObjects"].Add(new PlaceableNode(scripted_instance.param1.ToString(), scripted_instance.param2, scripted_instance.param3));
		}		
#endif
	}

	void ~EditorNode() 
	{
		if (GetGame() && GetGame().GetUpdateQueue(CALL_CATEGORY_GUI)) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		}
		
		delete m_Hud;
		GetGame().ObjectDelete(m_Camera);
	}
			
	override void OnSynchronized()
	{
		if (GetGame().IsDedicatedServer()) {
			return;
		}
		
		if (!IsLocal()) {
			return; // Zoom
		}

		if (!m_Hud) {
			m_Hud = new EditorHud(this);
		}
		
		// how long until this is a node?? :/
		if (!m_Camera) {
			if (!m_Player) {
				m_Player = DayZPlayer.Cast(m_Identity.GetPlayer());
				if (m_Player) {
					Error("Could not find player");
					return;
				}
			}
			
			m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + "0 10 0", ECE_LOCAL));
			m_Camera.SetActive(true);
		}
				
		// What?
		EnScript.SetClassVar(GetDayZGame(), "m_Editor", 0, this);
	}
	
	void InsertHistory(string display_name, Symbols icon, TreeNode node, ScriptReadWriteContext data)
	{
		// Clear the stack first
		foreach (string uuid, TreeNode undo_redo_node: this[HISTORY].Children) {	
			EditorFootprint footprint = EditorFootprint.Cast(undo_redo_node);
			if (!footprint) {
				continue;
			}
						
			if (!footprint.IsUndone()) {
				break;
			}	
			
			delete this[HISTORY][uuid];
		}
		
		string uuid_generated = string.Format("History:%1", this[HISTORY].Children.Count());		
		this[HISTORY][uuid_generated] = new EditorFootprint(uuid_generated, display_name, icon, node, data);
	}
	
	override void Update(float timeslice)
	{
		Input input = GetGame().GetInput();
		if (!m_Camera) {
			return;
		}
								
		if (input.LocalPress_ID(UAUIBack)) {
			TreeNode.StateMachine.RemoveAllStates(TreeNodeState.CONTEXT);
			TreeNode.StateMachine.RemoveAllStates(TreeNodeState.ACTIVE);
		}
		
		Raycast raycast = m_Camera.PerformCursorRaycast();
		if (input.LocalPress_ID(UAFire) || input.LocalPress_ID(UAUIBack)) {			
			
			if (!GetWidgetUnderCursor()) {
				// reassigning because were gonna fuck with this statemachine array
				array<TreeNode> nodes = {};
				nodes.Copy(TreeNode.StateMachine[TreeNodeState.ACTIVE]);
				foreach (TreeNode node_to_deselect: nodes) {
					if (node_to_deselect && node_to_deselect.GetInteractType() == TreeNodeInteract.PRESS) {
						node_to_deselect.RemoveState(TreeNodeState.ACTIVE);
					}
				}
				
				TreeNode.StateMachine.RemoveAllStates(TreeNodeState.CONTEXT);
			}		
		}
						
		foreach (string uuid, TreeNode node1: Children[PLACING].Children) {
			ObjectNode object_node = ObjectNode.Cast(node1);
			if (!object_node) {
				continue;
			}
			
			raycast = m_Camera.PerformCursorRaycast(object_node.GetObject());
			if (!raycast) {
				continue;
			}
			
			vector camera_orthogonal[4] = { raycast.Source.Direction * raycast.Bounce.Direction, raycast.Bounce.Direction, raycast.Source.Direction, raycast.Source.Position };
			Math3D.MatrixOrthogonalize4(camera_orthogonal);	
			
			vector rotation_mat[3];
			Math3D.MatrixIdentity3(rotation_mat);
			if (input.LocalPress_ID(UAZoomInOptics)) {
				Math3D.YawPitchRollMatrix(Vector(-15, 0, 0), rotation_mat);
			}
			
			if (input.LocalPress_ID(UAZoomOutOptics)) {
				Math3D.YawPitchRollMatrix(Vector(15, 0, 0), rotation_mat);
			}
			
			Math3D.MatrixMultiply3(camera_orthogonal, rotation_mat, camera_orthogonal);
			
			//Shape.CreateMatrix(camera_orthogonal);
			
			m_CursorNormal = m_CursorNormal.Multiply3(rotation_mat);
			
			//Print(Placing.Count());
			vector transform[4] = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
			object_node.SetBaseTransform(transform);
		}
		
		if (input.LocalPress_ID(UAZoomIn)) {
			if (GetGame().GetUIManager().IsCursorVisible()) {				
				vector camera_position = m_Camera.GetCursorRay().GetPoint(1000.0);
				raycast = m_Camera.PerformCursorRaycast();
				if (raycast) {
					vector current_position = m_Camera.GetPosition();
					float y_height = current_position[1] - GetGame().SurfaceY(current_position[0], current_position[2]);
					camera_position = raycast.Bounce.GetPoint(y_height);
				}
								
				m_Camera.SetPosition(camera_position);
				m_Camera.Update();
			} else {
				m_Camera.FieldOfView = GameConstants.DZPLAYER_CAMERA_FOV_EYEZOOM;
			}
		}
		
		if (input.LocalRelease_ID(UAZoomIn)) { 
			m_Camera.FieldOfView = 1.0;
		}
	}
				
	override void Write(Serializer serializer, int version)
	{		
		super.Write(serializer, version);
		
		serializer.Write(m_Identity);
		serializer.Write(m_Player);
		serializer.Write(m_Camera);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(m_Identity);	
		serializer.Read(m_Player);
		serializer.Read(m_Camera);			
		return true;
	}
						
	// EditorSounds is helpful
	void PlaySound(string sound_set)
	{
		SEffectManager.PlaySoundOnObject(sound_set, m_Camera);
	}
			
	void SetActive(bool active)
	{
		if (!IsLocal()) {
			return;
		}
		
		if (active) {
			m_Camera.SetActive(true);
			m_Hud.Show(true);
		} else {
			m_Hud.Show(false);
			
			GetDayZGame().SelectPlayer(m_Identity, m_Player);
			Hud hud = GetDayZGame().GetMission().GetHud();
			hud.ShowHudUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.HUD));
			hud.ShowQuickbarUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.QUICKBAR));
		}
	}
	
	void Undo()
	{
		foreach (string uuid, TreeNode node: this[HISTORY].Children) {
			EditorFootprint footprint = EditorFootprint.Cast(node);
			if (!footprint) {
				continue;
			}
			
			if (!footprint.IsUndone()) {
				footprint.Undo();
				return;
			}
		}
	}
	
	void Redo()
	{
		for (int i = this[HISTORY].Children.Count() - 1; i >= 0; i--) {
			EditorFootprint footprint = EditorFootprint.Cast(this[HISTORY].Children.GetElement(i));
			if (!footprint) {
				continue;
			}
			
			if (footprint.IsUndone()) {
				footprint.Redo();
				return;
			}
		}
	}
	
	bool CanUndo() 
	{
		foreach (string uuid, TreeNode node: this[HISTORY].Children) {
			EditorFootprint footprint = EditorFootprint.Cast(node);
			if (footprint && !footprint.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool CanRedo() 
	{
		for (int i = this[HISTORY].Children.Count() - 1; i >= 0; i--) {
			EditorFootprint footprint = EditorFootprint.Cast(this[HISTORY].Children.GetElement(i));
			if (footprint && footprint.IsUndone()) {
				return true;
			}
		}
		
		return false;
	}
	
	TreeNode GetPlacingDestination()
	{
		foreach (TreeNode node: TreeNode.StateMachine[TreeNodeState.FOCUS]) {
			if (node && node.GetState().IsContext()) {
				return node;
			}
		}
		
		foreach (TreeNode tree_node: TreeNode.StateMachine[TreeNodeState.ACTIVE]) {
			FolderNode folder_node = FolderNode.Cast(tree_node);
			if (folder_node && folder_node.GetState().IsActive()) {
				return folder_node;
			}
		}
		
		return GetNode(EditorNode.EDITS);
	}
	
    static float GetSolarDeclination(DateTime date) 
	{			
		DateTime delta = (date - DateTime.Create(date.GetYear(), 1, 1));
        int day_of_year = DateTime.GetDays(delta.GetYear(), delta.GetMonth(), delta.GetDay()) + 1;
        float declination = 23.45 * Math.Sin(Math.DEG2RAD * (360 * (day_of_year + 10) / 365));
        return declination;
	}
	
	DateTime SunniestDay(int year, float lattitude)
	{
		DateTime solstice_date = DateTime.Create(year, 6, 21);
        float declination = GetSolarDeclination(solstice_date);

        for (int i = -10; i < 11; i++) {
            if ((GetSolarDeclination(solstice_date + DateTime.Create(i, 0, 0))) > declination) {
                declination = GetSolarDeclination(solstice_date + DateTime.Create(i, 0, 0));
                solstice_date = solstice_date + DateTime.Create(i, 0, 0);
			}
		}
		
        return solstice_date;
	}
			
	static Man CreateDefaultCharacter(string type, vector position)
	{
		if (GetGame().GetPlayer()) {
			return GetGame().GetPlayer();
		} 

		Man player = Man.Cast(GetGame().CreatePlayer(null, type, position, 0, string.Empty));
		if (!player) {
			Error(string.Format("Failed to create new player, type %1", type));
			return null;
		}

		player.SetPosition(position);
		player.Update();
		if (GetCurrentHoliday() == EditorHoliday.CHRISTMAS) {
			player.GetInventory().CreateAttachment("SantasHat");
			player.GetInventory().CreateAttachment("SantasBeard");
		} else {
			player.GetInventory().CreateInInventory("ConstructionHelmet_Yellow");
		}
		
    	player.GetInventory().CreateInInventory("Shirt_RedCheck");
    	player.GetInventory().CreateInInventory("Jeans_Blue");
    	player.GetInventory().CreateInInventory("WorkingBoots_Brown");
    	player.GetInventory().CreateInInventory("CivilianBelt");
    	player.GetInventory().CreateInInventory("TaloonBag_Blue");
    	player.GetInventory().CreateInInventory("SledgeHammer");
	    return player;
	}
			
	// Just annoying
	static string GetWorldName()
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		return world_name;
	}
		
	static vector GetMapCenterPosition()
	{
		TIntArray values();
		GetGame().ConfigGetIntArray(string.Format("CfgWorlds %1 centerPosition", GetWorldName()), values);
				
		// they were playing the wrong game when they thought of this one
		return Vector(values[0], values[2], values[1]);
	}
	
	static vector GenerateSafeStartPosition(float radius = 2000.0)
	{
		vector position = GetMapCenterPosition();
		position[0] = Math.RandomFloat(position[0] - radius, position[0] + radius);
		position[2] = Math.RandomFloat(position[2] - radius, position[2] + radius);
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 25.0;		
		return position;
	}
			
	static EditorHoliday GetCurrentHoliday()
	{		
		int year, month, day;
		GetYearMonthDayUTC(year, month, day);
		if (month == 12 && day > 12) {
			return EditorHoliday.CHRISTMAS;
		}
		
		if (month == 1 && day < 14) {
			return EditorHoliday.NEWYEARS;
		}
		
		if (month == 10 && day > 0 && day < 14) {
			return EditorHoliday.ANNIVERSARY;
		}
		
		if (month == 10 && day > 14) {
			return EditorHoliday.HALLOWEEN;
		}
		
		return EditorHoliday.NONE;
	}
	
	static Object CreateObject(string type, vector transform[4])
	{
		Math3D.MatrixOrthogonalize4(transform);
		Object object;
		if (type.Contains("\\") || type.Contains("/")) {
			object = GetGame().CreateStaticObjectUsingP3D(type, transform[3], transform[2].VectorToAngles(), 1.0, !GetGame().IsDedicatedServer());
		} else {
			object = GetGame().CreateObjectEx(type, transform[3], ECE_LOCAL | ECE_INITAI | ECE_CREATEPHYSICS | ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_UPDATEPATHGRAPH);
		}
		
		if (!object) {
			return null;
		}
		
		object.SetTransform(transform);
		object.Update();
		
		return object;
	}
	
	static int GetAutoSaveValue(float x)
	{
		return (5 * Math.Pow(x, 4) / 8) - (5 * Math.Pow(x, 3) / 12) - (45 * Math.Pow(x, 2) / 8) + (545 * x / 12) - 25;
	}
					
	EditorHud GetHud() 
	{
		return m_Hud;
	}
		
	EditorCamera GetCamera() 
	{
		return m_Camera;
	}
		
	EditorProfileSettings GetProfileSettings()
	{
		return EditorProfileSettings.Cast(GetDayZGame().GetProfileSetting(EditorProfileSettings));
	}
		
	void SetPlayer(DayZPlayer player)
	{
		m_Player = player;
	}
	
	bool IsActive()
	{
		return m_Camera && m_Camera.IsActive();
	}
	
	bool IsLocal()
	{
		return GetGame() && GetGame().GetUserManager() && GetGame().GetUserManager().GetTitleInitiator() && m_UUID == GetGame().GetUserManager().GetTitleInitiator().GetUid();
	}
		
	TreeNode FindNodeFromObject(Object object)
	{
		// Do this better! searching!!!
		return ObjectNode.All[object];
	}
		
	TreeNode GetObjects()
	{
		return this[EDITS];
	}
	
	TreeNode GetCommands()
	{
		return this[COMMANDS];
	}
	
	CommandNode GetCommand(string command)
	{
		return CommandNode.Cast(this[COMMANDS][command]);
	}
	
	TreeNode GetUndoRedo()
	{
		return this[HISTORY];
	}
	
	TreeNode GetPlaceables()
	{
		return this[PLACEABLES];
	}
	
	TreeNode GetBrushes()
	{
		return this[BRUSHES];
	}
	
	TreeNode GetMenu(string menu)
	{
		return this[MENUS][menu];
	}
	
	TreeNode GetRecycle()
	{
		return this[RECYCLE];
	}
	
	TreeNode GetPlacing()
	{
		return this[PLACING];
	}
}