




// make it so you can search for items by mod name with @ModNameHere

enum EditorCursor
{
	DEFAULT,
	HORIZONTAL_SCROLL
}


class EditorUI: UIScriptedMenu
{
	private ref Widget m_Root;
	Widget GetRoot() { return m_Root; }
	
	// UIManager
	// Maybe use EditorUIViewModel
	private ref EditorUIManager m_UIManager;
	
	// Canvas
	protected ref CanvasWidget m_EditorCanvas;
	CanvasWidget GetCanvas() { return m_EditorCanvas; }
	
	// Center Space Widget
	protected Widget m_CenterSpaceFrame;
	Widget GetCenterSpaceFrame() { return m_CenterSpaceFrame; }
	
	// Buttons 
	protected ButtonWidget m_BuildingSelect;
	protected ButtonWidget m_VehicleSelect;
	protected ButtonWidget m_EntitySelect;
	protected ButtonWidget m_HumanSelect;
	protected ButtonWidget m_LeftbarHide;
	protected ButtonWidget m_RightbarHide;
	
	protected ButtonWidget m_CategorySelectBuilding;
	protected ButtonWidget m_CategorySelectVehicle;
	protected ButtonWidget m_CategorySelectEntity;
	protected ButtonWidget m_CategorySelectHuman;
	
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
	Widget GetNotificationFrame() { return m_NotificationFrame; }

	
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
	
	// Debug
	private Widget		m_DebugFrame;
	TextWidget 			m_DebugText1;
	TextWidget			m_DebugText2;
	TextWidget 			m_DebugText3;
	TextWidget 			m_DebugText4;
	TextWidget 			m_DebugText5;
	TextWidget 			m_DebugText6;
	TextListboxWidget 	m_DebugActionStack;
	
	
	private ref EditorUIHandler m_EditorUIHandler;
	EditorUIHandler GetHandler() { return m_EditorUIHandler; }
	
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
		EditorPrint("EditorUI");
		m_UIManager = GetEditor().GetUIManager();
	}
	
	void ~EditorUI()
	{
		EditorPrint("~EditorUI");
	}
	
	
	override Widget Init()
	{
		EditorPrint("EditorUI::Init");
		
		
		// Init
		m_EditorUIHandler = new EditorUIHandler();
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorUI.layout");
		m_Root.GetScript(m_EditorUIHandler);
		
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
		m_BuildingSelect		= ButtonWidget.Cast(m_Root.FindAnyWidget("BuildingSelect"));
		m_VehicleSelect			= ButtonWidget.Cast(m_Root.FindAnyWidget("VehicleSelect"));
		m_EntitySelect			= ButtonWidget.Cast(m_Root.FindAnyWidget("EntitySelect"));
		m_HumanSelect			= ButtonWidget.Cast(m_Root.FindAnyWidget("HumanSelect"));
		m_LeftbarHide			= ButtonWidget.Cast(m_Root.FindAnyWidget("LeftbarHide"));
		m_RightbarHide			= ButtonWidget.Cast(m_Root.FindAnyWidget("RightbarHide"));
		
		m_CategorySelectBuilding= ButtonWidget.Cast(m_Root.FindAnyWidget("BuildingSelect"));
		m_CategorySelectVehicle	= ButtonWidget.Cast(m_Root.FindAnyWidget("VehicleSelect"));
		m_CategorySelectEntity	= ButtonWidget.Cast(m_Root.FindAnyWidget("EntitySelect"));
		m_CategorySelectHuman	= ButtonWidget.Cast(m_Root.FindAnyWidget("HumanSelect"));
		
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
		
		m_EditorMapWidget.GetScript(m_EditorMap);
		m_EditorMapWidget.SetMapPos(GetGame().GetCurrentCameraPosition());
		
		// Cursors
		//m_HorizontalScrollWidget = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/cursors/horizontalwidget.layout");
		

		
		// Debug
		m_DebugText1			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText1"));
		m_DebugText2			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText2"));
		m_DebugText3			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText3"));
		m_DebugText4			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText4"));
		m_DebugText5			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText5"));
		m_DebugText6			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText6"));
		m_DebugActionStack		= TextListboxWidget.Cast(m_Root.FindAnyWidget("DebugActionStackListbox"));
		
		// Brush init
		m_SimcityRadiusText.SetText(m_SimcityRadiusSlider.GetCurrent().ToString());
		m_SimcityDensityText.SetText(m_SimcityDensitySlider.GetCurrent().ToString());
		
		// Events
		EditorEvents.OnPlaceableCategoryChanged.Insert(OnPlaceableCategoryChanged);
			
		// Tooltips
		m_TooltipMagnet = new EditorUITooltip("Toggle magnet mode on/off.", "Magnet Mode");
		m_TooltipSnap = new EditorUITooltip("Toggle snap mode on/off.", "Snap Mode");
		m_TooltipGround = new EditorUITooltip("Toggle ground mode on/off.", "Ground Mode");
		
		// Info toolbar widgets
		m_ObjPosInfoPanel = m_Root.FindAnyWidget("InfobarObjPosFrame");
		array< EditorView > views = EditorView.GetUIProperties(m_ObjPosInfoPanel, this);
		Print( "EditorView count:" + views.Count() );
		for (int index = 0; index < views.Count(); ++index)
		{
			Print( "index:" + index );
			views[index].DebugPrint();
		}
		
		m_CamPosInfoPanel = m_Root.FindAnyWidget("InfobarCamPosFrame");
		m_CamPosInfoX = TextWidget.Cast(m_Root.FindAnyWidget("Info_CamPos_X_Value"));
		m_CamPosInfoY = TextWidget.Cast(m_Root.FindAnyWidget("Info_CamPos_Y_Value"));
		m_CamPosInfoZ = TextWidget.Cast(m_Root.FindAnyWidget("Info_CamPos_Z_Value"));
		
		// debug info
		m_DebugFrame = m_Root.FindAnyWidget("DebugFrame");
		//m_DebugFrame.Show(false);
		
		return m_Root;
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
			
	}
	
	private bool cursor_state;
	void ShowMap(bool state)
	{
		EditorPrint("EditorUI::ShowMap");			
		
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
	
	void SetOrientationWidget(vector orientation)
	{
		m_OrientationWidget.SetModelOrientation(Vector(orientation[1], orientation[0], orientation[2]));
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
		EditorPrint("EditorUI::OnClick");
		
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
					if (!m_SimcityButton.GetState())
						EditorEvents.ChangeBrush(this, null);
					else {
						
						EditorEvents.ChangeBrush(this, GetEditor().GetBrushFromName(brush_name));
					}
					return true;
				}
				
				case m_BrushTypeBox: {
					brush_name = m_CurrentBrushNames.Get(m_BrushTypeBox.GetCurrentItem());
					EditorEvents.ChangeBrush(this, GetEditor().GetBrushFromName(brush_name));
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
				
				case m_CategorySelectBuilding: {
					GetEditor().GetSettings().SetPlaceableObjectCategory(PlaceableObjectCategory.BUILDING);
					m_CategorySelectVehicle.SetState(false);
					m_CategorySelectEntity.SetState(false);
					m_CategorySelectHuman.SetState(false);
					return true;
				}
				
				case m_CategorySelectVehicle: {
					GetEditor().GetSettings().SetPlaceableObjectCategory(PlaceableObjectCategory.VEHICLE);
					m_CategorySelectBuilding.SetState(false);
					m_CategorySelectEntity.SetState(false);
					m_CategorySelectHuman.SetState(false);
					return true;
				}
				
				case m_CategorySelectEntity: {
					GetEditor().GetSettings().SetPlaceableObjectCategory(PlaceableObjectCategory.ENTITY);
					m_CategorySelectVehicle.SetState(false);
					m_CategorySelectBuilding.SetState(false);
					m_CategorySelectHuman.SetState(false);
					return true;
				}
				
				case m_CategorySelectHuman: {
					GetEditor().GetSettings().SetPlaceableObjectCategory(PlaceableObjectCategory.HUMAN);
					m_CategorySelectVehicle.SetState(false);
					m_CategorySelectEntity.SetState(false);
					m_CategorySelectBuilding.SetState(false);
					return true;
				}
				
				default: {
					Print(string.Format("%1 Doesnt have a click function!", w.GetName()));
				}
			}
		}
		
		return super.OnClick(w, x, y, button);
		
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorPrint("EditorUI::OnMouseButtonDown");
		
		
		if (EditorUIManager.IsContextMenuActive()) {
			EditorContextMenu menu = EditorUIManager.GetContextMenu();
			if (menu.GetRoot() != w)
				menu.Close();
		}
		
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
			
			
			EditorEvents.ClearSelection(this);
			if (GetEditor().GetEditorBrush() == null) {
				
				if (Editor.EditorObjectUnderCursor == null) {
					// delayed dragbox					
					GetCursorPos(start_x, start_y);
					GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragBoxCheck, 60);
					return true;
					
					
				} else if (Editor.EditorObjectUnderCursor != null) {
					if (!input.LocalValue("UATurbo")) {
						EditorEvents.ClearSelection(this);
					}
					EditorEvents.SelectObject(this, Editor.EditorObjectUnderCursor);
					return true;
				}
			}
			
			
		}
		
		// Right mouse
		if (button == 1) {
			// temp
			if (w.GetName() == "RightbarScroll") {
				
				EditorContextMenu context_menu = new EditorContextMenu();
				context_menu.AddButton(new EditorContextMenuButton("Create Folder", "CreatePlacedFolder", this));
				EditorContextMenuFolder folder = EditorContextMenuFolder("Folder1");
				folder.AddButton(new EditorContextMenuButton("Test1"));
				context_menu.AddButton(folder);
				context_menu.Show();

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
				pos[1] = GetEditor().GetUIManager().GetEditorCamera().GetPosition()[1];
				GetEditor().GetUIManager().GetEditorCamera().SetPosition(pos);
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
			
			ref EditorObjectSet placed_objects = GetEditor().GetObjectManager().GetPlacedObjects();
			foreach (EditorObject editor_object: placed_objects) {
				
				float marker_x, marker_y;
				if (IsMapOpen()) {
					editor_object.GetMapMarkerPos(marker_x, marker_y);
				} else {
					editor_object.GetObjectMarkerPos(marker_x, marker_y);
				}
				
				
				if ((marker_x < x_high && marker_x > x_low) && (marker_y < y_high && marker_y > y_low)) {		
					EditorEvents.SelectObject(this, editor_object);
				}
			}
			
			return;
		}
		


	}
	
	

	
	void ShowExportWindow()
	{
		Print("EditorUI::ShowExportWindow");
		//GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorExportWindow.layout", Getm_Root());
		EditorExportWindow dialog = new EditorExportWindow();
		GetGame().GetUIManager().ShowScriptedMenu(dialog, GetGame().GetUIManager().GetMenu());
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
	
	

	
	
	void OnPlaceableCategoryChanged(Class context, PlaceableObjectCategory category)
	{
		#ifdef EDITORPRINT
		EditorPrint("EditorUI:OnPlaceableCategoryChanged - Start");
		#endif
		
		m_LeftbarSpacer.Update();
		
		switch (category) {
			
			case PlaceableObjectCategory.BUILDING: {
				m_CategorySelectBuilding.SetState(true);
				m_CategorySelectVehicle.SetState(false);
				m_CategorySelectEntity.SetState(false);
				m_CategorySelectHuman.SetState(false);
				break;
			}
			
			case PlaceableObjectCategory.VEHICLE: {
				m_CategorySelectBuilding.SetState(false);
				m_CategorySelectVehicle.SetState(true);
				m_CategorySelectEntity.SetState(false);
				m_CategorySelectHuman.SetState(false);
				break;
			}
			
			case PlaceableObjectCategory.ENTITY: {
				m_CategorySelectBuilding.SetState(false);
				m_CategorySelectVehicle.SetState(false);
				m_CategorySelectEntity.SetState(true);
				m_CategorySelectHuman.SetState(false);
				break;
			}
			
			case PlaceableObjectCategory.HUMAN: {
				m_CategorySelectBuilding.SetState(false);
				m_CategorySelectVehicle.SetState(false);
				m_CategorySelectEntity.SetState(false);
				m_CategorySelectHuman.SetState(true);
				break;
			}
		}
		
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		EditorPrint("EditorUI::OnMouseEnter");
		
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
		EditorPrint("EditorUI::OnMouseLeave");
		
		
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
	
	void CreatePlacedFolder()
	{	
		EditorCollapsibleListItem t = new EditorCollapsibleListItem();
		InsertPlacedObject(t);
	}
}

class EditorUIHandler: EditorWidgetEventHandler
{	
	
	void EditorUIHandler()
	{
		Print("EditorUIHandler");
	}
	
	void ~EditorUIHandler()
	{
		Print("~EditorUIHandler");
	}
		
	
	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		Print("EditorUIHandler::OnModalResult");
		Print(code);
		Print(result);
		
		return true;
	}
}