class EditorDialogController: ViewController
{
	ref ObservableCollection<ref TreeView> Folders = new ObservableCollection<ref TreeView>(this);
}

class EditorDialog: ScriptViewMenu
{
	static ref EditorDialog Instance;
	
	TextWidget MachineName;
	
	Widget Header, Navigation, Inner, Outline, Footer, Drag, DragTexture, Tree, Files;
	
	Widget North, South, East, West;
	TextWidget HeaderText;
	
	void EditorDialog()
	{
		MachineName.SetText(GetMachineName());
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
				
		float x, y;
		m_LayoutRoot.GetScreenPos(x, y);		
			
		float w, h;
		m_LayoutRoot.GetScreenSize(w, h);
		
		float footer_w, footer_h;
		Footer.GetScreenSize(footer_w, footer_h);
		
		// nav + header = 64
		Inner.SetSize(1.0, h - footer_h - 64);
		Inner.Update();
		
		float drag_x, drag_y;
		Drag.GetPos(drag_x, drag_y);
		
		Tree.SetSize(drag_x, 1.0);
		Files.SetSize(1.0 - drag_x, 1.0);
		
		Drag.Update();
	}
	
	void OnBackExecute(ButtonCommandArgs args)
	{
	}
	
	void OnForwardExecute(ButtonCommandArgs args)
	{
	}
	
	void OnNavBackExecute(ButtonCommandArgs args)
	{
		
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		return false;
	}
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{				
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{						
		return false;
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{				
		return false;
	}
	
	override bool OnDrag(Widget w, int x, int y)
	{		
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{				
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		switch (w) {
			case Drag: {
				WidgetAnimator.AnimateColor(DragTexture, EditorColor.SELECT, 50);
				break;
			}
			
			case North:
			case South: {
				
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{			
		switch (w) {
			case Drag: {
				WidgetAnimator.AnimateColor(DragTexture, ARGB(200, 255, 255, 255), 50);
				break;
			}
		}
			
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		return true;
	}
		
	override typename GetControllerType()
	{
		return EditorDialogController;
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\Dialog.layout";
	}
}