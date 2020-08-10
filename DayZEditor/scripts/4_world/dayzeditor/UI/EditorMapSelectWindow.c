

static const ref array<string> ExcludedMapItems = {
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};



class EditorDialog: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	protected TextWidget m_TitleText;
	protected ButtonWidget m_TitleClose;
	protected WrapSpacerWidget m_ContentWrapper;
	
	void EditorDialog()
	{
		Print("EditorDialog");
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialog.layout");
		m_LayoutRoot.SetHandler(this);
		m_LayoutRoot.Show(false);
		
		m_TitleText 		= TextWidget.Cast(m_LayoutRoot.FindAnyWidget("TitleText"));
		m_TitleClose		= ButtonWidget.Cast(m_LayoutRoot.FindAnyWidget("TitleClose"));
		m_ContentWrapper	= WrapSpacerWidget.Cast(m_LayoutRoot.FindAnyWidget("DialogContent"));
		
	}
	
	protected void SetContent(Widget content)
	{
		m_ContentWrapper.AddChild(content);
	}
	
	void ShowDialog()
	{
		Print("MapSelectDialog::ShowDialog");
		m_LayoutRoot.Show(true);
		GetEditor().GetUIManager().ModalSet(m_LayoutRoot);
	}
	
	void CloseDialog()
	{
		Print("MapSelectDialog::CloseDialog");
		m_LayoutRoot.Show(false);
		GetEditor().GetUIManager().ModalClose();
	}
}



class MapSelectDialog: EditorDialog
{	
	
	protected TextListboxWidget m_MapHostListbox;


	
	void MapSelectDialog()
	{

		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorMapSelector.layout");
		SetContent(content);
		m_MapHostListbox = TextListboxWidget.Cast(m_LayoutRoot.FindAnyWidget("MapHostListbox"));
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (ExcludedMapItems.Find(name) == -1) {
				m_MapHostListbox.AddItem(name, null, 0);
			}
		}
	}
	
	
	void Update(float timeslice)
	{
		/*
		m_TitleBar.SetPos(0, 0);
		bool lots_of_branches_on_your_penis = m_MapHostListbox.GetSelectedRow() != -1;
		if (lots_of_branches_on_your_penis)
			m_SelectButton.SetAlpha(1);
		else m_SelectButton.SetAlpha(0.5); 
		
		m_SelectButton.Enable(lots_of_branches_on_your_penis);
		*/
	}
	
	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		Print("MapSelectWindow::OnKeyPress");
		
		switch (key) {
			
			case KeyCode.KC_ESCAPE: {
				CloseDialog();
				return true;
			}
			
			
		}
		
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("MapSelectWindow::OnClick");
		
		if (button != 0) return false; 
		
		if (w == m_LayoutRoot.FindAnyWidget("TitleClose") || w == m_LayoutRoot.FindAnyWidget("CloseButton")) {
			CloseDialog();
			return true;

		}/*else if (w == m_SelectButton) {
			string name;
			m_MapHostListbox.GetItemText(m_MapHostListbox.GetSelectedRow(), 0, name);
			GetGame().PlayMission(CreateEditorMission(name));
			CloseDialog();
			return true;
		}*/
		

		
		return false;
	}
	
	float m_OffsetX, m_OffsetY;
	
	override bool OnDrag(Widget w, int x, int y) 
	{/*
		if (w == m_TitleBar) {
			
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			
			m_OffsetX = x - m_OffsetX;
			m_OffsetY = y - m_OffsetY;
	
			m_TitleBar.SetPos(0, 0, true);
			m_TitleBar.SetPos(0, 0, false);
			
			return true;
		}
		*/
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{/*
		if (w == m_TitleBar) {

			m_LayoutRoot.SetPos(x - m_OffsetX, y - m_OffsetY, true);
			m_TitleBar.SetPos(0, 0, true);
			return true;
		}
		*/
		return true;//super.OnDragging(w, x, y, reciever);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{/*
	    if (w == m_TitleBar) {
	        m_TitleBar.SetPos(0, 0);
			m_LayoutRoot.SetPos(x - m_OffsetX, y - m_OffsetY);
			
			//m_WindowDragWrapper.SetPos(x - m_OffsetX, y - m_OffsetY);
	        
	        return true;
	    }*/
	    
	    return false;
	}
	
	
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{		
		if (button != 0) return false; 
		
		if ( button == MouseState.LEFT )
		{
			if ( w == m_MapHostListbox )
			{
				string name;
				m_MapHostListbox.GetItemText(m_MapHostListbox.GetSelectedRow(), 0, name);
				if(name != "")
				{
					GetGame().PlayMission(CreateEditorMission(name));
					CloseDialog();
					return true;
				}
			}
		}
		
		return super.OnDoubleClick( w, x, y, button );
	}
}