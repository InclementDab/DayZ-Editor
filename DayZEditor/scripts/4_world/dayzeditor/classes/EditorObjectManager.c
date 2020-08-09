
class EditorObjectSet: map<int, EditorObject>
{
	
	void ~EditorObjectSet()
	{
		foreach (EditorObject obj: this)
			GetGame().ObjectDelete(obj);
		
		
	}
	
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
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
		EditorEvents.OnSelectionCleared.Insert(OnSelectionCleared);
		EditorEvents.OnObjectDeleted.Insert(OnObjectDeleted);
		
	}
	
	void ~EditorObjectManager() 
	{
		Print("~EditorObjectManager");
		
		delete m_PlacedObjects;
		delete m_SelectedObjects;
		delete m_SessionCache;
		delete m_ClipboardCache;
		delete m_ActionStack;
	}
	
	static int GetPlaceableObjects(out EditorPlaceableObjectSet placeable_objects) 
	{ 
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < g_Game.ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        g_Game.ConfigGetChildName(Config_Path, j, Config_Name); 
				
				EditorPlaceableObject placeable_object = new EditorPlaceableObject(Config_Name, Config_Path);
				placeable_objects.Insert(placeable_object);
		    }
		}
		

		
		return placeable_objects.Count();
	}
	
	
	EditorObject CreateObject(string name, vector position)
	{		
		Print("EditorObjectManager::CreateObject");
		//bool ai = GetGame().IsKindOf("Hatchback_02_Black", "DZ_LightAI");
		EditorObject editor_object = EditorObject.Cast(GetGame().CreateObjectEx("EditorObject", position, ECE_LOCAL));
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
	
	void OnObjectSelected(Class context, EditorObject target)
	{
		Print("EditorObjectManager::SelectObject");
		m_SelectedObjects.InsertEditorObject(target);
	}
	
	void OnObjectDeselected(Class context, EditorObject target)
	{
		Print("EditorObjectManager::SelectObject");
		m_SelectedObjects.RemoveEditorObject(target);
	}	
	
	void OnObjectDeleted(Class context, EditorObject target)
	{
		Print("EditorObjectManager::SelectObject");
		m_SelectedObjects.RemoveEditorObject(target);
	}
	
	
	/*
	int SelectObjects(EditorObjectSet target, bool reset_selection = true)
	{
		Print("EditorObjectManager::SelectObjects");
		if (reset_selection)
			ClearSelection();
		
		foreach (EditorObject editor_object: target)
			if (!SelectObject(editor_object, false))
				Print("Failed to select object");
		
		return m_SelectedObjects.Count(); 
		
	}
	*/

	
	
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
			EditorEvents.DeselectObject(this, selected_object);
			
		}
		
		InsertAction(action);
		GetGame().ObjectDelete(GetEditor().GetTranslationWidget());
		return result;
	}
	

	
	void OnSelectionCleared(Class context)
	{
		Print("EditorObjectManager::ClearSelections");		
		foreach (EditorObject editor_object: m_SelectedObjects)
			EditorEvents.DeselectObject(this, editor_object);
	
	}
	
	void ToggleSelection(EditorObject target)
	{
		if (IsSelected(target))
			EditorEvents.DeselectObject(this, target);
		
		EditorEvents.SelectObject(this, target);
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
		EditorEvents.ClearSelection(this);
		vector avg_position;

		foreach (EditorObject copy_object: m_ClipboardCache) {
			avg_position += copy_object.GetPosition();
		}
		
		avg_position[0] = avg_position[0] / m_ClipboardCache.Count();
		avg_position[1] = avg_position[1] / m_ClipboardCache.Count();
		avg_position[2] = avg_position[2] / m_ClipboardCache.Count();
		
		EditorObjectSet pasted_objects = new EditorObjectSet();
		foreach (EditorObject editor_object: m_ClipboardCache) {
			vector mat[4];
			editor_object.GetTransform(mat);
			mat[3] = avg_position - editor_object.GetPosition() + Editor.CurrentMousePosition;
			EditorObject result = CreateObject(editor_object.GetType(), mat);
			pasted_objects.InsertEditorObject(result);
			EditorEvents.SelectObject(this, editor_object);
		}
		
		
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
	
	EditorObject GetEditorObjectFromListItem(EditorPlacedListItem list_item)
	{
		foreach (EditorObject editor_object: m_PlacedObjects)
			if (editor_object == list_item.GetEditorObject())
				return editor_object;
		
		
		Print("EditorObjectManager::GetEditorObjectFromListItem Item Not Found!");
		return null;
	}
	
	
	bool CheckIfRootIsSelected(Widget root)
	{
		foreach (EditorObject editor_object: m_SelectedObjects)
			if (IsSelected(editor_object) && editor_object.IsRootSelected(root))
				return true;
		
		return false;
	}
	
	EditorObject GetFromUILinkedRoot(Widget root)
	{
		Print("EditorObjectManager::GetFromObjectRoot");
		foreach (EditorObject editor_object: m_PlacedObjects)
			if (editor_object.IsRootSelected(root))
				return editor_object;
		
		
		Print("EditorObjectManager::GetFromUILinkedRoot: Item Not Found!");
		return null;
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