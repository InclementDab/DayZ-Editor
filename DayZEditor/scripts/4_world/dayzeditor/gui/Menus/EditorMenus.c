

class EditorFileMenu: EditorMenu
{
	void EditorFileMenu()
	{
		EditorLog.Trace("EditorFileMenu");
		
		AddItem("New", "EditorFileMenuNewCommand");
		AddItem("Open", "EditorFileMenuOpenCommand");
		
		AddItem("Save", "EditorFileMenuSaveCommand");
		AddItem("Save As...", "EditorFileMenuSaveAsCommand");
		AddItem("Close", "EditorFileMenuCloseCommand");
	}
}

class EditorEditMenu: EditorMenu
{
	void EditorEditMenu()
	{
		EditorLog.Trace("EditorEditMenu");
	}
}


class EditorMenu: EditorMVCLayout
{
	private ref array<ref EditorMenuItem> m_MenuItems = {};
	protected WrapSpacerWidget EditorMenuContent;
	
	void EditorMenu() {
		EditorLog.Trace("EditorMenu");
		if (m_LayoutRoot) {
			m_LayoutRoot.Show(false);
		}
	}
	
	void ~EditorMenu() {
		EditorLog.Trace("~EditorMenu");
		delete m_MenuItems;
	}
	
	override void Show()
	{
		EditorLog.Trace("EditorMenu::Show");
		super.Show();
		
		m_LayoutRoot.Show(true);
		m_EditorHud.SetMenu(this);
	}
	
	override void Close()
	{
		EditorLog.Trace("EditorMenu::Close");
		super.Close();
	}
	
	void SetPosition(float x, float y) {
		m_LayoutRoot.SetPos(x, y);
	}
	
	void AddItem(string title, string relay_command = "", string icon = "")
	{
		EditorMenuItem menu_item = new EditorMenuItem();
		menu_item.SetText(title);
		menu_item.SetIcon(icon);
		
		ViewBinding view_binding;
		menu_item.GetLayoutRoot().FindAnyWidget("EditorMenuItemButton").GetScript(view_binding);
		if (view_binding && relay_command != string.Empty) {
			view_binding.SetRelayCommand(relay_command);
		}
		
		AddItem(menu_item);
	}
	
	void AddItem(ref EditorMenuItem menu_item)
	{
		if (menu_item) {
			EditorMenuContent.AddChild(menu_item.GetLayoutRoot());
			m_MenuItems.Insert(menu_item);
		}
	}
		
	void RemoveItem(ref EditorMenuItem menu_item)
	{
		EditorMenuContent.RemoveChild(menu_item.GetLayoutRoot());
		m_MenuItems.Remove(m_MenuItems.Find(menu_item));
		delete menu_item;
	}
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenu.layout";
	}
}


class EditorMenuItemController: Controller
{
	string LabelText;
	string IconPath;
}

class EditorMenuItem: EditorMVCLayout
{
	protected ref EditorMenuItemController m_EditorMenuItemController;
	
	void EditorMenuItem() {
		EditorLog.Trace("EditorMenuItem");
		if (m_LayoutRoot) {
			m_LayoutRoot.Show(true);
		}
		
		m_EditorMenuItemController = EditorMenuItemController.Cast(GetController());
	}
	
	void ~EditorMenuItem() {
		EditorLog.Trace("~EditorMenuItem");
	}
	
	void SetText(string text) {
		m_EditorMenuItemController.LabelText = text;
		m_EditorMenuItemController.NotifyPropertyChanged("LabelText");
	}
	
	void SetIcon(string icon) {
		m_EditorMenuItemController.IconPath = icon;
		m_EditorMenuItemController.NotifyPropertyChanged("IconPath");
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenuItem.layout";
	}
	
	override typename GetControllerType() {
		return EditorMenuItemController;
	}
	
}
