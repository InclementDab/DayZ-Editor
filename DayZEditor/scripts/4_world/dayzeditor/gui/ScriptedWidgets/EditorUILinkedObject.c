class UILinkedObject : ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected EditorObject m_EditorObject = null;
	
	void ~UILinkedObject() 
	{ 
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_Root; 
	}
	
	void Update() {}
	
	void SetObject(notnull EditorObject target) 
	{
		m_EditorObject = target;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
	}
	
	
	Widget GetLayoutRoot() { return m_Root;	}
	
	void GetPos(out float x, out float y) { m_Root.GetPos(x, y); }
}