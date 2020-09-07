class EditorActionStack: set<ref EditorAction>
{
	
	int InsertAction(EditorAction value)
	{	
		int count = Count();
		for (int i = 0; i < count; i++) {
			if (this[i].IsUndone()) {
				Remove(i);
				i--; count--;
			}
		}

		// Adds to bottom of stack
		return InsertAt(value, 0);
	}
	
	void UpdateDebugReadout(out ref ObservableCollection<string> readout)
	{
		readout.Clear();
		foreach (EditorAction action: this) {
			readout.Insert(action.GetName());
		}
	}	
}


class EditorAction
{
	private string name;
	private bool undone = false;
	
	ref map<int, ref Param> UndoParameters = null;
	ref map<int, ref Param> RedoParameters = null;
	
	string m_UndoAction, m_RedoAction;
			
	void EditorAction(string undo_action, string redo_action)
	{
		name = undo_action;
		m_UndoAction = undo_action;
		m_RedoAction = redo_action;
		UndoParameters = new map<int, ref Param>();
		RedoParameters = new map<int, ref Param>();
	}
	
	void ~EditorAction()
	{
		EditorLog.Trace("~EditorAction");
		
		foreach (int i, ref Param p: UndoParameters)
			GetEditor().DeleteSessionData(i);
		
	}
	
	string GetName() { 
		return name; 
	}
	
	bool IsUndone() { 
		return undone; 
	}
	
	void CallUndo()
	{
		EditorLog.Trace("EditorAction::CallUndo");		
		undone = true;
		foreach (int id, ref Param param: UndoParameters) {			
			GetGame().GameScript.Call(this, m_UndoAction, param);
		}
	}
	
	void CallRedo()
	{
		EditorLog.Trace("EditorAction::CallRedo");
		undone = false;
		foreach (int id, ref Param param: RedoParameters) {
			GetGame().GameScript.Call(this, m_RedoAction, param);
		}
		
	}
	
	void InsertUndoParameter(EditorObject source, ref Param params)
	{
		EditorLog.Trace("InsertUndoParameter");
		UndoParameters.Insert(source.GetID(), params);
	}	
	
	void InsertRedoParameter(EditorObject source, ref Param params)
	{
		EditorLog.Trace("InsertRedoParameter");
		RedoParameters.Insert(source.GetID(), params);
	}
	
	
	void Create(Param1<int> params)
	{
		EditorLog.Trace("EditorAction::Create");
		Sleep(10);
		GetEditor().CreateObject(GetEditor().GetSessionDataById(params.param1), false);
	}
	
	void Delete(Param1<int> params)
	{
		EditorLog.Trace("EditorAction::Delete");
		Sleep(10);
		GetEditor().DeleteObject(GetEditor().GetPlacedObjectById(params.param1), false);
	}
	
	
	void SetTransform(Param3<int, vector, vector> params)
	{
		EditorLog.Trace("EditorObject::SetTransform");
		
		Sleep(10);
		EditorObject editor_object = GetEditor().GetPlacedObjectById(params.param1);
		editor_object.SetPosition(params.param2);
		editor_object.SetOrientation(params.param3);
		editor_object.Update();
		
	}	
}