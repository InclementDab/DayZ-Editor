

class EditorAction: Managed
{
	string m_UndoAction, m_RedoAction;
	
	ref map<Class, ref Param> UndoParameters = null;
	ref map<Class, ref Param> RedoParameters = null;
	
	bool was_undone = false;
	string action_name;
	void EditorAction(string undo_action, string redo_action)
	{	
		action_name = undo_action;
		m_UndoAction = undo_action;
		m_RedoAction = redo_action;
		UndoParameters = new map<Class, ref Param>();
		RedoParameters = new map<Class, ref Param>();
	}
	
	void ~EditorAction()
	{
		Print("~EditorAction");
		
	}
	
	void CallUndo()
	{
		was_undone = true;
		Print("EditorAction::CallUndo");		
		foreach (Class source, Param param: UndoParameters) {
			GetGame().GameScript.Call(source, m_UndoAction, param);
		}		
	}
	
	void CallRedo()
	{
		Print("EditorAction::CallRedo");
		was_undone = false;
		foreach (Class source, Param param: RedoParameters) {
			GetGame().GameScript.Call(source, m_RedoAction, param);
		}	
	}
	
	void InsertUndoParameter(Class source, Param params)
	{
		UndoParameters.Insert(source, params);
	}	
	
	void InsertRedoParameter(Class source, Param params)
	{
		RedoParameters.Insert(source, params);
	}
	
}