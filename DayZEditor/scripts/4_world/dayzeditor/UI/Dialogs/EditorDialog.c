


class EditorDialog extends ScriptedWidgetEventHandler
{
	protected ref Widget m_Root;
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
	
	void ~EditorDialog()
	{
		Print("~EditorDialog");
	}
	
	
	protected void SetContent(Widget content)
	{
		m_ContentWrapper.AddChild(content);
	}
	
	protected ButtonWidget AddButton(string label) 
	{
		Widget panel = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout", m_ButtonGrid);		
		ButtonWidget btn = ButtonWidget.Cast(panel.FindAnyWidget("Button"));
		btn.SetText(label);
		return btn;
	}
	
	protected void AddWidget(Widget target)
	{
		m_ButtonGrid.AddChild(target);
	}
	
	
	protected void SetTitleText(string text)
	{
		m_TitleText.SetText(text);
	}
	
	void ShowDialog()
	{
		Print("MapSelectDialog::ShowDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
		m_Root.Show(true);
		EditorUIManager.ModalSet(m_Root);
	}
	
	void CloseDialog()
	{
		Print("MapSelectDialog::CloseDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		m_Root.Show(false);
		EditorUIManager.ModalClose();
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
