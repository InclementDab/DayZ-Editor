
class EditorDialog: EditorMVCLayout
{	
	protected ref EditorDialogController m_DialogController;
	EditorDialogController GetDialogController() {
		return EditorDialogController.Cast(m_Controller);
	}
			
	protected WrapSpacerWidget DialogContent;
	protected GridSpacerWidget ButtonGrid;
	protected WrapSpacerWidget WindowDragWrapper;
	
	void EditorDialog() 
	{
		EditorLog.Trace("EditorDialog");
		
		m_LayoutRoot.Show(false);
		m_DialogController = GetDialogController();
	}
	
	void ~EditorDialog() 
	{
		EditorLog.Trace("~EditorDialog");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}

	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialog.layout";
	}
	
	override typename GetControllerType() {
		return EditorDialogController;
	}
	
	protected Controller AddContent(Widget content)
	{
		DialogContent.AddChild(content);
		Controller controller;
		content.GetScript(controller);		
		return controller;
	}
	
	protected Controller AddContent(string layout)
	{
		Widget content = GetGame().GetWorkspace().CreateWidgets(layout);
		DialogContent.AddChild(content);
		Controller controller;
		content.GetScript(controller);		
		return controller;
	}
	
	protected Controller AddContent(EditorPrefab prefab)
	{
		DialogContent.AddChild(prefab.GetLayoutRoot());
		Controller controller;
		prefab.GetLayoutRoot().GetScript(controller);
		return controller;
	}
	
	
	protected ButtonWidget AddButton(string label, string callback) 
	{
		Widget panel = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout", ButtonGrid);		
		EditorDialogButtonHandler handler;
		panel.GetScript(handler);
		handler.SetLabel(label);
		handler.SetCallback(this, callback);
		return panel.FindAnyWidget("DialogButton");
	}
	

	protected void SetTitle(string title)
	{
		m_DialogController.TitleText = title;
		m_DialogController.NotifyPropertyChanged("TitleText");
	}
		
	
	override void Show()
	{
		EditorLog.Trace("EditorDialog::Show");
		super.Show();
				
		m_Editor.GetCamera().SetMoveEnabled(false);
		m_Editor.GetCamera().SetLookEnabled(false);
		m_EditorHud.ShowCursor();
		
		m_EditorHud.SetModal(this);
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
	}
	
	override void Close()
	{
		EditorLog.Trace("EditorDialog::Close");
		super.Close();
		
		m_EditorHud.SetModal(null);
		m_Editor.GetCamera().SetMoveEnabled(true);
		m_Editor.GetCamera().SetLookEnabled(true);
		m_EditorHud.ShowCursor();
	}
		
	float m_OffsetX, m_OffsetY;
	override bool OnDrag(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorDialog::OnDrag");
		if (w == WindowDragWrapper) {
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			m_OffsetX -= x; m_OffsetY -= y;		
			g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);	
		}
		
		return super.OnDrag(w, x, y);
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever) { 
		return true; 
	}
	
	private bool DragUpdate()
	{
		int x, y;
		GetMousePos(x, y);
		m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		EditorLog.Trace("EditorDialog::OnDrop");
		g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		
		if (w == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
	    }		
		
		return false;
	}
}
