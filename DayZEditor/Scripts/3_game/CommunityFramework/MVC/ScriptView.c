
// Abstract Class
// Inherit from this if you want to create UI elements with 100% script
/*

Example:

class CustomDialogWindow: ScriptView
{
	override string GetLayoutFile() {
		return "MyMod/gui/Layouts/dialogs/Dialog.layout";
	}
}

	CustomDialogWindow window = new CustomDialogWindow();
	window.Show();
	....
	....
	window.Close();

*/

class ScriptView: ScriptedViewBase
{	
	protected ref Controller m_Controller;
	Controller GetController() {
		return m_Controller;
	}
	
	void ScriptView(Widget parent = null)
	{
		//Debug_Logging = true;
		Trace("ScriptView");
		if (!GetLayoutFile()) {
			Error("Layout file not found! Are you overriding GetLayoutFile?");
			return;
		}
		
		WorkspaceWidget workspace = GetWorkbenchGame().GetWorkspace();
		if (!workspace) {
			Error("Workspace was null, try reloading Workbench");
			return;
		}
		
		Log("Loading %1", GetLayoutFile());
		m_LayoutRoot = workspace.CreateWidgets(GetLayoutFile(), parent);
		if (!m_LayoutRoot) {
			Error("Invalid layout file %1", GetLayoutFile());
			return;
		}
		
		m_LayoutRoot.GetScript(m_Controller);
		if (!m_Controller || !m_Controller.IsInherited(Controller)) {

			if (!GetControllerType().IsInherited(Controller)) {
				Error("ScriptView: %1 is invalid. Must inherit from Controller!", GetControllerType().ToString());
				return;
			}
			
			m_Controller = GetControllerType().Spawn();
			m_Controller.Debug_Logging = Debug_Logging;
			m_Controller.OnWidgetScriptInit(m_LayoutRoot);
			m_Controller.SetParent(this);
			//m_LayoutRoot.SetHandler(this);
		}
	
		PropertyTypeHashMap property_map = PropertyTypeHashMap.FromType(Type());
		//property_map.RemoveType(ScriptView); crashing ?
		foreach (string property_name, typename property_type: property_map) {
			
			if (property_type.IsInherited(Widget)) {
				Widget target = m_LayoutRoot.FindAnyWidget(property_name);
				// fixes bug that breaks everything
				if (target && m_LayoutRoot.GetName() != property_name) {
					EnScript.SetClassVar(this, property_name, 0, target);
				}
				
				// Allows you to define the layout root aswell within it
				if (!target && m_LayoutRoot.GetName() == property_name) {
					EnScript.SetClassVar(this, property_name, 0, m_LayoutRoot);
				}
			}
		}
	}
	

	void ~ScriptView()
	{
		Trace("~ScriptView");
		delete m_Controller;
	}
	
	void SetParent(Widget parent)
	{
		WorkspaceWidget workspace = GetWorkbenchGame().GetWorkspace();
		if (!workspace) {
			Error("Workspace was null, try reloading Workbench");
			return;
		}
		
		if (m_LayoutRoot && m_LayoutRoot.GetParent()) {
			m_LayoutRoot.Unlink();
		}
		
		m_LayoutRoot = workspace.CreateWidgets(GetLayoutFile(), parent);
	}
		
	// Useful if you want to set to an existing controller
	void SetController(Controller controller)
	{
		m_Controller = controller;
		m_Controller.Debug_Logging = Debug_Logging;
		m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		m_Controller.SetParent(this);
	}
	
		
	// Abstract Methods
	protected string GetLayoutFile();
	protected typename GetControllerType() {
		return Controller;
	}
}




