
static int last_id = 2147483647; // very scuffed lol
class EditorAction: Managed
{
	private int id;
	private string name;
	private bool undone = false;
	
	ref map<Class, ref Param> UndoParameters = null;
	ref map<Class, ref Param> RedoParameters = null;
	
	string m_UndoAction, m_RedoAction;
		
	void EditorAction(string undo_action, string redo_action)
	{
		id = last_id;
		name = undo_action;
		m_UndoAction = undo_action;
		m_RedoAction = redo_action;
		UndoParameters = new map<Class, ref Param>();
		RedoParameters = new map<Class, ref Param>();
		last_id--;
	}
	
	void ~EditorAction()
	{
		Print("~EditorAction");
		
	}
	
	int GetID()
	{
		return id;
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
		undone = true;
		Print("EditorAction::CallUndo");		
		foreach (Class source, Param param: UndoParameters)
			GetGame().GameScript.Call(source, m_UndoAction, param);
		
	}
	
	void CallRedo()
	{
		Print("EditorAction::CallRedo");
		undone = false;
		foreach (Class source, Param param: RedoParameters)
			GetGame().GameScript.Call(source, m_RedoAction, param);
		
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