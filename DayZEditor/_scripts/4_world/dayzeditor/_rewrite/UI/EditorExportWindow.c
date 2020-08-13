

class EditorExportWindow: UIScriptedMenu
{
	static ref EditorExportWindow m_Instance;
	static EditorExportWindow GetInstance() { return m_Instance; }
	
	
	protected ButtonWidget 	m_TitleClose
	protected ButtonWidget	m_CloseButton;
	protected TextWidget 	m_TitleText;
	
	void EditorExportWindow()
	{
		m_Instance = this;
	}
	
	override Widget Init()
	{
		super.Init();
		layoutRoot 			= GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/BaseWidgets/EditorExportWindow.layout");
		
		m_TitleText 		= TextWidget.Cast(layoutRoot.FindAnyWidget("TitleText"));
		m_TitleClose 		= ButtonWidget.Cast(layoutRoot.FindAnyWidget("TitleClose"));
		m_CloseButton 		= ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseButton"));
		
		m_TitleText.SetText("Export Data...");		
		
		return layoutRoot;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Print("EditorExportWindow::OnMouseButtonDown");
		if (button == 0) {
			if (w == m_TitleClose || w == m_CloseButton)
				Close();
		}
		return true;
	}
	
}



class EditorCloseButton: ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	
	void ~EditorCloseButton()
	{
		delete m_Root;
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
		m_Root.SetHandler(this);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorCloseButton::OnClick");
		delete EditorExportWindow.GetInstance();
		return true;
	}
}

Widget RecursivelyGetParent(string parent_name, Widget start_widget)
{
	Widget parent = start_widget.GetParent();
	if (parent == null) return start_widget;
	if (parent.GetName() == parent_name) {
		return parent;
	} else return RecursivelyGetParent(parent_name, parent);	
}