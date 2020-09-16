
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
		
		m_EditorHud.SetMenu(this);
	}
	
	override void Close()
	{
		EditorLog.Trace("EditorMenu::Close");
		super.Close();
		
		m_EditorHud.SetMenu(null);
	}
	
	void SetPosition(float x, float y) {
		EditorMenuContent.SetPos(x, y);
	}
	
	void AddItem(string title, string icon = "")
	{
		EditorMenuItem menu_item();
		menu_item.SetText(title);
		menu_item.SetIcon(icon);
		AddItem(menu_item);
	}
	
	void AddItem(ref EditorMenuItem menu_item)
	{
		if (menu_item) {
			EditorMenuContent.AddChild(menu_item.GetLayoutRoot());
			m_MenuItems.Insert(menu_item);
		}
		Print(m_MenuItems.Count());
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


class EditorMenuItem: EditorMVCLayout
{
	protected TextWidget EditorMenuItemLabel;
	protected ImageWidget EditorMenuItemIcon;
	
	void EditorMenuItem() {
		EditorLog.Trace("EditorMenuItem");
	}
	
	void ~EditorMenuItem() {
		EditorLog.Trace("~EditorMenuItem");
	}
	
	void SetText(string text)
	{
		if (EditorMenuItemLabel)
			EditorMenuItemLabel.SetText(text);
	}
	
	void SetIcon(string icon)
	{
		if (EditorMenuItemIcon) {
			EditorMenuItemIcon.LoadImageFile(0, icon);
			EditorMenuItemIcon.SetImage(0);
		}
	}
	
	void SetIcon(int image, RTTextureWidget texture)
	{
		if (EditorMenuItemIcon) {
			EditorMenuItemIcon.SetImageTexture(image, texture);
		}
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/menus/EditorMenuItem.layout";
	}
	
}
