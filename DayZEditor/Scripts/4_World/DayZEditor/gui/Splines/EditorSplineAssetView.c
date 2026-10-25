// One asset of a generator list, with a tag when it cannot be used on console
class EditorSplineAssetView: ScriptView
{
	protected string m_Field;
	protected int m_Index;
	protected ref ScriptCaller m_OnRemoved;

	TextWidget Tag, Label;
	ButtonWidget RemoveButton;

	void EditorSplineAssetView(string field, int index, string asset, ScriptCaller on_removed)
	{
		m_Field = field;
		m_Index = index;
		m_OnRemoved = on_removed;

		Label.SetText(asset);
		EditorSplineAssetInfo asset_info = EditorSplineAssetCatalog.Get(asset);
		if (!asset_info) {
			return;
		}

		Label.SetText(asset_info.GetDisplayName());
		if (!asset_info.Valid) {
			Tag.SetText("Not Found");
		} else if (!asset_info.ConsoleFriendly) {
			Tag.SetText("PC Only");
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == RemoveButton && button == 0 && m_OnRemoved) {
			m_OnRemoved.Invoke(this, m_Field, m_Index);
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/splines/SplineAsset.layout";
	}
}