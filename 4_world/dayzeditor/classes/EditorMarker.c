
class IEditorUI: UIScriptedMenu
{
	static ref IEditorUI m_Instance;
	static IEditorUI GetInstance() { return m_Instance; }
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
	Widget m_EditorListItemFrame;
	TextWidget m_EditorListItemText;	
	
	static float ALPHA_ON_SHOW = 1;
	static float ALPHA_ON_HIDE = 0.25;
	
	
		
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
		IEditorUI ui = IEditorUI.GetInstance();
	
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

	bool is_selected = false;
	
	void Update()
	{
		vector clip_info[2];
		float radius = m_EditorObject.WorldObject.ClippingInfo(clip_info);	
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		position = position + m_EditorObject.WorldObject.GetPosition();
		position[1] = position[1] - clip_info[1][1];
		
		vector screenpos = GetGame().GetScreenPos(position);
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		if (screenpos[0] > screen_x || screenpos[1] > screen_y || screenpos[0] <= 0 || screenpos[1] <= 0) {
			m_EditorObjectMarkerPanel.Show(false);
			return;
		}
		if (!is_selected) {
			float distance = vector.Distance(position, GetGame().GetCurrentCameraPosition());
			float alpha = ALPHA_ON_HIDE * 1 / (distance/250) * (distance < 1000);
			m_EditorObjectMarkerPanel.SetAlpha(alpha); 
		}
		
		m_EditorObjectMarkerPanel.Show(true); 
		m_EditorObjectMarkerPanel.SetPos(screenpos[0], screenpos[1]);
	}
	

	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		// you should set cursor here its smart smile :)
		
		if (w == m_EditorObjectMarkerPanel)
			m_EditorObjectMarkerPanel.SetAlpha(ALPHA_ON_SHOW);
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == m_EditorObjectMarkerPanel)
			m_EditorObjectMarkerPanel.SetAlpha(ALPHA_ON_HIDE);
		return true;
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Input input = GetGame().GetInput();
		Print("EditorObject::OnMouseButtonDown");
		Print(w.GetName());
		if (button == 0) {
			
			if (w.GetName() == "EditorListItemFrame") {

				if (Editor.IsPlacing())
					delete Editor.ObjectInHand;
				
				SetFocus(m_Root);
				
			} else if (w.GetName() == "EditorObjectMarkerPanel" && Editor.SelectedObjects.Find(m_EditorObject) == -1) {
				Editor.CreateSelection(m_EditorObject, !input.LocalValue("UATurbo"));
				SetFocus(m_Root);				
			}			
		}
		return true;
	}
	
	void Select()
	{
		is_selected = true;
		m_EditorListItemFrame.SetColor(COLOR_BLUE);
		m_EditorObjectMarkerPanel.SetAlpha(ALPHA_ON_SHOW);
	}
	
	void Deselect()
	{
		is_selected = false;
		m_EditorListItemFrame.SetColor(0xFFFFFFFF);
		m_EditorObjectMarkerPanel.SetAlpha(ALPHA_ON_HIDE);
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorObject::OnFocus");
		return true;
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
	
	vector GetMarkerPosition()
	{
		float x, y;
		m_EditorObjectMarkerPanel.GetPos(x, y);
		return Vector(x, y, 0);
	}
}

