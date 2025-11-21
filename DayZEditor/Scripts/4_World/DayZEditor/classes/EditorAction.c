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
		EditorLog.Trace("EditorAction::CallUndo %1", m_UndoAction);		
		m_Undone = true;
		foreach (Param param: UndoParameters) {
			g_Script.Call(this, m_UndoAction, param);
		}
	}
	
	void CallRedo()
	{
		EditorLog.Trace("EditorAction::CallRedo %1", m_UndoAction);
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
	
	void Create(Param params)
	{
		EditorObjectData data;
		vector snapshotPosition = vector.Zero;
		bool useSnapshotPosition = false;

		// Case 1: Undo Delete (ID + Final Position)
		Param2<int, vector> undoDeleteParams = Param2<int, vector>.Cast(params);
		if (undoDeleteParams)
		{
			data = GetEditor().GetSessionDataById(undoDeleteParams.param1);
			snapshotPosition = undoDeleteParams.param2;
			useSnapshotPosition = true;
		}
		// Case 2: Redo Create (Full Data Snapshot)
		else 
		{
			Param1<EditorObjectData> redoCreateParams = Param1<EditorObjectData>.Cast(params);
			if (redoCreateParams)
			{
				data = redoCreateParams.param1;
			}
		}
		
		if (!data) {
			EditorLog.Error("EditorAction::Create Data was null!");
			return;
		}
		
		// Prevent Duplicate Spawning if the ID already exists in the world
		if (GetEditor().GetPlacedObjectById(data.GetID())) 
		{
			return;
		}

		// Temp override data's position with the snapshot from deletion
		vector cachedPosition = data.Position;
		if (useSnapshotPosition) 
		{
			data.Position = snapshotPosition;
		}

		// Respawn the object
		GetEditor().CreateObject(data, false);

		// Revert the data object for session cache
		if (useSnapshotPosition) 
		{
			data.Position = cachedPosition;
		}
	}
	
	void Delete(Param1<int> params)
	{
		//EditorLog.Trace("EditorAction::Delete %1", params.param1.ToString());
        if (!params) return;
		EditorObject object = GetEditor().GetPlacedObjectById(params.param1);
		if (!object) {
			EditorLog.Error("EditorAction::Delete Object was null!");
			return;
		}
		
		GetEditor().DeleteObject(object, false);
	}
	
	void SetTransform(Param4<int, vector, vector, float> params)
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

		float scale = params.param4;
		
		vector matrix[4];
		Math3D.YawPitchRollMatrix(params.param3, matrix);
		matrix[0] = matrix[0] * scale;
		matrix[1] = matrix[1] * scale;
		matrix[2] = matrix[2] * scale;
		matrix[3] = params.param2;
						
		editor_object.SetTransform(matrix);
		editor_object.Update();
		editor_object.UpdateNet();
	}
	
	void Hide(Param1<int> params)
	{
		EditorLog.Trace("EditorAction::Hide %1", params.param1.ToString());
		
		EditorDeletedObjectData data = GetEditor().GetDeletedSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Hide Data was null!");
			return;
		}
		
		if (!GetEditor().HideMapObject(data, false)) {
			EditorLog.Warning("EditorAction::Hide failed to hide object");
		}
	}
	
	void Unhide(Param1<int> params)
	{
		EditorLog.Trace("EditorAction::Unhide %1", params.param1.ToString());
		EditorDeletedObjectData data = GetEditor().GetDeletedSessionDataById(params.param1);
		if (!data) {
			EditorLog.Error("EditorAction::Unhide Data was null!");
			return;
		}
		
		if (!GetEditor().UnhideMapObject(data, false)) {
			EditorLog.Warning("EditorAction::Unhide failed to unhide object");
		}
	}
	
	void Unshow(Param1<int> params)
	{
		EditorObject object = GetEditor().GetPlacedObjectById(params.param1);
		if (!object) {
			EditorLog.Error("EditorAction::Delete Object was null!");
			return;
		}

		object.Show(false);
	}

	void Show(Param1<int> params)
	{
		EditorObject object = GetEditor().GetPlacedObjectById(params.param1);
		if (!object) {
			EditorLog.Error("EditorAction::Delete Object was null!");
			return;
		}

		object.Show(true);
	}
	
	void Lock(Param1<EditorObject> param)
	{
		param.param1.Lock(true);
	}
	
	void Unlock(Param1<EditorObject> param)
	{
		param.param1.Lock(false);
	}
}