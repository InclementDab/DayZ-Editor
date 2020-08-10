class EditorListItemTooltip extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_Title;
	private ItemPreviewWidget m_Preview;

	private ref Timer	m_ToolTipUpdateTimer;
	private ref EditorPlaceableObject m_PlaceableObject;
	private EntityAI m_PreviewObj;
	
	void EditorListItemTooltip(EditorPlaceableObject obj)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::EditorListItemTooltip - Start");
		#endif
		
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorListItemTooltip.layout") );
		m_Title = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemTooltipTitle"));
		m_Preview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("EditorListItemTooltipPreview"));

		m_PlaceableObject = obj;
		
		m_Root.SetHandler( this );
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::EditorListItemTooltip - End");
		#endif
	}
	
	void ~EditorListItemTooltip()
	{
		HideTooltip();
		delete m_Root;
	}
		
	void SetToolTip()
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::SetToolTip - Start");
		#endif
		
		if(m_PlaceableObject)
		{
			m_Title.SetText(m_PlaceableObject.GetType());
			m_PreviewObj = EntityAI.Cast(GetGame().CreateObjectEx(m_PlaceableObject.GetType(), vector.Zero, ECE_NONE));
			
			#ifdef EDITORPRINT
			EditorPrint("EditorListItemTooltip::SetToolTip - m_PreviewObj: " + m_PreviewObj);
			#endif
			
			m_Preview.SetItem(m_PreviewObj);
			m_Preview.SetView(m_Preview.GetItem().GetViewIndex());
		}
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItemTooltip::SetToolTip - End");
		#endif
	}
	
	void SetPos(int x, int y)
	{
		m_Root.SetPos(x, y);
	}
		
	void ShowTooltip()
	{		
		if (!m_ToolTipUpdateTimer)
		{
			m_ToolTipUpdateTimer = new Timer();
			m_ToolTipUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds
		}
		
		m_Root.Show( true );
		
		SetToolTip();
		
		int x, y;
		GetMousePos(x,y);
		m_Root.SetPos(x, y);
		
		m_Root.Update();
	}
		
	void HideTooltip()
	{		
		if (m_ToolTipUpdateTimer)
			m_ToolTipUpdateTimer.Stop();
		
		m_Root.Show(false);
		m_Root.SetPos(0, 0);
		//m_Title.SetText("");
		
		if(m_PreviewObj)
		{
			m_Preview.SetItem(null);
			GetGame().ObjectDelete(m_PreviewObj);
		}
	}
	
	void SetColor(int color)
	{		
		m_Root.SetColor( color );
	}
	
	void SetTextColor(int color)
	{		
		m_Title.SetColor( color );
	}
	
	void Update( float timeslice )
	{		
		if ( m_Root && m_Root.IsVisible() ) 
			UpdateTooltip();
	}
	
	void UpdateTooltip()
	{
		int mouse_x;
		int mouse_y;
			
		GetGame().GetMousePos( mouse_x, mouse_y );
		m_Root.SetPos( mouse_x, mouse_y );
		m_Root.Update();
	}
	
	bool IsVisable()
	{
		return m_Root.IsVisible();
	}
}


class EditorListItem: UILinkedObject
{
	private Widget						m_EditorListItemPanel;
	private ImageWidget					m_EditorListItemIcon;
	private TextWidget 					m_EditorListItemText;
	private ref EditorPlaceableObject 	m_PlaceableObject;
	
	private static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	private static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);
	
	
	
	
	private ref EditorListItemTooltip m_Tooltip;
	
	// Getters
	EditorPlaceableObject GetPlaceableObject() { return m_PlaceableObject; }
	override void OnWidgetScriptInit(Widget w)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnWidgetScriptInit - Start");
		#endif
		
		super.OnWidgetScriptInit(w);
		m_EditorListItemPanel = Widget.Cast(m_Root.FindAnyWidget("EditorListItemPanel"));
		m_EditorListItemIcon = ImageWidget.Cast(m_Root.FindAnyWidget("EditorListItemIcon"));
		m_EditorListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnWidgetScriptInit - End");
		#endif
	}
	
	void SetObject(EditorPlaceableObject target)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::SetObject - Start");
		#endif
		
		m_PlaceableObject = target;
		m_EditorListItemText.SetText(m_PlaceableObject.GetType());
		GetIcon();
		m_EditorListItemText.Update();
		
		m_Tooltip = new EditorListItemTooltip(m_PlaceableObject);
		
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::SetObject - End");
		#endif
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnMouseButtonDown - Start");
		#endif
		
		if (button == 0) {
			//if (w == GetFocus()) return true;
			if (GetEditor().IsPlacing()) {
				delete Editor.ObjectInHand;
			}
			
			SetFocus(w);
			return true;
		} else if (button == 1) {
			
			if (GetGame().GetInput().LocalValue("UAWalkRunTemp")) {
				
				// all very temporary please abstract elsewhere
				if (GetEditor().IsLootEditActive()) {
					GetEditor().PlaceholderRemoveLootMode();
				} else {
					GetEditor().PlaceholderForEditLootSpawns(m_PlaceableObject.GetType());
				}
				
				return true;
				
			}
		}
		
		return false;
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnFocus - Start");
		#endif
		
		if (w == m_EditorListItemPanel) {
			m_EditorListItemPanel.SetColor(COLOR_ON_SELECTED);
			GetEditor().CreateObjectInHand(m_PlaceableObject.GetType());	
			return true;
		}
		
		return false;
	}	
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		EditorPrint("EditorListItem::OnFocusLost");
		if (w == m_EditorListItemPanel) {
			m_EditorListItemPanel.SetColor(COLOR_ON_DESELECTED);
			return true;
		}
		
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnMouseEnter - Start");
		#endif
		
		if (w == m_EditorListItemPanel)
		{
			m_EditorListItemPanel.SetColor(COLOR_ON_SELECTED);
			m_Tooltip.ShowTooltip();
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::OnMouseLeave - Start");
		#endif
		
		if (w == m_EditorListItemPanel)
		{
			if (GetFocus() != w) {
				m_EditorListItemPanel.SetColor(COLOR_ON_DESELECTED);
			}
			
			m_Tooltip.HideTooltip();
			return true;
		}
		return false;
	}
	
	private void GetIcon()
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorListItem::GetIcon - Start");
		EditorPrint("EditorListItem::GetIcon - m_PlaceableObject: " + m_PlaceableObject.Type());
		#endif
		
		string path;
		if(m_PlaceableObject.IsInherited(BuildingBase))
		{
			path = "set:dayz_editor_gui image:building_icon";
		} else if(m_PlaceableObject.IsInherited(PlantSuper))
		{
			path = "set:dayz_editor_gui image:tree_icon";
		}
		else {path = "DayZEditor/gui/images/dayz_editor_icon_black.edds"}
		
		SetIcon(path);
	}
	
	void SetIcon(string path)
	{
		m_EditorListItemIcon.LoadImageFile(0, path);
		m_EditorListItemIcon.SetImage(0);
		m_EditorListItemIcon.Update();
	}
}