




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

	// ExportDialog
	protected ref EditorExportDialog m_ExportDialog;
	
	// Orientation Tool
	ItemPreviewWidget m_OrientationWidget;
	
	// Debug
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
	
	void EditorUI()
	{
		Print("EditorUI");
	}
	
	void ~EditorUI()
	{
		Print("~EditorUI");
	}
	
	
	override Widget Init()
	{
		// Init
		m_EditorUIHandler = new EditorUIHandler();
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorNew.layout");
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
		m_OrientationWidget		= ItemPreviewWidget.Cast(m_Root.FindAnyWidget("OrientationView"));
		
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
		

		m_ExportDialog = new EditorExportDialog(m_Root);
		m_ExportDialog.Show(false); //! Comment me if you have implementent me or want to see me!
		
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
		EditorEvents.OnBrushChanged.Insert(OnBrushChanged);
		EditorEvents.OnPlaceableCategoryChanged.Insert(OnPlaceableCategoryChanged);
			
		// Tooltips
		m_TooltipMagnet = new EditorUITooltip("Magnet Mode", "Toogle magnet mode on/off.");
		m_TooltipSnap = new EditorUITooltip("Snap Mode", "Toogle snap mode on/off.");
		m_TooltipGround = new EditorUITooltip("Ground Mode", "Toogle ground mode on/off.");
		
		return m_Root;
	}
	
	
	
	override void Update(float timeslice)
	{
		super.Update(timeslice);
				
		if (m_LeftbarScroll.GetVScrollPos() > m_LeftbarScroll.GetContentHeight())
			m_LeftbarScroll.VScrollToPos(0);
		
		CheckToolbarButtonsState();
	}
	
	void ShowMap(bool state)
	{
		Print("EditorUI::OpenMap");
		if (state) {
			m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() * 3);
			m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() * 3);
		} else {
			m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() / 3);
			m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() / 3);
		}
		
		m_EditorMapContainer.Show(state);
		m_EditorMapContainer.Update();
		ShowCursor();
	}
	
	
	void CreateDialog()
	{
		// todo use editor UI manager to do ShowDialog
		//GetGame().GetUIManager().ShowDialog("Overwrite", "Do you really want to Overwrite?", 169, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
	}
	
	EditorMap GetMap() 			{ return m_EditorMap; }
	MapWidget GetMapWidget() 	{ return m_EditorMapWidget; }
	bool IsMapOpen() 			{ return m_EditorMapContainer.IsVisible(); }
	
	void ShowCursor()
	{
		// todo use editor UI manager to do this
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	void HideCursor()
	{
		// todo use editor UI manager to do this
		GetGame().GetUIManager().ShowUICursor(false);
	}
	
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
	
	
	

	private ref array<ref EditorListItem> m_CurrentPlaceableObjects = new array<ref EditorListItem>();
	void InsertPlaceableObject(EditorPlaceableObject placeable_object)
	{
		m_CurrentPlaceableObjects.Insert(placeable_object.SetListItem(m_LeftbarSpacer));
	}
	
	
	/* Events */
	override bool OnClick(Widget w, int x, int y, int button) 
	{
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
					if (!m_SimcityButton.GetState())
						EditorEvents.BrushChangedInvoke(this, null);
					else EditorEvents.BrushChangedInvoke(this, EditorBrushFromIndex(m_BrushTypeBox.GetCurrentItem()));
					
					return true;
				}
				
				case m_BrushTypeBox: {
					EditorEvents.BrushChangedInvoke(this, EditorBrushFromIndex(m_BrushTypeBox.GetCurrentItem()));
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
					return false;
				}
			}
		}
		
		return false;
		
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnMouseButtonDown: " + button);
		Input input = GetGame().GetInput();	
		// Left Click
		if (button == 0) {
			
			if (GetEditor().IsPlacing()) {
				GetEditor().PlaceObject();
				return true;
			} 
			
			// Raycast to see if TranslationWidget is under cursor			
			RaycastRVParams raycast_params = new RaycastRVParams(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * EditorSettings.OBJECT_VIEW_DISTANCE);
			ref array<ref RaycastRVResult> raycast_result = new array<ref RaycastRVResult>();
			DayZPhysics.RaycastRVProxy(raycast_params, raycast_result);
			
			//DayZPhysics.RaycastRV(start_pos, end_pos, contact_pos, contact_dir, contact_comp, collisions);
			if (raycast_result.Get(0).obj != NULL) {
				if ((raycast_result.Get(0).obj == GetEditor().GetTranslationWidget() || raycast_result.Get(0).obj == GetEditor().GetTranslationWidget().GetRotationWidget())) {
					EditorEvents.DragInvoke(raycast_result[0].obj, GetEditor().GetTranslationWidget().GetEditorObject(), raycast_result.Get(0));
					return true;
				}
			}
			
			if (Editor.EditorObjectUnderCursor == null && GetEditor().GetEditorBrush() == null) {
				// delayed dragbox
				GetEditor().GetObjectManager().ClearSelection();
				GetCursorPos(start_x, start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragBoxCheck, 60);
				
			} 
			
			else if (Editor.EditorObjectUnderCursor != null) {
				GetEditor().GetObjectManager().SelectObject(Editor.EditorObjectUnderCursor, !input.LocalValue("UATurbo"));
			}
			
			
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
		
		// temp hopefully
		SetFocus(null);
		
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
		if (GetGame().GetInput().LocalRelease("UAFire")) {
			m_EditorCanvas.Clear();
			DragBoxQueue.Remove(UpdateDragBox);
			return;
		}
		
		int current_x, current_y;
		GetCursorPos(current_x, current_y);
		m_EditorCanvas.Clear();
		int selection_box_thickness = 2;
		int selection_box_color = ARGB(100, 255, 0, 0);
		m_EditorCanvas.DrawLine(start_x, start_y, current_x, start_y, selection_box_thickness, selection_box_color);
		m_EditorCanvas.DrawLine(start_x, start_y, start_x, current_y, selection_box_thickness, selection_box_color);
		m_EditorCanvas.DrawLine(start_x, current_y, current_x, current_y, selection_box_thickness, selection_box_color);
		m_EditorCanvas.DrawLine(current_x, start_y, current_x, current_y, selection_box_thickness, selection_box_color);
		
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
				editor_object.GetMapMarker().GetPos(marker_x, marker_y);
			} else {
				editor_object.GetObjectMarker().GetPos(marker_x, marker_y);
			}
			if ((marker_x < x_high && marker_x > x_low) && (marker_y < y_high && marker_y > y_low)) {		
				GetEditor().GetObjectManager().SelectObject(editor_object, false);
			} else {
				GetEditor().GetObjectManager().DeselectObject(editor_object);
			}
		}
	}
	
	

	
	void ShowExportWindow()
	{
		Print("EditorUI::ShowExportWindow");	
		//GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorExportWindow.layout", Getm_Root());
		EditorExportWindow dialog = new EditorExportWindow();
		GetGame().GetUIManager().ShowScriptedMenu(dialog, GetGame().GetUIManager().GetMenu());
	}

	void InsertPlacedObject(EditorObject target)
	{
		m_RightbarSpacer.AddChild(target.GetObjectBrowser());
	}
	
	
	
	private EditorBrush EditorBrushFromIndex(int index)
	{
		switch (index) {
				
			// Tree Brush
			case 0: {
				return new TreeBrush(m_SimcityRadiusSlider.GetCurrent());
			}				
			
			// Grass Brush
			case 1: {
				return new GrassBrush(m_SimcityRadiusSlider.GetCurrent());
			}
			
			// ExplosionBrush
			case 2: {
				return new BoomBrush(m_SimcityRadiusSlider.GetCurrent());
			}
			
			// DeleteBrush
			case 3: {
				return new DeleteBrush(m_SimcityRadiusSlider.GetCurrent());
			}
			
			default: {
				Print("Brush index not found");
				break;
			}
		}
		
		return null;
	}
		
	void OnBrushChanged(Class context, EditorBrush brush)
	{
		if (brush == null) {
			m_SimcityDensitySlider.Show(false);
			m_SimcityRadiusSlider.Show(false);
		
			m_SimcityDensityText.Show(false);
			m_SimcityRadiusText.Show(false);
			return;
		}
		
		switch (brush.Type()) {
			
			case DeleteBrush: {
				m_SimcityDensitySlider.Show(false);
				m_SimcityRadiusSlider.Show(true);
				
				m_SimcityDensityText.Show(false);
				m_SimcityRadiusText.Show(true);
				break;
			}
		
			default: {
				m_SimcityDensitySlider.Show(true);
				m_SimcityRadiusSlider.Show(true);
				
				m_SimcityDensityText.Show(true);
				m_SimcityRadiusText.Show(true);
				break;
			}
		}
	}
	
	void OnPlaceableCategoryChanged(Class context, PlaceableObjectCategory category)
	{
		Print("EditorUI::OnPlaceableCategoryChanged");
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
		SetFocus( w );
				
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_Icon" ) );
		
		if( image )
		{
			image.SetColor( ARGB( 255, 52, 152, 219 ) );
		}
	}
	
	void ColorRed( Widget w, int x, int y )
	{
		SetFocus( w );
		
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
		SetFocus( w );
				
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
		SetFocus( w );
		
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
		SetFocus( w );
		
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

	private void CheckToolbarButtonsState()
	{
		//! Color managment
		if ( m_MagnetButton.GetState() )
		{
			ColorRed( m_MagnetButton, 0, 0 );
		} else if ( !m_MagnetButton.GetState() && !m_FocusMagnet )
		{
			ColorNoFocus( m_MagnetButton, null, 0, 0 );
		}
		
		if ( m_GroundButton.GetState() )
		{
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