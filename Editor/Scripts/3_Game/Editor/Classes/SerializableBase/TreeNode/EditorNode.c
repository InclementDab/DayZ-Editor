/*
                    Euler sleeps here
               __..--''``---....___   _..._    __
     /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /
    ///_.-' _..--.'_    \                    `( ) ) // //
    / (_..-' // (< _     ;_..__               ; `' / ///
     / // // //  `-._,_)' // / ``--...____..-' /// / //
*/

class EditorNode: TreeNode
{	
	void LegacyLoad(notnull EditorSaveData save_data)
	{
		m_Camera.SetPosition(save_data.CameraPosition);
		foreach (EditorObjectData data: save_data.EditorObjects) {
			GetPlacingDestination().Add(data.CovertToNode());
		}
	}
	
	static const ref array<string> CATEGORIES = { "Unknown", "Plants", "Rocks", "Clutter", "Structures", "Wrecks", "AI", "Water", "Vehicles", "StaticObjects", "DynamicObjects", "ScriptedObjects" };
	
	PlayerIdentity Identity;
	DayZPlayer Player;
	DateTime Date = DateTime.Now(false);
	// this isnt 'const' because climate change exists in DayZ
	int Climate = ARGB(0 /* todo, low-precision forecast? */, GetGame().GetWeather().GetFog().GetActual() * 255, GetGame().GetWeather().GetRain().GetActual(), GetGame().GetWeather().GetOvercast().GetActual());
	
	protected EditorCamera m_Camera;
	protected ref EditorHud	m_Hud;
		
	//[TreeNode("SERVER\\Layers", "Layers", Symbols.LAYER_GROUP)]
	static const string LAYERS = "Layers";
	
		//[TreeNode("SERVER\\Layers\\Brushed", "Brushed", Symbols.PAINTBRUSH)];
		static const string BRUSHED = "Brushed";
		static const string HIDDEN = "Hidden";
	
	
	static const string MENUS = "Menus";
	static const string HISTORY = "UndoRedo";
	static const string PLACEABLES = "Placeables";
	static const string BRUSHES = "Brushes";
	static const string PLACING = "Placing";
	static const string RECYCLE = "Recycle";
		
	void EditorNode(string uuid, string display_name, Symbols icon) 
	{				
		// Load all default categories and placements
		Add(new TreeNode(LAYERS, "Layers", Symbols.LAYER_GROUP));
		Add(new TreeNode(MENUS, "Menus", Symbols.SQUARE_LIST));
		Add(new TreeNode(HISTORY, "History", Symbols.CLOCK_ROTATE_LEFT));
		Add(new TreeNode(PLACEABLES, "Placeable Objects", Symbols.ADDRESS_BOOK));
		Add(new TreeNode(BRUSHES, "Brushes", Symbols.BRUSH));
		Add(new TreeNode(PLACING, "Placing", Symbols.FIREPLACE));
		Add(new TreeNode(RECYCLE, "Recycle Bin", Symbols.BIN_RECYCLE));			
		
		this[LAYERS].Add(new TreeNode(BRUSHED, "Brushed", Symbols.PAINTBRUSH));
		this[LAYERS].Add(new TreeNode(HIDDEN, "Hidden", Symbols.PAINTBRUSH));
		// default layer for now
		this[LAYERS].AddState(TreeNodeState.ACTIVE);
		
		/*
		
		this[MENUS].Add(new CommandNode("File", "File", Symbols.FILE_SPREADSHEET));
		this[MENUS].Add(new CommandNode("Edit", "Edit", Symbols.FILE_PEN));
		this[MENUS].Add(new CommandNode("View", "View", Symbols.BINOCULARS));
		this[MENUS].Add(new CommandNode("Mission", "Mission", Symbols.PERSON_RIFLE));
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["New"]);
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["Open"]);
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["Save"]);
		this[MENUS]["File"].Add(GetDayZGame().GetSandbox()[COMMANDS]["SaveAs"]);
		*/
		
		this[BRUSHES].Add(new BetulaPendula_Brush("BetulaPendula_Brush", "Betula Pendula", Symbols.TREES));
		this[BRUSHES].Add(new LightningBrush("LightningBrush", "Lightning Brush", Symbols.BOLT));
		//this[BRUSHES].Add(this[COMMANDS]["Piano"]);
	}

	void ~EditorNode() 
	{
		if (GetGame() && GetGame().GetUpdateQueue(CALL_CATEGORY_GUI)) {
			GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		}
		
		delete m_Hud;
		GetGame().ObjectDelete(m_Camera);
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		GetDayZGame().SetDate(Date);
		
		float _, f, r, o;
		InverseARGBF(Climate, _, f, r, o);
		GetGame().GetWeather().GetFog().Set(f);
		GetGame().GetWeather().GetRain().Set(r);
		GetGame().GetWeather().GetOvercast().Set(o);
		
		Player.GetInputController().SetDisabled(true);
	}
	
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
		
		if (state.IsActive()) {
			if (total_state.IsActive()) {
				m_Camera.SetActive(true);
				m_Hud.Show(true);
				Player.GetInputController().SetDisabled(true);
			} else {
				m_Hud.Show(false);
				
				GetDayZGame().SelectPlayer(Identity, Player);
				Player.GetInputController().SetDisabled(false);
				Hud hud = GetDayZGame().GetMission().GetHud();
				hud.ShowHudUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.HUD));
				hud.ShowQuickbarUI(GetDayZGame().GetProfileOption(EDayZProfilesOptions.QUICKBAR));
			}
		}
	}
	
	override void OnSynchronized()
	{
		super.OnSynchronized();
				
#ifndef SERVER
		if (IsLocal()) {
			delete m_Hud;
			if (!m_Hud) {
				m_Hud = new EditorHud(this);
			}
		
			// how long until this is a node?? :/
			if (!m_Camera) {		
				m_Camera = EditorCamera.Cast(GetGame().CreateObjectEx("EditorCamera", Player.GetPosition() + "0 10 0", ECE_LOCAL));
				m_Camera.SetActive(true);
			}
		}		
#endif
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
					
	override void Write(Serializer serializer, int version)
	{		
		super.Write(serializer, version);
		
		serializer.Write(Identity);
		serializer.Write(Player);
		serializer.Write(Date);
	}
	
	override bool Read(Serializer serializer, int version)
	{		
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(Identity);	
		serializer.Read(Player);			
		serializer.Read(Date);			
		return true;
	}
						
	// EditorSounds is helpful
	void PlaySound(string sound_set)
	{
		SEffectManager.PlaySoundOnObject(sound_set, m_Camera);
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
			LayerNode folder_node = LayerNode.Cast(tree_node);
			if (folder_node && folder_node.GetState().IsActive()) {
				return folder_node;
			}
		}
		
		return Get(EditorNode.LAYERS);
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
			object = GetGame().CreateStaticObjectUsingP3D(type, transform[3], transform[2].VectorToAngles(), 1.0, true);
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
	
	override TreeNodeState GetDefaultState()
	{
		return TreeNodeState.ACTIVE;
	}

	override TreeNodeState GetStateMask()
	{
		return TreeNodeState.ACTIVE | TreeNodeState.CONTEXT;
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
		
	TreeNode GetLayers()
	{
		return this[LAYERS];
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