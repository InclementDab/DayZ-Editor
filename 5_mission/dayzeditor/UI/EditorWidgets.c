
static string layout_dir = "P:/DayZ_Server/dev/DayZEditor/Addons/Editor/Layouts/";

class EditorContextMenu: ScriptedWidgetEventHandler
{
	static ref EditorContextMenu m_Instance;
	static EditorContextMenu GetInstance() 
	{
		return m_Instance;
	}
	
	protected Widget m_Root;
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorObjectMarkerHandler::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		m_Instance = this;
	}	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorContextMenu::OnClick");
		if (Editor.SelectedObject == null) return false;
		
		switch (w.GetName()) {
			case ("CtxProperties"):
				EditorObjectPropertiesWindow properties_window;
				Widget window = GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorObjectProperties.layout", GetGame().GetUIManager().GetMenu().GetLayoutRoot());
				window.SetHandler(properties_window);
				properties_window.SetObject(Editor.SelectedObject);
				
				break;
			
			case ("CtxAlignToGround"):
				//PlaceOnSurfaceRotated is better if you wanna spend 40k in college
				
				vector tra[4];		
				Editor.SelectedObject.GetTransform(tra);	
				float a = GetGame().SurfaceY(tra[3][0], tra[3][2]);
				vector surface_orientation = GetGame().GetSurfaceOrientation(tra[3][0], tra[3][2]);
				
				vector mat[4];	
				surface_orientation.RotationMatrixFromAngles(mat);
				

				mat[3] = tra[3];
				Print(mat);
				Print(tra);
								
				Editor.SelectedObject.SetTransform(mat);
				Editor.SelectedObject.PlaceOnSurface();
			
				break;
			
			case ("CtxDelete"):
				//Editor.DeleteObject(Editor.SelectedObject);
				break;
			
			default:
				return false;
		}
		
		m_Instance.m_Root.Show(false);
		return true;
	}
	
	
	static void ShowContextMenu(int x, int y)
	{
		m_Instance.m_Root.SetPos(x, y);
		m_Instance.m_Root.Show(true);
	}
	
	static void HideContextMenu()
	{
		m_Instance.m_Root.Show(false);
	}
}



static EntityAI CreateDummyEntityAI(string name)
{	
	Object o = GetGame().CreateObject(name, "0 0 0");
	EntityAI e = EntityAI.Cast(o);
	GetGame().ObjectDelete(o);
	return e;
}

class EditorListItem: ScriptedWidgetEventHandler
{
	
	protected Widget m_Root;
	private string m_ObjectName;
	protected Widget m_EditorListItemPanel;
	protected TextWidget m_EditorListItemText;	

	protected Widget m_EditorListItemTooltip;
	protected TextWidget m_EditorListItemTooltipTitle;
	protected ItemPreviewWidget m_EditorListItemTooltipPreview;
	
	protected EntityAI m_ItemPreview;
		
	
	void EditorListItem()
	{
		//Print("EditorListItem");
	}
	
	void ~EditorListItem()
	{
		//Print("~EditorListItem");
		delete m_EditorListItemTooltip;
		delete m_Root;
	}
	
	protected void OnWidgetScriptInit(Widget w)
	{
		//Print("EditorListItem::OnWidgetScriptInit");
		m_Root = w;
		m_Root.SetHandler(this);
		
		m_EditorListItemPanel 				= m_Root.FindAnyWidget("EditorListItemPanel");
		m_EditorListItemText 				= TextWidget.Cast(m_Root.FindAnyWidget("EditorListItemText"));
		
		m_EditorListItemTooltip 			= GetGame().GetWorkspace().CreateWidgets(layout_dir + "EditorListItemTooltip.layout");						
		m_EditorListItemTooltipTitle 		= TextWidget.Cast(m_EditorListItemTooltip.FindAnyWidget("EditorListItemTooltipTitle"));
		m_EditorListItemTooltipPreview 		= ItemPreviewWidget.Cast(m_EditorListItemTooltip.FindAnyWidget("EditorListItemTooltipPreview"));
		
	}		
	
	
	
	void SetObject(string name)
	{
		m_ObjectName = name;
		m_EditorListItemText.SetText(m_ObjectName);
		m_ItemPreview = GetGame().CreateObject(m_ObjectName, "0 0 0");
		m_EditorListItemTooltipTitle.SetText(m_ObjectName);
		m_EditorListItemTooltipPreview.SetItem(m_ItemPreview);
		//m_EditorListItemTooltipPreview.SetModelPosition("0 0 0");
	}
	
	override bool OnFocus(Widget w, int x, int y)
	{
		Print("EditorListItem::OnFocus");
		w.SetColor(COLOR_BLUE);
		
		Editor.CreateObjectInHand(m_ObjectName);		
		
		return true;
	}
	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		Print("EditorListItem::OnFocusLost");
		w.SetColor(0xFFFFFFFF);
		return true;
	}		
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorListItem::OnMouseButtonDown");
		if (button == 0) {
			if (w == GetFocus()) return true;
			if (Editor.IsPlacing()) {
				delete Editor.ObjectInHand;
			}
			
			SetFocus(w);
			return true;
		}
		
		return false;
	}
	
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		m_EditorListItemTooltip.SetPos(x, y);
		m_EditorListItemTooltip.Show(true);
		m_EditorListItemTooltipPreview.Update();
		
		return true;
	}	
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_EditorListItemTooltip.Show(false);
		
		//delete m_EditorListItemTooltip;
		return true;
	}
	
	string GetObjectName()
	{
		return m_ObjectName;
	}
}




class EditorSearchBar: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected WrapSpacerWidget m_SearchContext;
	
	void EditorSearchBar(WrapSpacerWidget searchContext)
	{
		m_SearchContext = searchContext;
		m_Root.Enable(false); // annoying as shit
	}
		
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		Print("EditorSearchBar::OnChange");
		string filter = EditBoxWidget.Cast(w).GetText();
		
		Widget child = m_SearchContext.GetChildren();
		while (child != null) {
			EditorListItem editor_list_item;
			child.GetScript(editor_list_item);
			
			if (editor_list_item != null) {
				string Config_Lower = editor_list_item.GetObjectName();
				Config_Lower.ToLower();
	           	child.Show(!(filter != "" && !Config_Lower.Contains(filter)));				
			}
						
			child = child.GetSibling();
		}
		
		return super.OnChange(w, x, y, finished);
	}
	
}


class EditorUICartesian: ScriptedWidgetEventHandler
{
	static ItemPreviewWidget m_Root;
	//protected ItemPreviewWidget m_EditorUICartesian;
	
	static EntityAI m_UICartiesian;
	
	void EditorUICartesian()
	{
		Print("EditorUICartesian");
		m_UICartiesian = GetGame().CreateObject("BoundingBoxBase", "0 0 0");
		m_UICartiesian.SetOrientation(vector.Up);
		m_UICartiesian.SetOrigin(vector.Zero);
	}
	
	void ~EditorUICartesian()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		GetGame().ObjectDelete(m_UICartiesian);
	}
	
	protected void OnWidgetScriptInit(Widget w)
	{
		Print("EditorUICartesian::OnWidgetScriptInit");
		m_Root = ItemPreviewWidget.Cast(w);
		m_Root.SetHandler(this);
		m_Root.SetItem(m_UICartiesian);
		m_Root.SetForceFlipEnable(true);
		m_Root.SetForceFlip(true);
		m_Root.SetView(0);
		//m_Root.SetView(m_Root.GetItem().GetViewIndex());
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void Update()
	{
		//vector v = GetGame().GetCurrentCameraDirection();	
		//m_Root.SetModelOrientation(v);
	}
	
	
}
