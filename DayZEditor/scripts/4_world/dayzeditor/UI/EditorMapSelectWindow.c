

static const ref array<string> ExcludedMapItems = {
	"access",
	"DefaultLighting",
	"DefaultWorld",
	"initWorld",
	"CAWorld"
};






class EditorDialog extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	Widget GetRoot() { return m_Root; }
	protected Widget m_TitleBar;
	
	protected TextWidget m_TitleText;
	protected ButtonWidget m_TitleClose;
	
	protected WrapSpacerWidget m_ContentWrapper;
	protected GridSpacerWidget m_ButtonGrid;
	protected WrapSpacerWidget m_WindowDragWrapper;

	
	void EditorDialog()
	{
		Print("EditorDialog");
		
		m_Root = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialog.layout", null);
		m_Root.Show(false);
			
		m_TitleBar			= m_Root.FindAnyWidget("TitleBar");
		m_TitleText 		= TextWidget.Cast(m_Root.FindAnyWidget("TitleText"));
		m_TitleClose		= ButtonWidget.Cast(m_Root.FindAnyWidget("TitleClose"));
		m_ContentWrapper	= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("DialogContent"));
		m_ButtonGrid		= GridSpacerWidget.Cast(m_Root.FindAnyWidget("ButtonGrid"));
		m_WindowDragWrapper	= WrapSpacerWidget.Cast(m_Root.FindAnyWidget("WindowDragWrapper"));
		
		m_Root.SetHandler(this);
	}
	
	
	
	protected void SetContent(Widget content)
	{
		m_ContentWrapper.AddChild(content);
	}
	
	protected ButtonWidget AddButton(string label) 
	{
		Widget panel = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout", m_ButtonGrid);		
		TextWidget txt = TextWidget.Cast(panel.FindAnyWidget("ButtonLabel"));
		txt.SetText(label);
		return ButtonWidget.Cast(panel.FindAnyWidget("Button"));
	}
	
	void ShowDialog()
	{
		Print("MapSelectDialog::ShowDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		m_Root.Show(true);
		GetEditor().GetUIManager().ModalSet(this);
	}
	
	void CloseDialog()
	{
		Print("MapSelectDialog::CloseDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		m_Root.Show(false);
		GetEditor().GetUIManager().ModalClose();
	}
	
	void Update() {}

	float m_OffsetX, m_OffsetY;
	override bool OnDrag(Widget w, int x, int y) 
	{
		if (w == m_TitleBar) {
			
			m_Root.GetPos(m_OffsetX, m_OffsetY);
			
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

			m_Root.SetPos(x - m_OffsetX, y - m_OffsetY, true);
			m_TitleBar.SetPos(0, 0, true);
			return true;
		}
		
		return true;//super.OnDragging(w, x, y, reciever);
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
	    if (w == m_TitleBar) {
	        m_TitleBar.SetPos(0, 0);
			m_Root.SetPos(x - m_OffsetX, y - m_OffsetY);
			
			m_WindowDragWrapper.SetPos(x - m_OffsetX, y - m_OffsetY);
	        
	        return true;
	    }
	    
	    return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("EditorDialog::OnClick");
		
		if (button != 0) return false; 
		
		if (w == m_TitleClose) {
			CloseDialog();
			return true;
		}
		

		
		return super.OnClick(w, x, y, button);
	}

	
}



class MapSelectDialog: EditorDialog
{	
	
	protected TextListboxWidget m_MapHostListbox;
	
	private ref ButtonWidget m_SelectButton;
	private ref ButtonWidget m_CloseButton;
		
	void MapSelectDialog()
	{
		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorMapSelector.layout", m_ContentWrapper);
		//SetContent(content);
		
		m_SelectButton = AddButton("Select");
		m_CloseButton = AddButton("Close");
		m_MapHostListbox = TextListboxWidget.Cast(m_Root.FindAnyWidget("MapHostListbox"));
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (ExcludedMapItems.Find(name) == -1) {
				m_MapHostListbox.AddItem(name, null, 0);
			}
		}
	}
	
	
	
	override void Update()
	{
		m_TitleBar.SetPos(0, 0);
		bool lots_of_branches_on_your_penis = m_MapHostListbox.GetSelectedRow() != -1;
		if (lots_of_branches_on_your_penis)
			m_SelectButton.SetAlpha(1);
		else m_SelectButton.SetAlpha(0.5); 
		
		m_SelectButton.Enable(lots_of_branches_on_your_penis);
		
	}
	



	override bool OnClick(Widget w, int x, int y, int button)
	{
		Print("MapSelectDialog::OnClick");
		
		if (button != 0) return false; 
		
		if (w == m_SelectButton) {
			string name;
			m_MapHostListbox.GetItemText(m_MapHostListbox.GetSelectedRow(), 0, name);
			GetGame().PlayMission(CreateEditorMission(name));
			CloseDialog();
			return true;
		} 
		
		if (w == m_CloseButton) {
			CloseDialog();
			return true;
		}
	
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{		
		if (button != 0) return false; 
		

		if (w == m_MapHostListbox) {
			string name;
			m_MapHostListbox.GetItemText(m_MapHostListbox.GetSelectedRow(), 0, name);
			if (name != "") {
				GetGame().PlayMission(CreateEditorMission(name));
				CloseDialog();
				return true;
			}
		}
		
		
		return super.OnDoubleClick( w, x, y, button );
	}
	

}