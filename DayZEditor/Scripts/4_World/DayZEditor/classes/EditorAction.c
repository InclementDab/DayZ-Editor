class EditorActionStack: set<ref EditorAction>
{
	static const int MAX_SIZE = 512;
	
	int InsertAction(EditorAction value)
	{	
		int count = Count();
		for (int i = 0; i < count; i++) {
			if (!this[i].IsUndone()) {
				break;
			}
			
			Remove(i);
			i--; count--;
		}
		
		if (count >= MAX_SIZE) {
			Remove(count - 1);
		}
		
		// Adds to bottom of stack
		return InsertAt(value, 0);
	}
}

class EditorAction
{
	private string name;
	private bool undone = false;
	
	ref array<ref Param> UndoParameters = {};
	ref array<ref Param> RedoParameters = {};
	
	string m_UndoAction, m_RedoAction;
			
	void EditorAction(string undo_action, string redo_action)
	{
		name = undo_action;
		m_UndoAction = undo_action;
		m_RedoAction = redo_action;
	}
	
	string GetName() 
	{ 
		return name; 
	}
	
	bool IsUndone() 
	{ 
		return undone; 
	}
	
	void CallUndo()
	{
		EditorLog.Trace("EditorAction::CallUndo %1", name);		
		undone = true;
		foreach (Param param: UndoParameters) {
			g_Script.Call(this, m_UndoAction, param);
		}
	}
	
	void CallRedo()
	{
		EditorLog.Trace("EditorAction::CallRedo %1", name);
		undone = false;
		foreach (Param param: RedoParameters) {
			g_Script.Call(this, m_RedoAction, param);
		}
	}
	
	void InsertUndoParameter(Param params)
	{
		UndoParameters.Insert(params);
	}
			
	void InsertRedoParameter(Param params)
	{
		RedoParameters.Insert(params);
	}
	
	void Create(Param1<int> params)
	{
		//EditorLog.Trace("EditorAction::Create %1", params.param1.ToString());
		EditorObjectData data = GetEditor().GetSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Create Data was null!");
			return;
		}
		
		GetEditor().CreateObject(data, false);
	}
	
	void Delete(Param1<int> params)
	{
		//EditorLog.Trace("EditorAction::Delete %1", params.param1.ToString());
		EditorObject object = GetEditor().GetPlacedObjectById(params.param1);
		if (!object) {
			EditorLog.Error("EditorAction::Delete Object was null!");
			return;
		}
		
		GetEditor().DeleteObject(object, false);
	}
	
	void SetTransform(Param3<int, vector, vector> params)
	{
		//EditorLog.Trace("EditorAction::SetTransform");
		EditorObjectData editor_object_data = GetEditor().GetSessionDataById(params.param1);
		if (!editor_object_data) {
			EditorLog.Error("EditorAction::SetTransform EditorObjectData was null!");
			return;
		}
		
		EditorObject editor_object = GetEditor().GetPlacedObjectById(editor_object_data.GetID());
		if (!editor_object) {
			EditorLog.Error("EditorAction::SetTransform EditorObject was null!");
			return;
		}

		editor_object.SetPosition(params.param2);
		editor_object.SetOrientation(params.param3);
	}
	
	void Hide(Param1<int> params)
	{
		//EditorLog.Trace("EditorAction::Hide %1", param.param1.ToString());
		
		EditorDeletedObjectData data = GetEditor().GetDeletedSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Create Data was null!");
			return;
		}
		
		GetEditor().HideMapObject(data, false);
	}
	
	void Unhide(Param1<int> params)
	{
		//EditorLog.Trace("EditorAction::Unhide %1", param.param1.ToString());
				
		EditorDeletedObjectData data = GetEditor().GetDeletedSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Create Data was null!");
			return;
		}
		
		GetEditor().UnhideMapObject(data, false);
	}
	
	void Lock(Param1<EditorObject> param)
	{
		param.param1.Lock(true);
	}
	
	void Unlock(Param1<EditorObject> param)
	{
		param.param1.Lock(false);
	}
	
	void CreateCameraTrack(SerializedCameraTrack params)
	{
		EditorLog.Trace("EditorAction::CreateCameraTrack %1", params.param5);
		EditorCameraTrackListItem list_item(params.param1, params.param2, params.param3, params.param5, params.param4);
		GetEditor().GetCameraTrackManager().InsertCameraTrack(list_item);
	}
	
	void DeleteCameraTrack(SerializedCameraTrack params)
	{
		EditorLog.Trace("EditorAction::DeleteCameraTrack %1", params.param1.ToString());
		
		/*foreach (EditorCameraTrackListItem list_item: GetEditor().GetEditorHud().GetTemplateController().CameraTrackItems) {
			
		}*/
		
		//GetEditor().GetEditorHud().GetTemplateController().RemoveCameraTrack(params.param1);	
	}
}