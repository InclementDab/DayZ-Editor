
class EditorObject: ScriptedWidgetEventHandler
{
	
}



class EditorObjectMarker: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Object m_Building = null;
	protected Widget m_EditorObjectMarkerPanel;
	protected ImageWidget m_EditorObjectMarkerImage;
	
	
	void ~EditorObjectMarker()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		Print("~EditorObjectMarker");
	}
	
	void Init(Object building)
	{
		m_Building = building;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarker::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorObjectMarkerPanel = m_Root.FindAnyWidget("EditorObjectMarkerPanel");
		m_EditorObjectMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorObjectMarkerImage"));
	}
	
		
	void Update()
	{
		if (m_Building != null) {
			vector position = m_Building.GetPosition();
			vector screenpos = GetGame().GetScreenPos(position);
			m_Root.SetPos(screenpos[0], screenpos[1]);
		}
		
		
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorObjectMarker::OnMouseButtonDown");
		SetFocus(w);
		
		return true;
	}
	

	
	
}
