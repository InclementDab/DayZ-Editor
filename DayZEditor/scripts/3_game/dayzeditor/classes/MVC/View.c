

class EditorView: EditorViewBase
{
	// Required
	// Name of Widget that has Controller ScriptClass	
	protected Widget m_LayoutRoot;
	protected Widget m_ViewModelWidget;
		
	protected ref Controller m_ViewModel;

	void EditorView() { EditorLog.Trace("EditorView"); }
	void ~EditorView() { EditorLog.Trace("~EditorView"); }


	void OnWidgetScriptInit(Widget w)
	{
		
		EditorLog.Trace("EditorView::OnWidgetScriptInit");
		m_LayoutRoot = w;
		
#ifdef COMPONENT_SYSTEM
		
		m_ViewModel = ControllerHashMap.Get(Controller.GetFromWidget(ControllerBase.GetWidgetSource()));
		
		if (m_EditorViewProjectData == null) {
			m_EditorViewProjectData = new EditorViewProjectData();
		}
		
		EditorViewProjectData.LoadData(m_EditorViewProjectData, "P:\\DayZEditor\\layoutdata.bin");
		m_EditorViewData = m_EditorViewProjectData.GetData(w);
		if (m_EditorViewData != null) {
			Print("Data Found!");
		} else {
			Print("Data NOT found");
			m_EditorViewData = new EditorViewData(m_LayoutRoot.GetName());
			m_EditorViewProjectData.InsertViewData(m_LayoutRoot, m_EditorViewData);
			EditorViewProjectData.SaveData(m_EditorViewProjectData, "P:\\DayZEditor\\layoutdata.bin");
		}
		
		m_EditorViewData.SetDialogCallback(this, "EditDialogCallback");

#endif	
	
			
		// Set the control widget to relevant Widget
		if (ProxyName != string.Empty) {
			m_LayoutRoot = m_LayoutRoot.FindAnyWidget(ProxyName);
		}
		
		// If var_name is blank, just use the name of root
		if (DataBindingName == string.Empty)
			DataBindingName = m_LayoutRoot.GetName();

		if (!m_ViewModel) {
			//Workbench.Dialog("Error", string.Format("%1: Could not find ViewModel", m_LayoutRoot.GetName()));
			return;
		}
		
		m_ViewModel.InsertView(this);
		m_LayoutRoot.SetHandler(this);
		
		// This reloads the view incase data is added before view is created
		OnPropertyChanged();
	}


	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		EditorLog.Trace("EditorView::OnChange");
		UpdateModel();
		return super.OnChange(w, x, y, finished);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorView::OnClick");
		UpdateModel();
		return super.OnClick(w, x, y, button);
	}
	
	void EditDialogCallback(int result)
	{
		EditorLog.Trace("EditorView::EditDialogCallback");
		Print(result);
		if (result > 0) {
			m_EditorViewProjectData.InsertViewData(m_LayoutRoot, m_EditorViewData);
			EditorViewProjectData.SaveData(m_EditorViewProjectData, "P:\\DayZEditor\\layoutdata.bin");
		}
	}

	
	// UI -> Model
	void UpdateModel()
	{
		EditorLog.Trace("EditorView::UpdateModel");
	
		string text;
		switch (m_LayoutRoot.Type()) {
			
			case TextWidget: {
				EditorLog.Warning("ViewModel cannot be updated for TextWidgets");
				break;
			}
			
			case MultilineEditBoxWidget: {
				MultilineEditBoxWidgetData _MultilineEditBoxWidgetData;
				MultilineEditBoxWidget _MultilineEditBoxWidget = MultilineEditBoxWidget.Cast(m_LayoutRoot);
				for (int i = 0; i < _MultilineEditBoxWidget.GetLinesCount(); i++) {
					string line;
					_MultilineEditBoxWidget.GetLine(i, line);
					EnScript.SetClassVar(m_ViewModel, DataBindingName, i, line);
				}

				break;
			}
			
			case EditBoxWidget: {

				switch (m_ViewModel.GetVariableType(DataBindingName)) {
					
					case EditBoxWidgetData:
					case string: {
						EditBoxWidgetData _EditBoxWidgetData = EditBoxWidget.Cast(m_LayoutRoot).GetText();
						EnScript.SetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _EditBoxWidgetData);
						break;
					}
					
					case EditBoxWidgetDataF: {
						EditBoxWidgetDataF _EditBoxWidgetDataF = EditBoxWidget.Cast(m_LayoutRoot).GetText();
						EnScript.SetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _EditBoxWidgetDataF.GetValidString());
						break;
					}
					
					
				}
				
				

				
				break;
			}
			
			case ButtonWidget: {
				ButtonWidgetData _ButtonWidgetData;
				_ButtonWidgetData = ButtonWidget.Cast(m_LayoutRoot).GetState();
				EnScript.SetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _ButtonWidgetData);
				break;
			}
			
			case CheckBoxWidget: {
				CheckBoxWidgetData _CheckBoxWidgetData;
				_CheckBoxWidgetData = CheckBoxWidget.Cast(m_LayoutRoot).IsChecked();
				EnScript.SetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _CheckBoxWidgetData);
				break;
			}
			
			case SliderWidget: {
				SliderWidgetData _SliderWidgetData;
				_SliderWidgetData = SliderWidget.Cast(m_LayoutRoot).GetCurrent();
				EnScript.SetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _SliderWidgetData);
				break;
			}
			
			case XComboBoxWidget: {

				break;
			}
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_LayoutRoot.Type().ToString()));
			}
		}
		
		NotifyPropertyChanged(DataBindingName);
	}
	
	
	// Model -> UI
	override void OnPropertyChanged() 
	{
		//EditorLog.Trace("EditorView::OnPropertyChanged: " + DataBindingName);
		
		switch (m_LayoutRoot.Type()) {
			
			case TextWidget: {
				
				switch (m_ViewModel.GetVariableType(DataBindingName)) {
					
					case TextWidgetData:
					case string: {
						string _TextWidgetDataS;
						EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _TextWidgetDataS);
						TextWidget.Cast(m_LayoutRoot).SetText(_TextWidgetDataS);
						break;
					}
					
					case int:
					case float: {
						float _TextWidgetDataF;
						EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _TextWidgetDataF);
						TextWidget.Cast(m_LayoutRoot).SetText(_TextWidgetDataF.ToString());
						break;
					}
					
					case bool: {
						bool _TextWidgetDataB;
						EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _TextWidgetDataB);
						TextWidget.Cast(m_LayoutRoot).SetText(_TextWidgetDataB.ToString());
						break;
					}
				}
						
				break;
			}
			
			case EditBoxWidget: {
				
				switch (m_ViewModel.GetVariableType(DataBindingName)) {
					
					case EditBoxWidgetData:
					case EditBoxWidgetDataF:
					case string: {
						string _EditBoxWidgetDataS;
						EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _EditBoxWidgetDataS);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataS);
						break;
					}
					
					case int:
					case float: {
						float _EditBoxWidgetDataF;
						EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _EditBoxWidgetDataF);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataF.ToString());
						break;
					}
					
					case bool: {
						bool _EditBoxWidgetDataB;
						EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _EditBoxWidgetDataB);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataB.ToString());
						break;
					}
					
					case vector: {
						vector _EditBoxWidgetDataV;
						EnScript.GetClassVar(m_ViewModel, DataBindingName, 0, _EditBoxWidgetDataV);
						EditBoxWidget.Cast(m_LayoutRoot).SetText(_EditBoxWidgetDataV[DataBindingIndex].ToString());
						break;
					}
				}
						
				break;
				
				
			}
			
		
			
			
			case ButtonWidget: {
				
				ButtonWidgetData _ButtonWidgetData;
				EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _ButtonWidgetData);
				ButtonWidget.Cast(m_LayoutRoot).SetState(_ButtonWidgetData);
				break;
			}
			
			case CheckBoxWidget: {
				
				CheckBoxWidgetData _CheckBoxWidgetData;
				EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _CheckBoxWidgetData);
				CheckBoxWidget.Cast(m_LayoutRoot).SetChecked(_CheckBoxWidgetData);
				break;
			}
			
			case SliderWidget: {
				SliderWidgetData _SliderWidgetData;
				EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _SliderWidgetData);
				SliderWidget.Cast(m_LayoutRoot).SetCurrent(_SliderWidgetData);
				break;
			}
			
			case WrapSpacerWidget: {
				WrapSpacerWidgetData _WrapSpacerWidgetData;
				EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _WrapSpacerWidgetData);
				_WrapSpacerWidgetData.ReloadData(WrapSpacerWidget.Cast(m_LayoutRoot));
				break;
			}
			
			case TextListboxWidget: {
				TextListboxWidgetData _TextListboxWidgetData;
				EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _TextListboxWidgetData);
				_TextListboxWidgetData.ReloadData(TextListboxWidget.Cast(m_LayoutRoot));
				break;
			}
			
			case XComboBoxWidget: {
				XComboBoxWidgetData _XComboBoxWidgetData;
				EnScript.GetClassVar(m_ViewModel, DataBindingName, DataBindingIndex, _XComboBoxWidgetData);
				_XComboBoxWidgetData.ReloadData(XComboBoxWidget.Cast(m_LayoutRoot));
				break;
			}
			
			default: {
				Error(string.Format("Unsupported Widget Type %1", m_LayoutRoot.Type().ToString()));
			}
			
		}
	}



	
	override void OnCollectionChanged(CollectionChangedEventArgs args)
	{		
		EditorLog.Trace(string.Format("EditorView::OnCollectionChanged: %1 Action: %2", DataBindingName, args.param2));
				
		Observable collection = args.param1;
		NotifyCollectionChangedAction action = args.param2;
		ref Param changed_params = args.param3;
		

		
		IWidgetData widget_data_interface(collection);
		
		switch (action) {
			case NotifyCollectionChangedAction.Add: {
				widget_data_interface.Add(m_LayoutRoot, changed_params);
				break;
			}
			
			case NotifyCollectionChangedAction.Remove: {
				widget_data_interface.Remove(m_LayoutRoot, changed_params);
				break;
			}
			
			case NotifyCollectionChangedAction.Clear: {
				widget_data_interface.Clear(m_LayoutRoot);
				break;
			}
			
			case NotifyCollectionChangedAction.Set: {
				widget_data_interface.Set(m_LayoutRoot, changed_params);
				break;
			}
			
			default: {
				Error("OnCollectionChanged: Not Implemented Exception!");
			}
		}	
	}
	
	
	
	
	void DebugPrint()
	{
		EditorLog.Debug("EditorView::DebugPrint: " + m_LayoutRoot.GetName());
		EditorLog.Debug("ControllerWidget:" + ControllerWidget);
		EditorLog.Debug("DataBindingName:" + DataBindingName);
		EditorLog.Debug("DataBindingIndex:" + DataBindingIndex);
		EditorLog.Debug("ProxyName:" + ProxyName);
	}
};