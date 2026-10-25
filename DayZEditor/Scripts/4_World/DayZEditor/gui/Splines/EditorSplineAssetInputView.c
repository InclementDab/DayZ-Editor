// Type a class name or model path and press enter, or take what is selected. single fields hold one asset (walls, posts)
class EditorSplineAssetInputView: ScriptView
{
	protected string m_Field;
	protected ref ScriptCaller m_OnEntered;
	protected ref ScriptCaller m_OnSelected;
	protected ref ScriptCaller m_OnCleared;

	TextWidget Caption;
	EditBoxWidget AssetBox;
	ButtonWidget SelectedButton, ClearButton;
	Widget SelectedButtonWrapper, ClearButtonWrapper;

	void EditorSplineAssetInputView(string caption, string field, bool single, ScriptCaller on_entered, ScriptCaller on_selected, ScriptCaller on_cleared)
	{
		m_Field = field;
		m_OnEntered = on_entered;
		m_OnSelected = on_selected;
		m_OnCleared = on_cleared;

		Caption.SetText(caption);
		if (single) {
			ClearButtonWrapper.Show(false);
			SelectedButtonWrapper.SetSize(0.32, 20);
			SelectedButton.SetText("Use Selected");
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return super.OnClick(w, x, y, button);
		}

		switch (w) {
			case SelectedButton: {
				if (m_OnSelected) {
					m_OnSelected.Invoke(this, m_Field);
				}

				return true;
			}

			case ClearButton: {
				if (m_OnCleared) {
					m_OnCleared.Invoke(this, m_Field);
				}

				return true;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w == AssetBox && finished) {
			string asset = AssetBox.GetText().Trim();
			AssetBox.SetText(string.Empty);
			if (asset != string.Empty && m_OnEntered) {
				m_OnEntered.Invoke(this, m_Field, asset);
			}

			return true;
		}

		return super.OnChange(w, x, y, finished);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/splines/SplineAssetInput.layout";
	}
}