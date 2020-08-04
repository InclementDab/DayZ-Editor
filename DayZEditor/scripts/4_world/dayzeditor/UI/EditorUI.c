
// make it so you can search for items by mod name with @ModNameHere

class EditorDropdownPrefab extends ScriptedWidgetEventHandler
{
	protected Widget				m_Root;
	protected ScrollWidget			m_Scroller;
	protected Widget				m_ContentContainer;
	protected ref array<Widget>		m_Content		= new array<Widget>>;
	
	protected Widget				m_Button;
	protected TextWidget			m_Text;
	protected ImageWidget			m_ImageExpand;
	protected ImageWidget			m_ImageCollapse;
	
	protected bool					m_IsExpanded;
	ref ScriptInvoker				m_OnSelectItem	= new ScriptInvoker();

	void EditorDropdownPrefab( Widget root, string text = "" )
	{
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/dropdown_prefab/dropdown_prefab.layout", root );
		
		m_Scroller			= ScrollWidget.Cast( m_Root.FindAnyWidget( "dropdown_container" ) );
		m_ContentContainer	= m_Root.FindAnyWidget( "dropdown_content" );
		m_Text				= TextWidget.Cast( m_Root.FindAnyWidget( "dropdown_text" ) );
		SetText( text );
		
		m_Button			= m_Root.FindAnyWidget( "dropdown_selector_button" );
		m_ImageExpand		= ImageWidget.Cast( m_Root.FindAnyWidget( "expand_image" ) );
		m_ImageCollapse		= ImageWidget.Cast( m_Root.FindAnyWidget( "collapse_image" ) );
		
		m_Root.SetHandler( this );
		
		RefreshContent();
	}
	
	void RefreshContent()
	{
		Widget child = m_ContentContainer.GetChildren();
		while( child )
		{
			if( m_Content.Find( child ) > -1 )
			{
				m_Content.Insert( child );
			}
		}
		
		m_ContentContainer.Update();
		m_Root.Update();
		
		float x, y;
		m_ContentContainer.GetScreenSize( x, y );
		if( y > m_Scroller.GetContentHeight() )
		{
			m_Scroller.SetAlpha( 1 );
		}
		else
		{
			m_Scroller.SetAlpha( 0 );
		}
	}
	
	int AddElement( string text, Widget content = null )
	{
		ButtonWidget element = ButtonWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "gui/layouts/new_ui/dropdown_prefab/dropdown_element.layout", m_ContentContainer ) );
		element.SetText( text );
		
		if( content )
		{
			element.AddChild( content, false );
		}
		m_ContentContainer.Update();
		m_Root.Update();
		
		m_Content.Insert( element );
		return m_Content.Count() - 1;
	}
	
	void RemoveElement( int index )
	{
		if( 0 < index && index < m_Content.Count() )
		{
			delete m_Content.Get( index );
			m_ContentContainer.Update();
			m_Root.Update();
		}
	}
	
	void Close()
	{
		if( m_IsExpanded )
		{
			m_Scroller.Show( false );
			m_ImageExpand.Show( false );
			m_ImageCollapse.Show( true );
		}
	}
	
	void SetText( string text )
	{
		m_Text.SetText( text );
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		int index = m_Content.Find( w );
		if( index > -1 )
		{
			m_OnSelectItem.Invoke( index );
			m_IsExpanded = false;
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			return true;
		}
		return false;
	}
	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if( w == m_Root && button == MouseState.LEFT )
		{
			m_IsExpanded = !m_IsExpanded;
			m_Scroller.Show( m_IsExpanded );
			m_ImageExpand.Show( !m_IsExpanded );
			m_ImageCollapse.Show( m_IsExpanded );
			
			m_Root.Update();
			return true;
		}
		return false;
	}
}

class EditorExportDialog: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Widget m_DirectorySelectorPanel;
	protected WrapSpacerWidget m_ListEntryWrapper;
	
	protected ref EditorDropdownPrefab m_Dropdown;
	protected ref array<string> m_Files;
	
	private string m_PathToMissions;
	
	void EditorExportDialog(Widget parent)
	{
		m_Root	= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZEditor/gui/layouts/EditorExportDialog.layout", parent ) );
			
		m_DirectorySelectorPanel = Widget.Cast(m_Root.FindAnyWidget("DirectorySelectorPanel"));
		m_ListEntryWrapper = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("LeftbarSpacer"));
				
		m_Root.SetHandler(this);
		
		InitDialog();
	}
	
	static array< string > FindFilesInLocation( string folder )
	{
		array< string > files = new array< string >;
		string fileName;
		FileAttr fileAttr;
		FindFileHandle findFileHandle = FindFile( folder + "*", fileName, fileAttr, 0 );
		if ( findFileHandle )
		{
			if ( fileName.Length() > 0 && !( fileAttr & FileAttr.DIRECTORY) )
			{
				files.Insert( fileName );
			}
			
			while ( FindNextFile( findFileHandle, fileName, fileAttr ) )
			{
				if ( fileName.Length() > 0 && !( fileAttr & FileAttr.DIRECTORY) )
				{
					files.Insert( fileName );
				}
			}
		}
		CloseFindFile( findFileHandle );
		return files;
	}
	
	void InitDialog()
	{
		m_Dropdown		= new EditorDropdownPrefab( m_DirectorySelectorPanel, "Directory" );
		m_Dropdown.m_OnSelectItem.Insert( OnSelectDirectory );
						
		m_PathToMissions = "missions";//"$saves:\\missions";
		string path_find_pattern = m_PathToMissions + "\\*";	
			
		m_Files = new array<string>;
		m_Files = FindFilesInLocation(path_find_pattern);	
		
		for( int i = 0; i < m_Files.Count(); i++ )
		{
			m_Dropdown.AddElement( m_Files[i] );
		}
	}
	
	void OnSelectDirectory()
	{
	
	}
	
	void Show(bool state)
	{
		m_Root.Show(state);
	}
}

class EditorUIToolbar: EditorWidgetEventHandler
{
	
	protected ButtonWidget 	m_UndoButton;
	protected ButtonWidget 	m_RedoButton;
	protected ButtonWidget 	m_MagnetButton;
	protected ButtonWidget 	m_GroundButton;
	protected ButtonWidget 	m_SnapButton;
	protected ButtonWidget 	m_SimcityButton;
	protected ButtonWidget 	m_DeleteBrushButton;
	
	protected SliderWidget 	m_SimcityRadiusSlider;
	protected TextWidget	m_SimcityRadiusText;	
	
	protected SliderWidget 	m_SimcityDensitySlider;
	protected TextWidget	m_SimcityDensityText;
	
	protected XComboBoxWidget	m_BrushTypeBox;
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorUIToolbar::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		m_UndoButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("UndoButton"));
		m_RedoButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("RedoButton"));
		m_MagnetButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("MagnetButton"));
		m_GroundButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("GroundButton"));
		m_SnapButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("SnapButton"));
		m_SimcityButton			= ButtonWidget.Cast(m_Root.FindAnyWidget("SimcityButton"));
		m_DeleteBrushButton		= ButtonWidget.Cast(m_Root.FindAnyWidget("DeleteBrush"));
		
		m_SimcityRadiusSlider	= SliderWidget.Cast(m_Root.FindAnyWidget("SimcityRadiusSlider"));
		m_SimcityRadiusText		= TextWidget.Cast(m_Root.FindAnyWidget("SimcityRadiusText"));		
		
		m_SimcityDensitySlider	= SliderWidget.Cast(m_Root.FindAnyWidget("SimcityDensitySlider"));
		m_SimcityDensityText	= TextWidget.Cast(m_Root.FindAnyWidget("SimcityDensityText"));
		
		m_BrushTypeBox	= XComboBoxWidget.Cast(m_Root.FindAnyWidget("BrushTypeBox"));
		
		
		m_SimcityRadiusText.SetText(m_SimcityRadiusSlider.GetCurrent().ToString());
	}
	

	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorUIToolbar::OnClick");
		if (button == 0) {
			if (w == m_MagnetButton) {
				Print(m_MagnetButton.GetState());
				EditorSettings.MAGNET_PLACEMENT = m_MagnetButton.GetState();
				return true;
			}
			if (w == m_GroundButton) {
				EditorSettings.MAINTAIN_HEIGHT = m_GroundButton.GetState();
				return true;
			}
			if (w == m_SnapButton) {
				EditorSettings.SNAPPING_MODE = m_SnapButton.GetState();
				return true;
			}
			
		}
		
		return false;
	}
	
	override bool OnEvent(EventType eventType, Widget target, int parameter0, int parameter1)
	{
		//Print("EditorUIToolbar::OnUpdate");
		
		if (target == m_BrushTypeBox || target == m_SimcityButton) {
			
			int index = m_BrushTypeBox.GetCurrentItem();
			if (!m_SimcityButton.GetState()) { 
				delete Editor.ActiveBrush;
				m_SimcityDensitySlider.Show(false);
				m_SimcityRadiusSlider.Show(false);
				return true;
			}
			
			switch (index) {
				
				// Tree Brush
				case 0: {
					Editor.ActiveBrush = new TreeBrush(m_SimcityRadiusSlider.GetCurrent());
					m_SimcityDensitySlider.Show(true);
					m_SimcityRadiusSlider.Show(true);
					break;
				}				
				
				// Grass Brush
				case 1: {
					Editor.ActiveBrush = new GrassBrush(m_SimcityRadiusSlider.GetCurrent());
					m_SimcityDensitySlider.Show(true);
					m_SimcityRadiusSlider.Show(true);
					break;
				}
				
				// ExplosionBrush
				case 2: {
					Editor.ActiveBrush = new BoomBrush(m_SimcityRadiusSlider.GetCurrent());
					m_SimcityDensitySlider.Show(true);
					m_SimcityRadiusSlider.Show(true);
					break;
				}
				
				// DeleteBrush
				case 3: {
					Editor.ActiveBrush = new DeleteBrush(m_SimcityRadiusSlider.GetCurrent());
					m_SimcityDensitySlider.Show(false);
					m_SimcityRadiusSlider.Show(true);
					break;
				}
				
			}
		}
		
		if (target == m_SimcityRadiusSlider && m_SimcityRadiusSlider.IsVisible()) {
			m_SimcityRadiusText.SetText(m_SimcityRadiusSlider.GetCurrent().ToString());
			Editor.ActiveBrush.SetRadius(m_SimcityRadiusSlider.GetCurrent());
		}
		
		if (target == m_SimcityDensitySlider && m_SimcityDensitySlider.IsVisible()) {
			m_SimcityDensityText.SetText(m_SimcityDensitySlider.GetCurrent().ToString());
			DensityBrush.Cast(Editor.ActiveBrush).SetDensity(m_SimcityDensitySlider.GetCurrent());
		}
		
		return false;
		
	}
	
	
	
}


enum EditorCursor
{
	DEFAULT,
	HORIZONTAL_SCROLL
}

class EditorUI: EditorWidgetEventHandler
{	
	
	static ref EditorUI m_Instance;
	static EditorUI GetInstance() { return m_Instance; }
	
	// Canvas
	static ref CanvasWidget EditorCanvas;
	
	// Buttons
	protected ButtonWidget m_BuildingSelect;
	protected ButtonWidget m_VehicleSelect;
	protected ButtonWidget m_EntitySelect;
	protected ButtonWidget m_HumanSelect;
	protected ButtonWidget m_LeftbarHide;
	protected ButtonWidget m_RightbarHide;

	
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
	
	// Cursors
	protected Widget			m_HorizontalScrollWidget;

	// ExportDialog
	protected ref EditorExportDialog m_ExportDialog;
	
	void EditorUI()
	{
		m_Instance = this;
	}
	

	
	
	override void OnWidgetScriptInit(Widget w)
	{
		Print("EditorUI::OnWidgetScriptInit");
		super.OnWidgetScriptInit(w);
		
		// Canvas
		EditorCanvas			= CanvasWidget.Cast(m_Root.FindAnyWidget("EditorCanvas"));
		
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
		
		// Spacers
		m_LeftbarSpacer			= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("LeftbarSpacer"));
		m_RightbarSpacer		= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("RightbarSpacer"));
		
		
		
		// Debug
		m_DebugText1			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText1"));
		m_DebugText2			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText2"));
		m_DebugText3			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText3"));
		m_DebugText4			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText4"));
		m_DebugText5			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText5"));
		m_DebugText6			= TextWidget.Cast(m_Root.FindAnyWidget("DebugText6"));
		m_DebugActionStack		= TextListboxWidget.Cast(m_Root.FindAnyWidget("DebugActionStackListbox"));

		
		m_EditorMapWidget.GetScript(m_EditorMap);
		m_EditorMapWidget.SetMapPos(GetGame().GetCurrentCameraPosition());
		
		// Cursors
		m_HorizontalScrollWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "cursors/horizontalwidget.layout");
		
		m_ExportDialog = new EditorExportDialog(m_Root);
		m_ExportDialog.Show(false); //! Comment me if you have implementent me or want to see me!
	}
	
	private bool left_bar_hidden = false;
	private bool right_bar_hidden = false;
	

	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorUI::OnMouseButtonDown: " + button);
		Input input = GetGame().GetInput();	
		// Left Click
		if (button == 0) {
			
			if (w == m_LeftbarHide) {
				left_bar_hidden = !left_bar_hidden;
				m_LeftbarFrame.SetPos(-300 * left_bar_hidden, 0);
				return true;
			} 
			
			if (w == m_RightbarHide) {
				right_bar_hidden = !right_bar_hidden;
				m_RightbarFrame.SetPos(-300 * right_bar_hidden, 48);
				return true;
			}
			
			if (Editor.IsPlacing()) {
				Editor.PlaceObject();
				return true;
			} 
			
			// Raycast to see if TranslationWidget is under cursor			
			RaycastRVParams raycast_params = new RaycastRVParams(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * EditorSettings.OBJECT_VIEW_DISTANCE);
			ref array<ref RaycastRVResult> raycast_result = new array<ref RaycastRVResult>();
			DayZPhysics.RaycastRVProxy(raycast_params, raycast_result);
			
			//DayZPhysics.RaycastRV(start_pos, end_pos, contact_pos, contact_dir, contact_comp, collisions);
			if (raycast_result.Get(0).obj != NULL) {
				if ((raycast_result.Get(0).obj == Editor.GetTranslationWidget() || raycast_result.Get(0).obj == Editor.GetTranslationWidget().GetRotationWidget())) {
					EditorEvents.DragInvoke(raycast_result[0].obj, Editor.GetTranslationWidget().GetEditorObject(), raycast_result.Get(0));
					return true;
				}
			}
			


			if (Editor.EditorObjectUnderCursor == null && Editor.ActiveBrush == null) {
				// delayed dragbox
				EditorUI.EditorCanvas.Clear();
				GetCursorPos(start_x, start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DelayedDragBoxCheck, 60);
				
			} else if (Editor.EditorObjectUnderCursor != null) {
				Editor.EditorObjectUnderCursor.Select(!input.LocalValue("UATurbo"));
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
				pos[1] = Editor.ActiveCamera.GetPosition()[1];
				Editor.ActiveCamera.SetPosition(pos);
			}
		}
		
		// temp hopefully
		SetFocus(null);
		
		return false;
	}
		

	bool OnKeyPress(int key)
	{
		switch (key) {
			
			case KeyCode.KC_M: {
				
				if (m_EditorMapContainer.IsVisible()) {
					CloseMap();
				} else {
					OpenMap();
				}
				m_EditorMapContainer.Update();
				return true;
			}
		
			case KeyCode.KC_SPACE: {
				if (GetGame().GetUIManager().IsCursorVisible() && !m_EditorMapContainer.IsVisible()) 
					HideCursor();
				else ShowCursor();
				return true;
			}
			/*
			case KeyCode.KC_U: {
				EditorSettings.MAGNET_PLACEMENT = !EditorSettings.MAGNET_PLACEMENT;
				m_ToolbarMagnet.SetState(EditorSettings.MAGNET_PLACEMENT);
				m_ToolbarMagnet.Update();
				SetFocus(null);
				return true;
			}*/
			
			case KeyCode.KC_Y: {
				m_Root.Show(!m_Root.IsVisible());
				foreach (EditorObject editor_object: Editor.PlacedObjects) {
					editor_object.GetObjectMarker().Show(m_Root.IsVisible());
				}
				
				return true;
			}
			/*
			case KeyCode.KC_G: {
				
				EditorSettings.MAINTAIN_HEIGHT = !EditorSettings.MAINTAIN_HEIGHT;
				m_ToolbarGround.SetState(EditorSettings.MAINTAIN_HEIGHT);
				m_ToolbarGround.Update();
				SetFocus(null);
				return true;
			}*/
		}
		return false;
	}
	
	ScriptInvoker DragBoxQueue = GetGame().GetUpdateQueue(CALL_CATEGORY_GUI);
	int start_x, start_y;
	void DelayedDragBoxCheck()
	{
		Input input = GetGame().GetInput();
		if (input.LocalValue("UAFire")) {
			DragBoxQueue.Insert(UpdateDragBox);
		}
		
	}
	
	bool IsDragging = false; // this is very broken find a better way to do this
	void UpdateDragBox()
	{	
		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			EditorUI.EditorCanvas.Clear();
			IsDragging = false;
			DragBoxQueue.Remove(UpdateDragBox);
			return;
		}
		
		IsDragging = true;
		int current_x, current_y;
		GetCursorPos(current_x, current_y);
		EditorUI.EditorCanvas.Clear();
		int selection_box_thickness = 2;
		int selection_box_color = ARGB(100, 255, 0, 0);
		EditorUI.EditorCanvas.DrawLine(start_x, start_y, current_x, start_y, selection_box_thickness, selection_box_color);
		EditorUI.EditorCanvas.DrawLine(start_x, start_y, start_x, current_y, selection_box_thickness, selection_box_color);
		EditorUI.EditorCanvas.DrawLine(start_x, current_y, current_x, current_y, selection_box_thickness, selection_box_color);
		EditorUI.EditorCanvas.DrawLine(current_x, start_y, current_x, current_y, selection_box_thickness, selection_box_color);
		
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
		
		
		foreach (EditorObject editor_object: Editor.PlacedObjects) {
			
			float marker_x, marker_y;
			if (IsMapOpen()) {
				editor_object.GetMapMarker().GetPos(marker_x, marker_y);
			} else {
				editor_object.GetObjectMarker().GetPos(marker_x, marker_y);
			}
			if ((marker_x < x_high && marker_x > x_low) && (marker_y < y_high && marker_y > y_low)) {				
				editor_object.Select(false);
			} else {
				editor_object.Deselect();
			}
		}
		
		
	}
	
	
	void OpenMap()
	{
		Print("EditorUI::OpenMap");
		m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() * 3);
		m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() * 3);
		m_EditorMapContainer.Show(true);
		ShowCursor();
	}
	
	void CloseMap()
	{
		Print("EditorUI::CloseMap");
		m_LeftbarPanelHost.SetAlpha(m_LeftbarPanelHost.GetAlpha() / 3);
		m_RightbarPanelHost.SetAlpha(m_RightbarPanelHost.GetAlpha() / 3);
		m_EditorMapContainer.Show(false);
		ShowCursor();
	}
	
	EditorMap GetMap() { return m_EditorMap; }
	MapWidget GetMapWidget() { return m_EditorMapWidget; }
	
	bool IsMapOpen() { return m_EditorMapContainer.IsVisible(); }
	
	void ShowCursor()
	{
		EditorCamera.LookFreeze = true;
		GetGame().GetUIManager().ShowUICursor(true);
	}
	
	void HideCursor()
	{
		EditorCamera.LookFreeze = false;
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
	
	void ShowExportWindow()
	{
		Print("EditorUI::ShowExportWindow");	
		
		//GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorExportWindow.layout", Getm_Root());
		
		EditorExportWindow dialog = new EditorExportWindow();
		GetGame().GetUIManager().ShowScriptedMenu(dialog, GetGame().GetUIManager().GetMenu());
	}
	
	
	void InsertPlaceableObject(string placeable_object)
	{
		EditorListItem list_item;
		Widget list_widget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItem.layout", m_LeftbarSpacer);
		list_widget.GetScript(list_item);
		list_item.SetObject(placeable_object);
	}
	
	void InsertPlacedObject(EditorObject target)
	{
		m_RightbarSpacer.AddChild(target.GetObjectBrowser());
	}
}