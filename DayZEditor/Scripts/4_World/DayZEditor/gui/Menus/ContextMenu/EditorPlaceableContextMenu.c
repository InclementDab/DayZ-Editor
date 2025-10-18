class EditorPlaceableContextMenu: EditorContextMenu
{
	void EditorPlaceableContextMenu(float x, float y, EditorPlaceableItem context)
	{
		EditorLog.Trace("EditorPlacedContextMenu");
		
		/*if (context.GetTemplateController().Favorite) {
			m_Editor.CommandManager[EditorRemoveFromFavoritesCommand].SetData(new Param1<EditorPlaceableItem>(context));
			AddMenuButton(m_Editor.CommandManager[EditorRemoveFromFavoritesCommand]);
		} else {
			m_Editor.CommandManager[EditorAddToFavoritesCommand].SetData(new Param1<EditorPlaceableItem>(context));
			AddMenuButton(m_Editor.CommandManager[EditorAddToFavoritesCommand]);
		}*/
		
		AddMenuButton(m_Editor.CommandManager[EditorLootEditorCommand]);
		
		m_Editor.CommandManager[EditorCopyPlaceableToClipboard].SetData(new Param1<EditorPlaceableItem>(context));
		AddMenuButton(m_Editor.CommandManager[EditorCopyPlaceableToClipboard]);
	}
}