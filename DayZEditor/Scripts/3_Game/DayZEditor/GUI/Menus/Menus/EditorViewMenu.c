class EditorViewMenu: EditorMenu
{	
	void EditorViewMenu()
	{
		EditorLog.Trace("EditorViewMenu");

		AddMenuButton(m_Editor.CommandManager[EditorReloadHudCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorDumpSceneCommand]);
		AddMenuButton(m_Editor.CommandManager[EditorCameraControlsCommand]);
	}
}