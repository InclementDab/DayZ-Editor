
class EditorObjectSet: ref map<int, ref EditorObject>
{
	
	bool InsertEditorObject(EditorObject target)
	{
		return Insert(target.GetID(), target);
	}
	
	void RemoveEditorObject(EditorObject target)
	{
		Remove(target.GetID());
	}
	
}


//typedef ref map<int, ref EditorObject> EditorObjectSet;

class EditorObjectManager: Managed
{
	
	private ref EditorObjectSet 			m_PlacedObjects;
	private ref EditorObjectSet				m_SelectedObjects;
	private ref set<ref EditorObjectLink>	m_SessionCache;
	private ref EditorObjectSet 			m_ClipboardCache;
	private ref set<ref EditorAction> 		m_ActionStack;
	
	
	// Getters
	ref set<ref EditorAction> GetActionStack() { return m_ActionStack; }
	ref EditorObjectSet GetSelectedObjects() { return m_SelectedObjects; }
	ref EditorObjectSet GetPlacedObjects() { return m_PlacedObjects; }
	ref set<ref EditorObjectLink> GetSessionCache() { return m_SessionCache; }
	
	void EditorObjectManager() 
	{ 
		Print("EditorObjectManager");
		m_PlacedObjects 	= new EditorObjectSet();
		m_SelectedObjects	= new EditorObjectSet();
		m_SessionCache 		= new set<ref EditorObjectLink>();
		m_ActionStack 		= new set<ref EditorAction>();
		
	}
	
	void ~EditorObjectManager() 
	{
		Print("~EditorObjectManager");
	}
	
	static int GetPlaceableObjects(out array<string> placeable_objects) 
	{ 
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < g_Game.ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        GetGame().ConfigGetChildName(Config_Path, j, Config_Name);
		        GetGame().ConfigGetBaseName(Config_Path + " " + Config_Name, Base_Name);
		        Base_Name.ToLower();
		
		        //if (Base_Name != "housenodestruct")
		        //    continue;
				
				placeable_objects.Insert(Config_Name);	
				
		    }
		}
		return placeable_objects.Count();
	}
	
	
	EditorObject CreateObject(string name, vector position)
	{		
		Print("EditorObjectManager::CreateObject");
		//bool ai = GetGame().IsKindOf("Hatchback_02_Black", "DZ_LightAI");
		EditorObject editor_object = GetGame().CreateObjectEx("EditorObject", position, ECE_LOCAL);
		editor_object.Init(name);


		// maybe move this into createinvoke	jk lol
		m_SessionCache.Insert(new EditorObjectLink(editor_object));
		m_PlacedObjects.Insert(editor_object.GetID(), editor_object);
		

		// Create Undo / redo action for creation
		EditorObjectLink link = SearchSessionCache(editor_object);
		EditorAction action = new EditorAction("Delete", "Create");
		action.InsertUndoParameter(link, null);
		action.InsertRedoParameter(link, null);
		InsertAction(action);
		EditorEvents.ObjectCreateInvoke(null, editor_object);
		
		return editor_object;
	}
	
	bool SelectObject(EditorObject target, bool reset_selection = true)
	{
		Print("EditorObjectManager::SelectObject");
		if (reset_selection) {
			ClearSelection();
		}
		
		m_SelectedObjects.InsertEditorObject(target);
		EditorEvents.ObjectSelectedInvoke(this, target);
		
		return m_SelectedObjects.Contains(target.GetID());
	}
	
	int SelectObjects(EditorObjectSet target, bool reset_selection = true)
	{
		Print("EditorObjectManager::SelectObjects");
		if (reset_selection)
			ClearSelection();
		
		foreach (EditorObject editor_object: target) {
			if (!SelectObject(editor_object, false)) {
				Print("Failed to select object");
			}
		}
		
		return m_SelectedObjects.Count(); 
		
	}
	
	bool DeselectObject(EditorObject target)
	{
		Print("EditorObjectManager::DeselectObject");
		m_SelectedObjects.Remove(target.GetID()); // if deselection is broken this is why
		EditorEvents.ObjectDeselectedInvoke(this, target);
		
		return m_SelectedObjects.Contains(target.GetID());
	}
	
	
	int DeleteSelection()
	{
		Print("EditorObjectManager::DeleteObject");
		int result = m_SelectedObjects.Count();
		EditorAction action = new EditorAction("Create", "Delete");
		foreach (EditorObject selected_object: m_SelectedObjects) {
			EditorObjectLink link = SearchSessionCache(selected_object);
			action.InsertUndoParameter(link, null);
			action.InsertRedoParameter(link, null);
			GetGame().ObjectDelete(selected_object);
			m_PlacedObjects.Remove(selected_object.GetID());
			
			if (!DeselectObject(selected_object)) {
				Print("Deselection Failed on Deletion");
			}
		}
		
		InsertAction(action);
		GetGame().ObjectDelete(Editor.GetTranslationWidget());
		return result;
	}
	

	
	bool ClearSelection()
	{
		Print("EditorObjectManager::ClearSelections");		
		foreach (EditorObject editor_object: m_SelectedObjects)
			DeselectObject(editor_object);
		
		return m_SelectedObjects.Count() == 0;
	
	}
	
	bool ToggleSelection(EditorObject target)
	{
		if (IsSelected(target))
			return DeselectObject(target);
		
		return SelectObject(target, false);
	}
	
	bool IsSelected(EditorObject target) { return m_SelectedObjects.Contains(target.GetID()); }
	
	
	int CutSelection()
	{
		m_ClipboardCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: m_SelectedObjects) {
			m_ClipboardCache.Insert(id, selected_object);
			DeleteSelection();
		} 
				
		return m_ClipboardCache.Count();
	}
	
	int CopySelection()
	{
		m_ClipboardCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: m_SelectedObjects) {
			m_ClipboardCache.Insert(id, selected_object);			
		}
		
		return m_ClipboardCache.Count();
	}
	
	int PasteSelection()
	{
		ClearSelection();
		vector avg_position;

		foreach (EditorObject copy_object: m_ClipboardCache) {
			avg_position += copy_object.GetPosition();
		}
		
		avg_position[0] = avg_position[0] / m_ClipboardCache.Count();
		avg_position[1] = avg_position[1] / m_ClipboardCache.Count();
		avg_position[2] = avg_position[2] / m_ClipboardCache.Count();
		
		EditorObjectSet pasted_objects;
		foreach (EditorObject editor_object: m_ClipboardCache) {
			vector mat[4];
			editor_object.GetTransform(mat);
			mat[3] = avg_position - editor_object.GetPosition() + Editor.CurrentMousePosition;
			EditorObject result = CreateObject(editor_object.GetType(), mat);
			pasted_objects.InsertEditorObject(result);
		}
		
		SelectObjects(pasted_objects);
		
		return pasted_objects.Count();
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
		GetEditor().GetUIManager().GetEditorUI().m_DebugActionStack.ClearItems();
		
		for (int debug_i = 0; debug_i < m_ActionStack.Count(); debug_i++) {
			GetEditor().GetUIManager().GetEditorUI().m_DebugActionStack.AddItem(m_ActionStack[debug_i].GetName(), m_ActionStack[debug_i], 0);
		}
		
	}
	
	
	bool CheckIfRootIsSelected(Widget root)
	{
		
		foreach (EditorObject editor_object: m_SelectedObjects)
			if (GetEditor().GetObjectManager().IsSelected(editor_object) && (editor_object.GetObjectBrowser() == root || editor_object.GetObjectMarker() == root))
				return true;
		
		return false;
	}
	

	void Save()
	{	
		EditorWorldData save_data = new EditorWorldData();
		GetEditor().GetUIManager().GetEditorCamera().GetTransform(save_data.CameraPosition);
		
		foreach (EditorObject save_object: m_PlacedObjects)	
			save_data.WorldObjects.Insert(save_object.GetSaveData());
		
		
		EditorFileManager.SaveFile(save_data);
	}
	
	void Open()
	{
		EditorWorldData load_data = EditorFileManager.LoadFile();
		GetEditor().GetUIManager().GetEditorCamera().SetTransform(load_data.CameraPosition);
		// find a proper way to remove all existing files. maybe delete the object manager :)
		
		foreach (EditorWorldObject load_object: load_data.WorldObjects) {
			EditorObject e_object = CreateObject(load_object.m_Typename, load_object.m_Transform[3]);
			
			m_PlacedObjects.Insert(e_object.GetID(), e_object);
		}
	}
	
	// O(n) shit :)
	EditorObject GetEditorObject(Object target)
	{
		foreach (EditorObject editor_object: m_PlacedObjects) 
			if (editor_object.GetObject() == target) 
				return editor_object;
		
		return null;
	}
	
	EditorObjectLink SearchSessionCache(EditorObject target)
	{
		foreach (EditorObjectLink editor_object_link: m_SessionCache)
			if (editor_object_link.Ptr() == target)
				return editor_object_link;
		
		return null;
	}
	
	
	

}