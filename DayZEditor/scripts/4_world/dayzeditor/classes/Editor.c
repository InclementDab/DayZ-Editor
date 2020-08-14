
private ref EditorClientModule m_EditorInstance;
EditorClientModule GetEditor() 
{ 	
	//if (m_EditorInstance == null)
	//	m_EditorInstance = new Editor();
	return m_EditorInstance;
}

bool IsEditor() { return m_EditorInstance != null; }


enum EditorClientModuleRPC
{
	INVALID = 36114,
	COUNT
};


class EditorClientModule: JMModuleBase
{
	/* Private Members */
	private Mission m_Mission;
	private UIManager m_UIManager;
	private EditorCamera m_Camera;
	private PlayerBase m_Player;
	EditorCamera GetCamera() { return m_Camera; }
	
	
	private ref EditorBrush					m_EditorBrush;
	private ref EditorBrushSettingsSet 		m_EditorBrushTypes;
	private ref map<string, typename> 		m_CustomBrushList = new map<string, typename>();
	
	
	static Object							ObjectUnderCursor = null;
	static EditorObject 					EditorObjectUnderCursor = null;
	static vector 							CurrentMousePosition;
	
	private bool m_Active = false;
	
	private ref EditorUI m_EditorUI;
	EditorUI GetEditorUI() { return m_EditorUI; }
	
	
	private ref EditorHologram ObjectInHand;
	bool IsPlacing() { return ObjectInHand != null; }
	
	private ref EditorObjectSet 				m_PlacedObjects;
	private ref EditorObjectSet					m_SelectedObjects;
	private ref EditorObjectDataSet			 	m_SessionCache;
	private ref set<ref EditorAction> 			m_ActionStack;
	
	ref set<ref EditorAction> GetActionStack() { return m_ActionStack; }
	ref EditorObjectSet GetSelectedObjects() { return m_SelectedObjects; }
	ref EditorObjectSet GetPlacedObjects() { return m_PlacedObjects; }
	ref EditorObjectDataSet	 GetSessionCache() { return m_SessionCache; }
	
	void DeleteSessionData(int id) { m_SessionCache.Remove(id);	}
	EditorObject GetEditorObject(int id) { return m_PlacedObjects.Get(id); }
	EditorObjectData GetSessionDataById(int id) { return m_SessionCache.Get(id); }
	EditorObject GetPlacedObjectById(int id) { return m_PlacedObjects.Get(id); }
	
	// Public Members
	void EditorClientModule()
	{
		EditorLog.Info("Editor");
		m_EditorInstance = this;	
		
	}
	
	void ~EditorClientModule()
	{
		EditorLog.Info("~Editor");
	}
	
	override void OnInit()
	{
		EditorLog.Trace("Editor::OnInit");
		m_SessionCache = new EditorObjectDataSet();
		
		/* UI Init */
		m_UIManager = GetGame().GetUIManager();
				
		/* EditorUI Init */
		m_EditorUI = new EditorUI();
		m_EditorUI.Show(false);
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
		EditorEvents.OnObjectDeleted.Insert(OnObjectDeleted);
		
		RegisterBinding(new JMModuleBinding("OnEditorToggleActive", "EditorToggleActive"));
		
	}
	
	private bool exit_condition = false;
	override void OnUpdate(float timeslice)
	{
		if (m_Camera != null) {
			ScriptRPC update_rpc = new ScriptRPC();
			update_rpc.Write(m_Camera.GetPosition());
			update_rpc.Write(m_Camera.GetOrientation());
			update_rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_UPDATE, true);
		}
		
		
		set<Object> obj = new set<Object>();
		int x, y;
		GetCursorPos(x, y);
		CurrentMousePosition = MousePosToRay(obj);
	
		if (!IsPlacing()) {
			Object target = obj.Get(0);
			if (target != null) {
				if (target != ObjectUnderCursor) {
					if (ObjectUnderCursor != null) OnMouseExitObject(ObjectUnderCursor, x, y);
					OnMouseEnterObject(target, x, y);
					ObjectUnderCursor = target;
				}
				
			} else if (ObjectUnderCursor != null) {
				exit_condition = OnMouseExitObject(ObjectUnderCursor, x, y);
				ObjectUnderCursor = null;
			}
		}
	}
	
	bool OnMouseEnterObject(Object target, int x, int y)
	{

	}
	
	bool OnMouseExitObject(Object target, int x, int y)
	{

	}
	
		
	void ClearSelection()
	{
		Print("EditorObjectManager::ClearSelections");		
		foreach (EditorObject editor_object: m_SelectedObjects)
			EditorEvents.DeselectObject(this, editor_object);
	
	}
	

	override void OnInvokeConnect(PlayerBase player, PlayerIdentity identity)
	{
		EditorLog.Trace("Editor::OnInvokeConnect");		
		m_Player = player;
		EditorLog.Debug(m_Player);
	}
	
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");
		m_Mission = GetGame().GetMission();
		
		// Only runs in Singleplayer
		if (IsMissionOffline()) {
			EditorLog.Info("Loading Offline Editor...");
			m_Active = true;
			SetActive(m_Active);
		}		
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_CREATED, true);
	}
	
	override void OnMissionFinish()
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_DESTROYED, true);
	}
	
	override void OnMissionLoaded()
	{
		EditorLog.Trace("Editor::OnMissionLoaded");
	}
	
	
	private void OnEditorToggleActive(UAInput input)
	{
		if (!input.LocalPress()) return; 
		EditorLog.Trace("Editor::OnEditorToggleActive");
		m_Active = !m_Active;
		SetActive(m_Active);
	}
	
	
	void SetActive(bool active)
	{
		if (m_Player == null) {
			m_Player = GetGame().GetPlayer();
		}
		
		if (m_Camera == null) {
			// Camera Init
			EditorLog.Info("Initializing Camera");
			m_Camera = GetGame().CreateObjectEx("EditorCamera", m_Player.GetPosition() + Vector(0, 10, 0), ECE_LOCAL);
		}
				
		m_Camera.SetActive(active);
		m_EditorUI.Show(active);
		m_Mission.GetHud().Show(!active);
		
		
		if (!active)
			GetGame().SelectPlayer(m_Player.GetIdentity(), m_Player);
	}
	
	
	
	override int GetRPCMin()
	{
		return EditorClientModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return EditorClientModuleRPC.COUNT;
	}
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	{
		switch (rpc_type) {
			
		
		}
	}
	
	
	void CreateObjects(ref EditorObjectDataSet data_list, bool create_undo = true)
	{
		EditorLog.Trace("Editor::CreateObjects");
		

		EditorAction action = new EditorAction("Delete", "Create");
		
		foreach (EditorObjectData editor_object_data: data_list) {
			
			EditorObject editor_object = new EditorObject(editor_object_data);
			
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));			
		}
		
		if (create_undo) {
			InsertAction(action);
		} else {
			delete action;
		}
		
		EditorEvents.ObjectCreated(this, editor_object);
	}
	
	
	EditorObject CreateObject(ref EditorObjectData editor_object_data, bool create_undo = true)
	{		
		EditorLog.Trace("Editor::CreateObject");
		
		
		EditorObject editor_object = new EditorObject(editor_object_data);
	
		if (create_undo) {
			EditorAction action = new EditorAction("Delete", "Create");;
			action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			InsertAction(action);
		}
		
		EditorEvents.ObjectCreated(this, editor_object);
		return editor_object;
	}

	void DeleteObject(EditorObject target, bool create_undo = true)
	{
		EditorLog.Trace("Editor::DeleteObject");
		EditorEvents.ObjectDeleted(this, target);
		
		if (create_undo) {
			EditorAction action = new EditorAction("Create", "Delete");
			action.InsertUndoParameter(target, new Param1<int>(target.GetID()));
			action.InsertRedoParameter(target, new Param1<int>(target.GetID()));
			InsertAction(action);
		}
		
		delete target;
	}
	
	void DeleteObjects(EditorObjectSet target, bool create_undo = true)
	{
		EditorLog.Trace("Editor::DeleteObjects");
		
		if (create_undo) EditorAction action = new EditorAction("Create", "Delete");
		
		foreach (EditorObject editor_object: target) {
			EditorEvents.ObjectDeleted(this, editor_object);
			if (create_undo) {
				action.InsertUndoParameter(editor_object, new Param1<int>(editor_object.GetID()));
				action.InsertRedoParameter(editor_object, new Param1<int>(editor_object.GetID()));
			}
			
			delete editor_object;
		}	
			
		if (create_undo) InsertAction(action);
	}
	
	
	void InsertAction(EditorAction target)
	{
		int count = m_ActionStack.Count();
		for (int i = 0; i < count; i++) {
			if (m_ActionStack[i].IsUndone()) {
				m_ActionStack.Remove(i);
				i--; count--;
			}
		}
			
		// Adds to bottom of stack
		m_ActionStack.InsertAt(target, 0);
		
		// debug
		m_EditorUI.m_DebugActionStack.ClearItems();
		
		for (int debug_i = 0; debug_i < m_ActionStack.Count(); debug_i++) {
			m_EditorUI.m_DebugActionStack.AddItem(m_ActionStack[debug_i].GetName(), m_ActionStack[debug_i], 0);
		}
	}
	
	
	static ref ModStructure GetModFromObject(string object_name)
	{
		
		ref array<ref ModStructure> mods = ModLoader.GetMods();
		string model_path = GetGame().ConfigGetTextOut("CfgVehicles " + object_name + " model");
		
		foreach (ModStructure mod: mods) {
			string dir;
			string path = mod.GetModPath();
			GetGame().ConfigGetText(string.Format("%1 dir", path), dir);
			dir.ToLower(); model_path.ToLower();
			if (model_path.Contains(dir))
				return mod;
			
		}
		
		return null;
	}
	
	static string GetIconFromMod(ref ModStructure m_ModInfo)
	{
		if (m_ModInfo != null) {
			string logo = m_ModInfo.GetModLogo();
			if (logo == string.Empty)
				logo = m_ModInfo.GetModLogoSmall();
			if (logo == string.Empty)
				logo = m_ModInfo.GetModLogoOver();
			if (logo == string.Empty)
				logo = m_ModInfo.GetModActionURL();
			if (logo != string.Empty)
				return logo;	
		}
		// default
		return "DayZEditor/gui/images/dayz_editor_icon_black.edds";
	}
	
	void OnObjectSelected(Class context, EditorObject target)
	{	
		m_SelectedObjects.InsertEditorObject(target);
	}
	
	void OnObjectDeselected(Class context, EditorObject target)
	{
		m_SelectedObjects.RemoveEditorObject(target);
	}	
	
	void OnObjectCreated(Class context, EditorObject target)
	{
		//m_SelectedObjects.InsertEditorObject(target);
		m_PlacedObjects.InsertEditorObject(target);
		m_PlacedObjectIndex.Insert(target.GetWorldObject().GetID(), target.GetID());
	}	
	
	void OnObjectDeleted(Class context, EditorObject target)
	{
		m_SelectedObjects.RemoveEditorObject(target);
		m_PlacedObjects.RemoveEditorObject(target);
	}
	
	// This is for hashmap lookup of placed objects. very fast
	private ref map<int, int> m_PlacedObjectIndex = new map<int, int>();
	EditorObject GetEditorObject(notnull Object world_object)
	{
		int id = world_object.GetID();
		int editor_obj_id = m_PlacedObjectIndex.Get(id);
		return GetEditorObject(editor_obj_id);
	}
	
	// Brush Management
	void ReloadBrushes(string filename)
	{
		EditorLog.Trace("EditorUIManager::ReloadBrushes");
		XMLEditorBrushes xml_brushes = new XMLEditorBrushes();
		EditorXMLManager.LoadBrushes(xml_brushes, filename);
	}

	
	void SetBrushTypes(EditorBrushSettingsSet brush_types)
	{
		m_EditorUI.ClearBrushBox();
		m_EditorBrushTypes = brush_types;

		foreach (EditorBrushSettings brush: m_EditorBrushTypes)
			m_EditorUI.InsertBrush(brush.Name);		
	}
	
	
	override bool IsServer() { return false; }

}
