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

class EditorAction: Managed
{
	protected bool m_Undone;
	protected string m_UndoAction, m_RedoAction;
	
	ref array<ref Param> UndoParameters = {};
	ref array<ref Param> RedoParameters = {};
			
	void EditorAction(string undo_action, string redo_action)
	{
		m_UndoAction = undo_action;
		m_RedoAction = redo_action;
	}
	
	string GetUndoAction() 
	{ 
		return m_UndoAction; 
	}
	
	string GetRedoAction()
	{
		return m_RedoAction;
	}
	
	bool IsUndone() 
	{ 
		return m_Undone; 
	}
	
	void CallUndo()
	{
		m_Undone = true;
		foreach (Param param: UndoParameters) {
			g_Script.Call(this, m_UndoAction, param);
		}
	}
	
	void CallRedo()
	{
		m_Undone = false;
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
	
	void Create(Param1<ref EditorObjectData> params)
	{		
		GetEditor().CreateObject(params.param1.CreateObject(), false);
	}
	
	void Delete(Param1<ref EditorObjectData> params)
	{
		/*
		EditorObject object = GetEditor().GetPlacedObjectById(params.param1);
		if (!object) {
			EditorLog.Error("EditorAction::Delete Object was null!");
			return;
		}
		
		GetEditor().DeleteObject(object, false);*/
	}
	
	void SetTransform(Param3<ref EditorObjectData, vector, vector> params)
	{
		/*EditorObjectData editor_object_data = GetEditor().GetSessionDataById(params.param1);
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
		editor_object.SetOrientation(params.param3);*/
	}
	
	void Hide(Param1<ref EditorObjectData> params)
	{
		/*EditorLog.Trace("EditorAction::Hide %1", params.param1.ToString());
		
		EditorHiddenObjectData data = GetEditor().GetDeletedSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Hide Data was null!");
			return;
		}
		
		if (!GetEditor().HideMapObject(data, false)) {
			EditorLog.Warning("EditorAction::Hide failed to hide object");
		}*/
	}
	
	void Unhide(Param1<ref EditorObjectData> params)
	{
		/*EditorLog.Trace("EditorAction::Unhide %1", params.param1.ToString());
		EditorHiddenObjectData data = GetEditor().GetDeletedSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Unhide Data was null!");
			return;
		}
		
		if (!GetEditor().UnhideMapObject(data, false)) {
			EditorLog.Warning("EditorAction::Unhide failed to unhide object");
		}*/
	}
	
	void Lock(Param1<EditorObject> param)
	{
		//param.param1.Lock(true);
	}
	
	void Unlock(Param1<EditorObject> param)
	{
		//param.param1.Lock(false);
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