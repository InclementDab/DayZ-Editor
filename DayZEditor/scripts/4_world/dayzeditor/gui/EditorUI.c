
// make it so you can search for items by mod name with @ModNameHere

enum EditorCursor
{
	DEFAULT,
	HORIZONTAL_SCROLL
}

// START USING Hud CLASS NOT UISCRIPTEDMENU
class EditorUI: UIScriptedMenu
{
	// Root
	private ref Widget m_Root;
	Widget GetRoot() { return m_Root; }
	
	// Canvas
	protected ref CanvasWidget m_EditorCanvas;
	CanvasWidget GetCanvas() { return m_EditorCanvas; }
	
	// Center Space Widget
	protected Widget m_CenterSpaceFrame;
	
	// Buttons 
	protected ButtonWidget m_LeftbarHide;
	protected ButtonWidget m_RightbarHide;
		
	// Toolbar Buttons
	protected ButtonWidget 	m_UndoButton;
	protected ButtonWidget 	m_RedoButton;
	protected ButtonWidget 	m_MagnetButton;
	protected ButtonWidget 	m_GroundButton;
	protected ButtonWidget 	m_SnapButton;
	protected ButtonWidget 	m_SimcityButton;
	protected SliderWidget 	m_SimcityRadiusSlider;
	protected TextWidget	m_SimcityRadiusText;	
	protected SliderWidget 	m_SimcityDensitySlider;
	protected TextWidget	m_SimcityDensityText;
	protected XComboBoxWidget	m_BrushTypeBox;
	
	// Notifications
	protected Widget m_NotificationFrame;

	
	// Frames and Hosts
	protected Widget m_LeftbarFrame;
	protected Widget m_RightbarFrame;
	protected Widget m_LeftbarPanelHost;
	protected Widget m_RightbarPanelHost;
	
	protected Widget m_SimcityRadiusFrame;
	protected Widget m_SimcityDensityFrame;
	

	// Misc
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	

	// Spacers for Item Lists
	protected WrapSpacerWidget 	m_LeftbarSpacer;
	protected WrapSpacerWidget 	m_RightbarSpacer;
	protected ScrollWidget		m_LeftbarScroll;
		
	// Cursors
	protected Widget			m_HorizontalScrollWidget;


	
	// Orientation Tool
	protected ItemPreviewWidget m_OrientationWidget;
	
	TextListboxWidget 	m_DebugActionStack;
	

	
	// Private members
	private bool left_bar_hidden = false;
	private bool right_bar_hidden = false;
	
	// Color management
	private bool m_FocusMagnet = false;
	private bool m_FocusGround = false;
	private bool m_FocusSnap = false;
	
	// Tooltips
	private ref EditorUITooltip m_TooltipMagnet;
	private ref EditorUITooltip m_TooltipSnap;
	private ref EditorUITooltip m_TooltipGround;
	
	// Info toolbar widgets
	protected Widget m_ObjPosInfoPanel;
	
	protected Widget m_CamPosInfoPanel;
	protected TextWidget m_CamPosInfoX;
	protected TextWidget m_CamPosInfoY;
	protected TextWidget m_CamPosInfoZ;

	protected float m_TestVariableX;
	protected float m_TestVariableY;
	protected float m_TestVariableZ;
	
	void EditorUI()
	{
		EditorLog.Trace("EditorUI");
	}
	
	void ~EditorUI()
	{
		EditorLog.Trace("~EditorUI");
	}
	
	
	override Widget Init()
	{
		EditorLog.Trace("EditorUI::Init");
		
		// Init
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorUI.layout");
		
		// Canvas
		m_EditorCanvas			= CanvasWidget.Cast(m_Root.FindAnyWidget("EditorCanvas"));
		
		// Center Space
		m_CenterSpaceFrame		= m_Root.FindAnyWidget("CenterSpaceFrame");
		
		// Frames
		m_LeftbarFrame			= m_Root.FindAnyWidget("LeftbarFrame");
		m_RightbarFrame			= m_Root.FindAnyWidget("RightbarFrame");
		m_LeftbarPanelHost		= m_Root.FindAnyWidget("LeftbarPanelHost");
		m_RightbarPanelHost		= m_Root.FindAnyWidget("RightbarPanelHost");
		m_SimcityRadiusFrame	= m_Root.FindAnyWidget("SimcityRadiusFrame");
		m_SimcityDensityFrame	= m_Root.FindAnyWidget("SimcityDensityFrame");
		
		
		// Orientation Widget
		m_OrientationWidget		= ItemPreviewWidget.Cast(m_Root.FindAnyWidget("OrientationView"));
		EntityAI translate 		= EntityAI.Cast(GetGame().CreateObjectEx("TranslationWidget", vector.Zero, ECE_NONE, RF_FRONT)); // todo 1line
		m_OrientationWidget.SetItem(translate);
		m_OrientationWidget.SetView(0);
		
		// Misc
		m_EditorMapContainer	= m_Root.FindAnyWidget("MapContainer");
		m_EditorMapWidget		= MapWidget.Cast(m_Root.FindAnyWidget("Map"));
		m_LeftbarSearchBar		= EditBoxWidget.Cast(m_Root.FindAnyWidget("LeftbarSearchBar"));
 
		// Buttons
		m_LeftbarHide			= ButtonWidget.Cast(m_Root.FindAnyWidget("LeftbarHide"));
		m_RightbarHide			= ButtonWidget.Cast(m_Root.FindAnyWidget("RightbarHide"));
				
		// Spacers
		m_LeftbarSpacer			= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("LeftbarSpacer"));
		m_RightbarSpacer		= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("RightbarSpacer"));
		m_LeftbarScroll			= ScrollWidget.Cast(m_Root.FindAnyWidget("LeftbarScroll"));
		
		// Toolbar
		m_UndoButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("UndoButton"));
		m_RedoButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("RedoButton"));
		m_MagnetButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("MagnetButton"));
		m_GroundButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("GroundButton"));
		m_SnapButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("SnapButton"));
		m_SimcityButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("SimcityButton"));
		m_SimcityRadiusSlider	= SliderWidget.Cast(m_Root.FindAnyWidget("SimcityRadiusSlider"));
		m_SimcityRadiusText		= TextWidget.Cast(m_Root.FindAnyWidget("SimcityRadiusText"));		
		m_SimcityDensitySlider	= SliderWidget.Cast(m_Root.FindAnyWidget("SimcityDensitySlider"));
		m_SimcityDensityText	= TextWidget.Cast(m_Root.FindAnyWidget("SimcityDensityText"));
		m_BrushTypeBox			= XComboBoxWidget.Cast(m_Root.FindAnyWidget("BrushTypeBox"));
	
		m_NotificationFrame		= m_Root.FindAnyWidget("NotificationFrame");
		m_NotificationFrame.GetPos(notification_start_x, notification_start_y);
		
		// why aint we usin scriptclass?
		m_EditorMapWidget.GetScript(m_EditorMap);
		m_EditorMapWidget.SetMapPos(GetGame().GetCurrentCameraPosition());
		

		// Debug
		m_DebugActionStack		= TextListboxWidget.Cast(m_Root.FindAnyWidget("DebugActionStackListbox"));
		
		// Brush init
		m_SimcityRadiusText.SetText(m_SimcityRadiusSlider.GetCurrent().ToString());
		m_SimcityDensityText.SetText(m_SimcityDensitySlider.GetCurrent().ToString());
		
		// Events
		// this will go into ViewModel
		//EditorEvents.OnPlaceableCategoryChanged.Insert(OnPlaceableCategoryChanged);
			
		// Tooltips
		m_TooltipMagnet 	= new EditorUITooltip("Toggle magnet mode on/off.", "Magnet Mode");
		m_TooltipSnap 		= new EditorUITooltip("Toggle snap mode on/off.", "Snap Mode");
		m_TooltipGround 	= new EditorUITooltip("Toggle ground mode on/off.", "Ground Mode");
		
		// Info toolbar widgets
		m_ObjPosInfoPanel 	= m_Root.FindAnyWidget("InfobarObjPosFrame");		
		
		
		m_CamPosInfoPanel	= m_Root.FindAnyWidget("InfobarCamPosFrame");
		m_CamPosInfoX 		= TextWidget.Cast(m_Root.FindAnyWidget("Info_CamPos_X_Value"));
		m_CamPosInfoY 		= TextWidget.Cast(m_Root.FindAnyWidget("Info_CamPos_Y_Value"));
		m_CamPosInfoZ 		= TextWidget.Cast(m_Root.FindAnyWidget("Info_CamPos_Z_Value"));
		
		
		
		
		return m_Root;
	}
	
	void Show(bool state)
	{
		m_Root.Show(state);
	}
	
	
	override void Update(float timeslice)
	{
		//Print("Test Vector: " + m_TestVariableX + ", " + m_TestVariableY + ", " + m_TestVariableZ);
		
		super.Update(timeslice);
				
		if (m_LeftbarScroll.GetVScrollPos() > m_LeftbarScroll.GetContentHeight())
			m_LeftbarScroll.VScrollToPos(0);
		
		// every frame? monkaS
		// todo switch this to events save some frames
		RefreshToolbarButtonsState();
		
		if (m_CamPosInfoPanel.IsVisible())
			UpdateInfoCamPos();
		
		
		vector orientation = GetEditor().GetCamera().GetOrientation();
		m_OrientationWidget.SetModelOrientation(Vector(orientation[1], orientation[0], orientation[2]));
			
	}
	
	private bool cursor_state;
	void ShowMap(bool state)
	{
		EditorLog.Trace("EditorUI::ShowMap");			
		
		if (state) {
			cursor_state = GetGame().GetUIManager().IsCursorVisible();
			m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() * 3);
			m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() * 3);
			GetGame().GetUIManager().ShowUICursor(true);
		} else {
			GetGame().GetUIManager().ShowUICursor(cursor_state);
			m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() / 3);
			m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() / 3);
		}
		
		m_EditorMapContainer.Show(state);
		m_EditorMapContainer.Update();
		
	}
		
	
	void CreateDialog()
	{
		// todo use editor UI manager to do ShowDialog
		//GetGame().GetUIManager().ShowDialog("Overwrite", "Do you really want to Overwrite?", 169, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
	}
	
	EditorMap GetMap() 			{ return m_EditorMap; }
	MapWidget GetMapWidget() 	{ return m_EditorMapWidget; }
	bool IsMapOpen() 			{ return m_EditorMapContainer.IsVisible(); }
		
	void SetCursor(EditorCursor cursor_type = EditorCursor.DEFAULT)
	{
		switch (cursor_type) {
			
			case EditorCursor.DEFAULT: {
				//SetCursorWidget(null);
				//ShowCursorWidget(true);
				break;
				
			}
			
			case EditorCursor.HORIZONTAL_SCROLL: {
				//ShowCursorWidget(false);
				//SetCursorWidget(m_HorizontalScrollWidget);
				
				break;
			}			
		}
	}
	
	

	void InsertPlaceableObject(EditorListItem target)
	{
		m_LeftbarSpacer.AddChild(target.GetRoot());
	}
	
	
	void InsertPlacedObject(EditorListItem target)
	{
		m_RightbarSpacer.AddChild(target.GetRoot());
	}
	
	void InsertMapObject(Widget map_marker)
	{
		Print("EditorUI::InsertMapObject " + map_marker.GetName());
		m_EditorMapWidget.AddChild(map_marker);
	}
	
	
	
	
	
	/* Events */
	override bool OnClick(Widget w, int x, int y, int button) 
	{
		EditorLog.Trace("EditorUI::OnClick");
		
		if (button == 0) {
			
			switch (w) {
				
				case m_MagnetButton: {
					EditorSettings.MAGNET_PLACEMENT = m_MagnetButton.GetState();
					return true;
				}
				
				case m_GroundButton: {
					EditorSettings.MAINTAIN_HEIGHT = m_GroundButton.GetState();
					return true;
				}
				
				case m_SnapButton: {
					EditorSettings.SNAPPING_MODE = m_SnapButton.GetState();
					return true;
				}
				
				case m_SimcityButton: {
					string brush_name = m_CurrentBrushNames.Get(m_BrushTypeBox.GetCurrentItem());
					if (!m_SimcityButton.GetState()) {
						GetEditor().SetBrush(null);
					} else {
						GetEditor().SetBrush(GetEditor().GetSettings().GetBrush(brush_name));
					}
					return true;
				}
				
				case m_BrushTypeBox: {
					brush_name = m_CurrentBrushNames.Get(m_BrushTypeBox.GetCurrentItem());
					GetEditor().SetBrush(GetEditor().GetSettings().GetBrush(brush_name));
					return true;
				}
				
				// todo resize m_CenterSpaceFrame when collapsed
				case m_LeftbarHide: {
					left_bar_hidden = !left_bar_hidden;
					m_LeftbarFrame.SetPos(-300 * left_bar_hidden, 0);
					return true;
				} 
				
				case m_RightbarHide: {
					right_bar_hidden = !right_bar_hidden;
					m_RightbarFrame.SetPos(-300 * right_bar_hidden, 48);
					return true;
				}
								
				default: {
					Print(string.Format("%1 Doesnt have a click function!", w.GetName()));
				}
			}
		}
		
		return super.OnClick(w, x, y, button);
		
	}
	
	
	private ref array<ref EditorCollapsibleListItem> test_objects = new array<ref EditorCollapsibleListItem>();
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorUI::OnMouseButtonDown");
		
		Input input = GetGame().GetInput();	
		// Left Click
		if (button == 0) {
			//SetFocus(null);
			if (GetEditor().IsPlacing()) {
				GetEditor().PlaceObject();
				return true;
			} 
			
			// Raycast to see if TranslationWidget is under cursor			
			RaycastRVParams raycast_params = new RaycastRVParams(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * EditorSettings.OBJECT_VIEW_DISTANCE);
			ref array<ref RaycastRVResult> raycast_result = new array<ref RaycastRVResult>();
			DayZPhysics.RaycastRVProxy(raycast_params, raycast_result);
			
/*
			if (raycast_result.Count() > 0) {
				Object obj = raycast_result.Get(0).obj;
				if ((obj.GetType() == "TranslationWidget" || obj.GetType() == "RotationWidget")) {
					EditorEvents.DragInvoke(obj, GetEditor().GetTranslationWidget().GetEditorObject(), raycast_result.Get(0));
					return true;
				}
				
				EditorObject editor_object = GetEditor().GetObjectManager().GetEditorObject(obj);
				if (editor_object != null) {
					if (input.LocalValue("UAWalkRunTemp")) {
						EditorObjectManager.ToggleSelection(editor_object);
					} else if (!input.LocalValue("UATurbo")) {
						EditorEvents.ClearSelection(this);
					} else EditorEvents.SelectObject(this, editor_object);
					return true;
				}
			}
			*/
			
			GetEditor().ClearSelection();
			if (GetEditor().GetBrush() == null) {
				
				if (GetEditor().EditorObjectUnderCursor == null) {
					// delayed dragbox					
					GetCursorPos(start_x, start_y);
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragBoxCheck, 60);
					return true;
					
					
				} else if (GetEditor().EditorObjectUnderCursor != null) {
					if (!input.LocalValue("UATurbo")) {
						GetEditor().ClearSelection();
					}
					GetEditor().SelectObject(GetEditor().EditorObjectUnderCursor);
					return true;
				}
			}
			
			
		}
		
		// Right mouse
		if (button == 1) {
			if (w.GetName() == "RightbarScroll") {
				
				EditorCollapsibleListItem t = new EditorCollapsibleListItem();
				test_objects.Insert(t);
				InsertPlacedObject(t);
				
			}
			return true;
		}
		
		// Middle Mouse
		if (button == 2) {
			if (input.LocalValue("UAWalkRunTemp")) {
				set<Object> o;
				vector pos = MousePosToRay(o);
				LightingBolt.CreateLightning(pos, 5);
			} else {
				pos = MousePosToRay(o);
				pos[1] = GetEditor().GetCamera().GetPosition()[1];
				GetEditor().GetCamera().SetPosition(pos);
			}
		}
				
		return false;
	}
		

		
	ScriptInvoker DragBoxQueue = GetGame().GetUpdateQueue(CALL_CATEGORY_GUI);
	int start_x, start_y;
	void DelayedDragBoxCheck()
	{
		if (GetGame().GetInput().LocalValue("UAFire"))
			DragBoxQueue.Insert(UpdateDragBox);
	}
	
	void UpdateDragBox()
	{	
		int current_x, current_y;
		GetCursorPos(current_x, current_y);
		m_EditorCanvas.Clear();
		int selection_box_thickness = 2;
		int selection_box_color = ARGB(100, 255, 0, 0);
		m_EditorCanvas.DrawLine(start_x, start_y, current_x, start_y, selection_box_thickness, selection_box_color);
		m_EditorCanvas.DrawLine(start_x, start_y, start_x, current_y, selection_box_thickness, selection_box_color);
		m_EditorCanvas.DrawLine(start_x, current_y, current_x, current_y, selection_box_thickness, selection_box_color);
		m_EditorCanvas.DrawLine(current_x, start_y, current_x, current_y, selection_box_thickness, selection_box_color);
		
	
		if (GetGame().GetInput().LocalRelease("UAFire")) {
			m_EditorCanvas.Clear();
			DragBoxQueue.Remove(UpdateDragBox);
			
			int x_low, x_high, y_low, y_high;
			if (start_x > current_x) {
				x_high = start_x;
				x_low = current_x;
			} else { 
				x_high = current_x;
				x_low = start_x;
			}
			
			if (start_y > current_y) {
				y_high = start_y;
				y_low = current_y;
			} else { 
				y_high = current_y;
				y_low = start_y;
			}
			
			ref EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
			foreach (EditorObject editor_object: placed_objects) {
				
				float marker_x, marker_y;
				if (IsMapOpen()) {
					editor_object.GetMapMarkerPos(marker_x, marker_y);
				} else {
					editor_object.GetObjectMarkerPos(marker_x, marker_y);
				}
				
				
				if ((marker_x < x_high && marker_x > x_low) && (marker_y < y_high && marker_y > y_low)) {		
					GetEditor().SelectObject(editor_object);
				}
			}
			
			return;
		}
		


	}
	
	
	private ref array<string> m_CurrentBrushNames = new array<string>();
	void ClearBrushBox()
	{
		m_CurrentBrushNames.Clear();
		m_BrushTypeBox.ClearAll();
	}
	
	void InsertBrush(string name)
	{
		m_CurrentBrushNames.Insert(name);
		m_BrushTypeBox.AddItem(name);
	}
	
	

	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		EditorLog.Trace("EditorUI::OnMouseEnter");
		
		if( w == m_UndoButton || w == m_RedoButton )
		{
			ColorBlue( w, x, y );
			return true;
		} else if( w == m_MagnetButton )
		{
			m_FocusMagnet = true;
			ColorRed( w, x, y );
			m_TooltipMagnet.ShowTooltip();
			return true;
		} else if( w == m_GroundButton )
		{
			m_FocusGround = true;
			ColorPureApple( w, x, y );
			m_TooltipGround.ShowTooltip();
			return true;
		} else if( w == m_SnapButton )
		{
			m_FocusSnap = true;
			ColorQuinceJelly( w, x, y );
			m_TooltipSnap.ShowTooltip();
			return true;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		EditorLog.Trace("EditorUI::OnMouseLeave");
		
		
		if( w == m_UndoButton || w == m_RedoButton )
		{
			ColorNoFocus( w, enterW, x, y );
			return true;
		} else if( w == m_MagnetButton )
		{
			m_FocusMagnet = false;
			ColorNoFocus( w, enterW, x, y );
			m_TooltipMagnet.HideTooltip();
			return true;
		} else if( w == m_GroundButton )
		{
			m_FocusGround = false;
			ColorNoFocus( w, enterW, x, y );
			m_TooltipGround.HideTooltip();
			return true;
		} else if( w == m_SnapButton )
		{
			m_FocusSnap = false;
			ColorNoFocus( w, enterW, x, y );
			m_TooltipSnap.HideTooltip();
			return true;
		}
		
		return false;
	}
	
	void ColorBlue( Widget w, int x, int y )
	{		
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_Icon" ) );
		
		if( image )
		{
			image.SetColor( ARGB( 255, 52, 152, 219 ) );
		}
	}
	
	void ColorRed( Widget w, int x, int y )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_Icon" ) );
		
		if( image )
		{
			image.SetColor( ARGB( 255, 231, 76, 60 ) );
		}
	}
	
	void ColorPureApple( Widget w, int x, int y )
	{		
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_Icon" ) );
		
		if( image )
		{
			image.SetColor( ARGB( 255, 106, 176, 76 ) );
		}
	}
	
	void ColorQuinceJelly( Widget w, int x, int y )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetColor( ARGB( 255, 0, 0, 0 ) );
		}		
		
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_Icon" ) );
		
		if( image )
		{
			image.SetColor( ARGB( 255, 240, 147, 43 ) );
		}
		
		string name = w.GetName();
		if( name.Contains("Magnet") )
		
	}
	
	void ColorNoFocus( Widget w, Widget enterW, int x, int y )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetColor( ARGB( 0, 0, 0, 0 ) );
		}
		
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_Icon" ) );
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
	}

	void RefreshToolbarButtonsState()
	{
		// Settings Management
		m_GroundButton.SetState(EditorSettings.MAINTAIN_HEIGHT);
		m_MagnetButton.SetState(EditorSettings.MAGNET_PLACEMENT);
		
		
		// Brush Management
		bool brush_enable = m_SimcityButton.GetState();		
		m_SimcityRadiusFrame.Show(brush_enable);
		m_SimcityDensityFrame.Show(brush_enable);

		EditorBrush.SetRadius(m_SimcityRadiusSlider.GetCurrent());
		EditorBrush.SetDensity(m_SimcityDensitySlider.GetCurrent());
				
		m_SimcityRadiusText.SetText(m_SimcityRadiusSlider.GetCurrent().ToString());
		m_SimcityDensityText.SetText(m_SimcityDensitySlider.GetCurrent().ToString());
		
		
		
		//! Color managment
		if (m_MagnetButton.GetState()) {
			ColorRed( m_MagnetButton, 0, 0 );
		} else if ( !m_MagnetButton.GetState() && !m_FocusMagnet )
		{
			ColorNoFocus( m_MagnetButton, null, 0, 0 );
		}
		
		if (m_GroundButton.GetState()) {
			ColorPureApple( m_GroundButton, 0, 0 );
		} else if ( !m_GroundButton.GetState() && !m_FocusGround)
		{
			ColorNoFocus( m_GroundButton, null, 0, 0 );
		}
		
		if ( m_SnapButton.GetState() )
		{
			ColorQuinceJelly( m_SnapButton, 0, 0 );
		} else if ( !m_SnapButton.GetState() && !m_FocusSnap )
		{
			ColorNoFocus( m_SnapButton, null, 0, 0 );
		}
	}
	
	private void UpdateInfoCamPos()
	{
		vector campos = GetGame().GetCurrentCameraPosition();
		m_CamPosInfoX.SetText(campos[0].ToString());
		m_CamPosInfoY.SetText(campos[1].ToString());
		m_CamPosInfoZ.SetText(campos[2].ToString());
	}
	
	void ShowObjPosInfoPanel(bool state)
	{
		m_ObjPosInfoPanel.Show(state);
	}
	
	
	/// stuff from manager
		
	static void ShowCursor()
	{
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	static void HideCursor()
	{
		GetGame().GetUIManager().ShowUICursor(false);
	}
	
	static void ToggleCursor()
	{
		GetGame().GetUIManager().ShowUICursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	bool IsCursorOverUI()
	{
		float pos_x, pos_y, size_x, size_y;
		
		m_CenterSpaceFrame.GetScreenPos(pos_x, pos_y);
		m_CenterSpaceFrame.GetScreenSize(size_x, size_y);
		
		int cursor_x, cursor_y;
		GetCursorPos(cursor_x, cursor_y);
		
		return (cursor_x > size_x + pos_x) || (cursor_x < pos_x) || (cursor_y > size_y + pos_y) || (cursor_y < pos_y);
	}
	
	private float notification_start_x, notification_start_y;
	void NotificationCreate(string text, int color = -4301218, float duration = 4)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationDestroy);
		
		Widget notif_panel = m_NotificationFrame.FindAnyWidget("NotificationPanel");
		TextWidget notif_text = TextWidget.Cast(m_NotificationFrame.FindAnyWidget("NotificationText"));
		
		notif_panel.SetColor(color);
		notif_text.SetText(text);
		m_NotificationFrame.Show(true);
			
		float width, height;
		m_NotificationFrame.GetSize(width, height);
		
		EffectSound notif_sound = SEffectManager.PlaySound("Notification_SoundSet", GetEditor().GetCamera().GetPosition());
		notif_sound.SetSoundAutodestroy(true);
		
		// Animate pulldown
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, m_NotificationFrame, GetGame().GetTime(), 0.25, notification_start_x, notification_start_x, notification_start_y, notification_start_y + height);
		
		// Call destroy after duration done
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationDestroy, duration * 1000, false, m_NotificationFrame, notification_start_x, notification_start_y);
	}
	
	
	
	
	private void NotificationAnimateFrame(Widget root, float anim_starttime, float duration, float start_x, float final_x, float start_y, float final_y)
	{
				
		float anim_frametime = GetGame().GetTime() - anim_starttime;
		anim_frametime /= 1000;
		
		float normalized_time = (1 / duration) * anim_frametime;
		normalized_time = Math.Clamp(normalized_time, 0, 1);
		
		float pos_x = Math.Lerp(start_x, final_x, normalized_time);
		float pos_y = Math.Lerp(start_y, final_y, normalized_time);
		
		root.SetPos(pos_x, pos_y);
		
		if (normalized_time >= 1)
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(NotificationAnimateFrame);
		
		
	}
	
	private void NotificationDestroy(Widget root, float start_x, float start_y)
	{
		float current_x, current_y;
		root.GetPos(current_x, current_y);
		
		float duration = 0.25;
		// Animate in reverse
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(NotificationAnimateFrame, 0, true, root, GetGame().GetTime(), duration, current_x, start_x, current_y, start_y);
		
		// Hide Object
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(root.Show, duration * 1000, false, false);
	}
	
	
	// Modal Window Control
	private static ref EditorDialog m_CurrentModal;
	static void ModalSet(EditorDialog w)
	{
		Print("ModalSet");
		m_CurrentModal = w;
		SetModal(m_CurrentModal.GetRoot());
		ShowCursor();
	}
	
	static void ModalClose()
	{
		Print("ModalClose");
		m_CurrentModal.GetRoot().Unlink();
		m_CurrentModal = null;
		ShowCursor();
	}

	bool IsModalActive()
	{
		return GetGame().GetUIManager().IsModalVisible();
	}
}
