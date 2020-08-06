

static const ref array<string> ExcludedMapItems = {
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};


class MapSelectWindow: UIScriptedMenu
{	
	protected Widget m_SelectButtonPanel;
	protected ButtonWidget m_SelectButton;
	protected TextListboxWidget m_MapHostListbox;
	
	protected Widget m_TitleBar;
	protected Widget m_WindowDragWrapper;

	override Widget Init() 
	{
		layoutRoot 					= GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorMapSelector.layout");
		m_WindowDragWrapper 		= layoutRoot.FindAnyWidget("WindowDragWrapper");
		
		m_TitleBar 					= layoutRoot.FindAnyWidget("WindowDragWrapper");
		m_SelectButtonPanel 		= layoutRoot.FindAnyWidget("SelectButtonPanel");
		
		m_SelectButton 				= ButtonWidget.Cast(layoutRoot.FindAnyWidget("SelectButton"));
		m_MapHostListbox 			= TextListboxWidget.Cast(layoutRoot.FindAnyWidget("MapHostListbox"));
		
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (ExcludedMapItems.Find(name) == -1) {
				m_MapHostListbox.AddItem(name, null, 0);
			}
		}
		
		return layoutRoot;
	}
	
	override void Update(float timeslice)
	{
		m_TitleBar.SetPos(0, 0);
		bool lots_of_branches_on_your_penis = m_MapHostListbox.GetSelectedRow() != -1;
		if (lots_of_branches_on_your_penis)
			m_SelectButton.SetAlpha(1);
		else m_SelectButton.SetAlpha(0.5); 
		
		m_SelectButton.Enable(lots_of_branches_on_your_penis);
		
	}
	
	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		Print("MapSelectWindow::OnKeyPress");
		
		switch (key) {
			
			case KeyCode.KC_ESCAPE: {
				Close();
				return true;
			}
			
			
		}
		
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("MapSelectWindow::OnClick");
		
		if (button != 0) return false; 
		
		if (w == layoutRoot.FindAnyWidget("TitleClose") || w == layoutRoot.FindAnyWidget("CloseButton")) {
			Close();
			return true;

		} else if (w == m_SelectButton) {
			string name;
			m_MapHostListbox.GetItemText(m_MapHostListbox.GetSelectedRow(), 0, name);
			GetGame().PlayMission(CreateEditorMission(name));
			Close();
			return true;
		}
		

		
		return false;
	}
	
	float m_OffsetX, m_OffsetY;
	
	override bool OnDrag(Widget w, int x, int y) 
	{
		if (w == m_TitleBar) {
			
			layoutRoot.GetPos(m_OffsetX, m_OffsetY);
			
			m_OffsetX = x - m_OffsetX;
			m_OffsetY = y - m_OffsetY;
	
			m_TitleBar.SetPos(0, 0, true);
			m_TitleBar.SetPos(0, 0, false);
			
			return true;
		}
		
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		if (w == m_TitleBar) {

			layoutRoot.SetPos(x - m_OffsetX, y - m_OffsetY, true);
			m_TitleBar.SetPos(0, 0, true);
			return true;
		}
		
		return true;//super.OnDragging(w, x, y, reciever);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
	    if (w == m_TitleBar) {
	        m_TitleBar.SetPos(0, 0);
			layoutRoot.SetPos(x - m_OffsetX, y - m_OffsetY);
			
			//m_WindowDragWrapper.SetPos(x - m_OffsetX, y - m_OffsetY);
	        
	        return true;
	    }
	    
	    return false;
	}
	
}