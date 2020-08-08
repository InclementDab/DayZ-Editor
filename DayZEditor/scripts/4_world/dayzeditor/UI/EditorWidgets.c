
class EditorContextMenu: UILinkedObject
{
	
	void EditorContextMenu()
	{
		Print("EditorContextMenu");
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (w.GetName()) {
			
			
			case "SetTargetButton": {
				Editor.SetCameraTarget(m_EditorObject);
				m_Root.Show(false);
				return true;
			}
			
		}
		
		
		return false;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		m_Root.Show(false);
		return true;
	}
	
}



class EditorMap: EditorWidgetEventHandler
{

	void EditorMap()
	{
		Print("EditorMap");
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
	}
	
	void ~EditorMap()
	{
		Print("~EditorMap");
	}
	
	void OnObjectCreated(Class context, EditorObject obj)
	{
		MapWidget map_widget = GetMapWidget();
		map_widget.AddChild(obj.GetMapMarker());
	}
	
	
	private int start_x, start_y;
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Input input = GetGame().GetInput();	
		MapWidget map_widget = GetMapWidget();
		
		if (button == 0) {
			if (GetEditor().IsPlacing()) {
				EntityAI e = Editor.ObjectInHand.GetProjectionEntity();
				vector mat[4];
				e.GetTransform(mat);
				EditorObject editor_object = GetEditor().GetObjectManager().CreateObject(e.GetType(), mat[3]);
				editor_object.SetTransform(mat);
				if (!input.LocalValue("UATurbo")) delete Editor.ObjectInHand;
				return true;
			} else {
				EditorUI ui = GetEditor().GetUIManager().GetEditorUI();
				ui.GetCanvas().Clear();
				GetCursorPos(ui.start_x, ui.start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ui.DelayedDragBoxCheck, 40);
				return true;
			}
			
		} else if (button == 2) {
			vector teleport_dest = map_widget.ScreenToMap(Vector(x, y, 0));
			vector current_pos = GetEditor().GetUIManager().GetEditorCamera().GetPosition();
			teleport_dest[1] = current_pos[1] - GetGame().SurfaceY(current_pos[0], current_pos[2]) + GetGame().SurfaceY(teleport_dest[0], teleport_dest[2]);
			GetEditor().GetUIManager().GetEditorCamera().SetPosition(teleport_dest);			
			
			return true;
			
		}
		
		return false;
	}
	
	MapWidget GetMapWidget() { return MapWidget.Cast(m_Root); }
}



class EditorListItem: EditorWidgetEventHandler
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
		
		if (w == m_EditorListItemPanel)
		{
			GetEditor().CreateObjectInHand(m_PlaceableObject.GetType());	
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
			m_EditorListItemPanel.SetColor(COLOR_ON_DESELECTED);
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
	
	private void SetIcon(string path)
	{
		m_EditorListItemIcon.LoadImageFile(0, path);
		m_EditorListItemIcon.SetImage(0);
	}
}

class EditorPlacedListItem: UILinkedObject
{
	private ImageWidget		m_EditorPlacedListItemIcon;
	protected TextWidget 	m_EditorPlacedListItemText;	
	protected Widget 		m_EditorPlacedListItemPanel;	
	
	private static int COLOR_ON_SELECTED = ARGB(140,41,128,185);
	private static int COLOR_ON_DESELECTED = ARGB(140,35,35,35);
	
	
	void ~EditorPlacedListItem()
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::~EditorPlacedListItem - Start");
		#endif
		
		delete m_EditorPlacedListItemText;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::~EditorPlacedListItem - End");
		#endif
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::OnWidgetScriptInit - Start");
		#endif
		
		super.OnWidgetScriptInit(w);
		
		m_EditorPlacedListItemPanel = m_Root.FindAnyWidget("EditorPlacedListItemPanel");
		m_EditorPlacedListItemText = TextWidget.Cast(m_Root.FindAnyWidget("EditorPlacedListItemText"));
		
		EditorEvents.OnObjectSelected.Insert(EditorObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(EditorObjectDeselected);
		//GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::OnWidgetScriptInit - End");
		#endif
	}
	
	void EditorObjectSelected(Class context, EditorObject target)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::EditorObjectSelected - Start");
		#endif
		
		m_EditorPlacedListItemPanel.SetColor(COLOR_ON_SELECTED);
		m_EditorPlacedListItemPanel.Update();
		
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::EditorObjectSelected - End");
		#endif
	}
	
	void EditorObjectDeselected(Class context, EditorObject target)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::EditorObjectDeselected - Start");
		#endif
		
		m_EditorPlacedListItemPanel.SetColor(COLOR_ON_DESELECTED); 
		m_EditorPlacedListItemPanel.Update();
		
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::EditorObjectDeselected - End");
		#endif
	}
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::OnClick - Start");
		#endif
		
		Input input = GetGame().GetInput();
		EditorObject editor_object = EditorObject.GetFromUILinkedRoot(m_Root);
		// LMB
		/*
		if (button == 0) {
			
			// If Holding Shift
			if (input.LocalValue("UATurbo")) {
				
				// If root object is already selected
				if (GetEditor().GetObjectManager().IsSelected(editor_object)) {
					GetEditor().GetObjectManager().DeselectObject(m_EditorObject);
					return true;
				}
				
				GetEditor().GetObjectManager().SelectObject(m_EditorObject, false);
				if (GetEditor().GetObjectManager().GetSelectedObjects().Count() != 0) {
					Widget root_object = m_Root.GetParent().GetChildren();
					bool selection_found = GetEditor().GetObjectManager().CheckIfRootIsSelected(root_object);
					
					// Search down the browser for first selected object
					while (!selection_found) {
						root_object = root_object.GetSibling();
						selection_found = GetEditor().GetObjectManager().CheckIfRootIsSelected(root_object);
						if (root_object == null) break;	
					}
										
					// Search until last selected object
					while (selection_found) {
						GetEditor().GetObjectManager().SelectObject(EditorObject.GetFromUILinkedRoot(root_object), false);
						root_object = root_object.GetSibling();
						selection_found = !GetEditor().GetObjectManager().CheckIfRootIsSelected(root_object);
						if (root_object == null) break;
					}
					return true;		
				}			
				
			} else if (input.LocalValue("UAWalkRunTemp")) {
				GetEditor().GetObjectManager().ToggleSelection(m_EditorObject);		
			} else {
				GetEditor().GetObjectManager().SelectObject(m_EditorObject);
			}
		}
		*/
		
		
		return super.OnClick(w, x, y, button);
		
	}
	

	override bool OnFocus(Widget w, int x, int y)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::OnFocus - Start");
		EditorPrint("EditorPlacedListItem::OnFocus - m_EditorObject: " + m_EditorObject);
		#endif
		
		if (w == m_EditorPlacedListItemText)
		{
			GetEditor().GetObjectManager().SelectObject(m_EditorObject, false);
			return true;
		}
		
		return false;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::OnFocusLost - Start");
		#endif
		
		if (w == m_EditorPlacedListItemText)
		{
			GetEditor().GetObjectManager().DeselectObject(m_EditorObject);
			return true;
		}
		
		return false;
	}
	
	

	
	override void SetObject(notnull EditorObject target)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::SetObject - Start");
		#endif
		
		super.SetObject(target);
		m_EditorPlacedListItemText.SetText(target.GetType());
		m_EditorPlacedListItemText.Update();
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::OnMouseEnter - Start");
		#endif
		
		if (w == m_EditorPlacedListItemText)
		{
			m_EditorPlacedListItemText.SetColor(COLOR_ON_SELECTED);
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorPlacedListItem::OnMouseLeave - Start");
		#endif
		
		if (w == m_EditorPlacedListItemText)
		{
			m_EditorPlacedListItemText.SetColor(COLOR_ON_DESELECTED);
			return true;
		}
		return false;
	}
}



class MenuBarFile: EditorWidgetEventHandler
{
	protected WrapSpacerWidget m_FileListWrapSpacer;
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("MenuBarFile::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		m_FileListWrapSpacer = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("FileListWrapSpacer"));
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_Root)
			m_FileListWrapSpacer.Show(true);
		
		// todo handle new, open etc...
		
		return true;
	}
}


class PlaceableSearchBar: EditorWidgetEventHandler
{

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		Print("PlaceableSearchBar::OnChange");
		
		
		WrapSpacerWidget spacer = WrapSpacerWidget.Cast(GetEditor().GetUIManager().GetEditorUI().GetRoot().FindAnyWidget("LeftbarSpacer"));
		string filter = EditBoxWidget.Cast(w).GetText();
		Widget child = spacer.GetChildren();
		while (child != null) {
			
			EditorListItem editor_list_item;
			child.GetScript(editor_list_item);
			
			if (editor_list_item != null) {
				string Config_Lower = editor_list_item.GetPlaceableObject().GetType();
				Config_Lower.ToLower();
				if (filter == "") return false;
	           	child.Show(Config_Lower.Contains(filter));				
			}
						
			child = child.GetSibling();

			
		}
		
		return true;
	}
	
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("PlaceableSearchBar::OnFocus");
		
		GetEditor().GetUIManager().GetEditorCamera().SetMoveEnabled(false);
		
		return super.OnFocus(w, x, y);
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("PlaceableSearchBar::OnFocusLost");
		
		GetEditor().GetUIManager().GetEditorCamera().SetMoveEnabled(true);
		
		return super.OnFocusLost(w, x, y);
	}
	
}

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