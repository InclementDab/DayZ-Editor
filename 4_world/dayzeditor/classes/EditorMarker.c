
class EditorUI: UIScriptedMenu
{
	static ref EditorUI m_Instance;
	static EditorUI GetInstance() { return m_Instance; }
	WrapSpacerWidget GetRightPanelSpacer() {}
}



class EditorObjectMarker: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected ref EditorObject m_EditorObject = null;
	
	// Object Markers
	protected Widget m_EditorObjectMarkerPanel;
	protected ImageWidget m_EditorObjectMarkerImage;
	
	// Browser Items
	protected Widget m_EditorListItemFrame;
	protected TextWidget m_EditorListItemText;	
	
		
	void ~EditorObjectMarker()
	{
		Print("~EditorObjectMarker");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		delete m_Root; delete m_EditorObjectMarkerPanel; delete m_EditorObjectMarkerImage;
		delete m_EditorListItemFrame; delete m_EditorListItemText;
	}
	
	void SetObject(notnull EditorObject target) 
	{
		Print("EditorObjectMarker::SetObject");
		m_EditorObject = target;
		m_EditorListItemText.SetText(m_EditorObject.WorldObject.GetType());
		EditorUI ui = EditorUI.GetInstance();
	
		ui.GetRightPanelSpacer().AddChild(m_EditorListItemFrame);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}

	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarker::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorObjectMarkerPanel = m_Root.FindAnyWidget("EditorObjectMarkerPanel");
		m_EditorObjectMarkerImage = ImageWidget.Cast(m_Root.FindAnyWidget("EditorObjectMarkerImage"));		
		
		m_EditorListItemFrame = m_Root.FindAnyWidget("EditorListItemFrame");
		m_EditorListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
		

	}

	
	void Update()
	{
		vector clip_info[2];
		float radius = m_EditorObject.WorldObject.ClippingInfo(clip_info);	
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		position = position + m_EditorObject.WorldObject.GetPosition();
		position[1] = position[1] - clip_info[1][1];
		vector screenpos = GetGame().GetScreenPos(position);
		m_EditorObjectMarkerPanel.SetPos(screenpos[0], screenpos[1]);
	}
	

	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		
		if (w == m_EditorObjectMarkerPanel)
			m_EditorObjectMarkerPanel.SetAlpha(0.75);
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_EditorObjectMarkerPanel)
			m_EditorObjectMarkerPanel.SetAlpha(0.25);
		return true;
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Input input = GetGame().GetInput();
		Print("EditorObject::OnMouseButtonDown");
		Print(w.GetName());
		if (button == 0) {
			
			//if (w == GetFocus()) return true;

			if (w.GetName() == "EditorListItemFrame") {

				if (Editor.IsPlacing())
					delete Editor.ObjectInHand;
				
				SetFocus(m_Root);
				
			} else if (w.GetName() == "EditorObjectMarkerPanel") {
				Editor.CreateSelection(m_EditorObject, !input.LocalValue("UATurbo"));
				SetFocus(m_Root);
			}			
		}
		return true;
	}
	
	void Select()
	{
		Print("EditorObject::OnFocus");
		m_EditorListItemFrame.SetColor(COLOR_BLUE);
		m_EditorObjectMarkerPanel.SetAlpha(1);
	}
	
	void Deselect()
	{
		Print("EditorObject::OnFocusLost");
		m_EditorListItemFrame.SetColor(0xFFFFFFFF);
		m_EditorObjectMarkerPanel.SetAlpha(0.25);
	}
	
	

	
	
	override bool OnDrag(Widget w, int x, int y)
	{
		Print("EditorObjectMarker::OnDrag");
		
		return m_EditorObject.OnDrag(w, x, y);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget r)
	{
		Print("EditorObjectMarker::OnDrop");
		Widget r;
		return m_EditorObject.OnDrop(w, x, y, r);
	}
	
	
	
	
	
	Widget GetLayoutRoot()
	{
		return m_EditorListItemFrame;
	}
	
	void GetMarkerPosition(out float x, out float y)
	{
		m_EditorObjectMarkerPanel.GetPos(x, y);
	}
}

