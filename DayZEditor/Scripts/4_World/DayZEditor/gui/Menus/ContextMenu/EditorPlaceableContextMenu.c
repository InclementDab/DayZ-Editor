class EditorPlaceableContextMenu: EditorContextMenu
{
	void EditorPlaceableContextMenu(float x, float y, EditorPlaceableListItem context)
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		
		if (context.GetTemplateController().Favorite) {
			m_Editor.CommandManager[EditorRemoveFromFavoritesCommand].SetData(new Param1<EditorPlaceableListItem>(context));
			AddMenuButton(m_Editor.CommandManager[EditorRemoveFromFavoritesCommand]);
		} else {
			m_Editor.CommandManager[EditorAddToFavoritesCommand].SetData(new Param1<EditorPlaceableListItem>(context));
			AddMenuButton(m_Editor.CommandManager[EditorAddToFavoritesCommand]);
		}
		
		AddMenuButton(m_Editor.CommandManager[EditorLootEditorCommand]);
	}
}