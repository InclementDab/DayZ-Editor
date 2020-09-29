
class EditorDialogButton: EditorScriptView
{	
	protected ButtonWidget DialogButton;
	protected ref ScriptInvoker m_ExecuteInvoker = new ScriptInvoker();
	
	protected string m_ExecuteAction;
	
	void EditorDialogButton(Widget parent = null)
	{
		EditorLog.Trace("EditorDialogButton"); 
	}
	
	void ~EditorDialogButton()
	{
		EditorLog.Trace("~EditorDialogButton");
		delete m_ExecuteInvoker;
	}
	
	bool DialogButtonExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("DialogButtonExecute");
		
		if (m_ExecuteAction == string.Empty) {
			m_ExecuteInvoker.Invoke();
		} else {
			g_Script.Call(GetParent(), m_ExecuteAction, args);
		}
		return true;
	}
	
	void SetExecuteFunction(func function)
	{
		m_ExecuteInvoker.Insert(function);
	}
	
	void SetExecuteFunctionString(string function)
	{
		m_ExecuteAction = function;
	}
	
	void SetLabel(string label)
	{
		DialogButton.SetText(label);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialogButton.layout";
	}
}

class EditorDialog: EditorScriptView
{				
	protected WrapSpacerWidget DialogContent;
	protected GridSpacerWidget ButtonGrid;
	protected WrapSpacerWidget WindowDragWrapper;
	
	protected ref array<ref EditorDialogButton> m_DialogButtons = {};
	
	void EditorDialog(Widget parent = null) 
	{
		m_LayoutRoot.Show(false);
	}
	
	void ~EditorDialog() 
	{
		delete m_DialogButtons;
		m_Editor.GetCamera().MoveEnabled = true;
		m_Editor.GetCamera().LookEnabled = true;
		m_EditorHud.ShowCursor(true);
	}

	protected void AddContent(string layout) {
		AddContent(GetGame().GetWorkspace().CreateWidgets(layout));
	}
	
	// todo use a collection for this
	protected void AddContent(ScriptView prefab) {
		AddContent(prefab.GetLayoutRoot());
	}
	
	protected void AddContent(Widget content) {
		DialogContent.AddChild(content);
		GetController().LoadDataBindings(DialogContent);
	}

	protected EditorDialogButton AddButton(string label, func action)
	{
		EditorDialogButton dialog_button = new EditorDialogButton(ButtonGrid);
		dialog_button.SetParent(this);
		dialog_button.SetExecuteFunction(action);
		dialog_button.SetLabel(label);
		m_DialogButtons.Insert(dialog_button);
		return dialog_button;
	}
	
	protected EditorDialogButton AddButtonStringAction(string label, string action)
	{
		EditorDialogButton dialog_button = new EditorDialogButton(ButtonGrid);
		dialog_button.SetParent(this);
		dialog_button.SetExecuteFunctionString(action);
		dialog_button.SetLabel(label);
		m_DialogButtons.Insert(dialog_button);
		return dialog_button;
	}

	void SetTitle(string title)
	{
		EditorDialogController controller = EditorDialogController.Cast(GetController());
		controller.TitleText = title;
		controller.NotifyPropertyChanged("TitleText");
	}
	
	DialogResult ShowDialog()
	{
		EditorLog.Trace("EditorDialog::Show");
		m_Editor.GetCamera().MoveEnabled = false;
		m_Editor.GetCamera().LookEnabled = false;
		m_EditorHud.ShowCursor(true);
		
		EditorUIManager.CurrentDialog = this;
		
		float du, dv, dx, dy;
		m_LayoutRoot.GetScreenSize(du, dv);		
		m_LayoutRoot.GetPos(dx, dy);
		m_LayoutRoot.SetPos(dx, dy - dv / 2);
		m_LayoutRoot.Show(true);
		
		// todo
		return DialogResult.OK;
	}
	
	void CloseDialog()
	{
		EditorLog.Trace("EditorDialog::Close");
		delete this;
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
		
		return false;
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
	
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/EditorDialog.layout";
	}
	
	override typename GetControllerType() {
		return EditorDialogController;
	}
}
