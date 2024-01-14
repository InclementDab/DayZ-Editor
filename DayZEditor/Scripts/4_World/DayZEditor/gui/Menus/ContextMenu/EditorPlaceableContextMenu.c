/*class EditorPlaceableContextMenu: EditorContextMenu
{
	void EditorPlaceableContextMenu(float x, float y, EditorItemTreeItem context)
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		
		if (context.GetTemplateController().Favorite) {
			m_Editor.CommandManager[EditorRemoveFromFavoritesCommand].SetData(new Param1<EditorItemTreeItem>(context));
			AddMenuButton(m_Editor.CommandManager[EditorRemoveFromFavoritesCommand]);
		} else {
			m_Editor.CommandManager[EditorAddToFavoritesCommand].SetData(new Param1<EditorItemTreeItem>(context));
			AddMenuButton(m_Editor.CommandManager[EditorAddToFavoritesCommand]);
		}
		
		AddMenuButton(m_Editor.CommandManager[EditorLootEditorCommand]);
	}
}