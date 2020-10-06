Widget GetChildAtIndex(Widget widget, int index)
{
	Widget result = widget.GetChildren();
	while (index > 0)
	{
		result = result.GetSibling();
		index--;
	}

	return result;
}

bool IsWidgetChild(Widget widget, Widget child)
{
	Widget result = widget.GetChildren();
	while (result != null)
	{
		if (result == child)
			return true;
		result = result.GetSibling();
	}

	return false;
}

class WidgetController
{
	bool CanTwoWayBind()
	{
		return false;
	}

	// Base Controller Stuff
	void Set(TypeConverter typeConverter);
	void Get(out TypeConverter typeConverter);

	// Collection Stuff
	void SetSelection(TypeConverter typeConverter)
	{
		NotImplementedError("SetSelection");
	}

	void GetSelection(out TypeConverter typeConverter)
	{
		NotImplementedError("GetSelection");
	}

	void Insert(TypeConverter typeConverter)
	{
		NotImplementedError("Insert");
	}

	void InsertAt(int index, TypeConverter typeConverter)
	{
		NotImplementedError("InsertAt");
	}

	void Remove(int index, TypeConverter typeConverter)
	{
		NotImplementedError("Remove");
	}

	void Replace(int index, TypeConverter typeConverter)
	{
		NotImplementedError("Replace");
	}

	void Move(int index, TypeConverter typeConverter)
	{
		NotImplementedError("Move");
	}

	void Swap(int indexA, int indexB)
	{
		NotImplementedError("Swap");
	}

	void Clear()
	{
		NotImplementedError("Clear");
	}

	int Find(TypeConverter typeConverter)
	{
		NotImplementedError("Find");
		return -1;
	}

	int Count()
	{
		NotImplementedError("Count");
		return -1;
	}

	private	void NotImplementedError(string function)
	{
		Error(string.Format("%1 does not support function %2", Type(), function));
	}
};

class WidgetControllerTemplate<Class T> : WidgetController
{
	protected T m_Widget;

	void SetWidget(T w)
	{
		Class.CastTo(m_Widget, w);
	}
};

class WidgetBaseController : WidgetControllerTemplate<Widget>
{
	override void Set(TypeConverter typeConverter)
	{
		if (!m_Widget)
			return;

		Widget parent = m_Widget.GetParent();
		int sort = m_Widget.GetSort();
		if (typeConverter.GetWidget())
		{
			m_Widget = typeConverter.GetWidget();

			if (parent)
			{
				parent.AddChild(m_Widget, false);
			}

			m_Widget.SetSort(sort);
		}
	}

	override void Get(out TypeConverter typeConverter)
	{
		typeConverter.SetWidget(m_Widget);
	}
};

class ButtonWidgetController : WidgetControllerTemplate<ButtonWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter typeConverter)
	{
		m_Widget.SetState(typeConverter.GetBool());
	}

	override void Get(out TypeConverter typeConverter)
	{
		typeConverter.SetBool(m_Widget.GetState());
	}

	override void SetSelection(TypeConverter typeConverter)
	{
		m_Widget.SetText(typeConverter.GetString());
	}

	override void GetSelection(out TypeConverter typeConverter)
	{
		// Since SetSelection is being used. This needs to override to avoid errors
	}
};

class EditBoxWidgetController : WidgetControllerTemplate<EditBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter typeConverter)
	{
		m_Widget.SetText(typeConverter.GetString());
	}

	override void Get(out TypeConverter typeConverter)
	{
		typeConverter.SetString(m_Widget.GetText());
	}
};

class CheckBoxWidgetController : WidgetControllerTemplate<CheckBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter typeConverter)
	{
		m_Widget.SetChecked(typeConverter.GetBool());
	}

	override void Get(out TypeConverter typeConverter)
	{
		typeConverter.SetBool(m_Widget.IsChecked());
	}

	override void SetSelection(TypeConverter typeConverter)
	{
		m_Widget.SetText(typeConverter.GetString());
	}

	override void GetSelection(out TypeConverter typeConverter)
	{
		// Since SetSelection is being used. This needs to override to avoid errors
	}
};

class SliderWidgetController : WidgetControllerTemplate<SliderWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter typeConverter)
	{
		m_Widget.SetCurrent(typeConverter.GetFloat());
	}

	override void Get(out TypeConverter typeConverter)
	{
		typeConverter.SetFloat(m_Widget.GetCurrent());
	}
};

class ProgressBarController : WidgetControllerTemplate<ProgressBarWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter typeConverter)
	{
		m_Widget.SetCurrent(typeConverter.GetFloat());
	}

	override void Get(out TypeConverter typeConverter)
	{
		typeConverter.SetFloat(m_Widget.GetCurrent());
	}
};

class TextWidgetController : WidgetControllerTemplate<TextWidget>
{
	override void Set(TypeConverter typeConverter)
	{
		m_Widget.SetText(typeConverter.GetString());
	}
};

class ImageWidgetController : WidgetControllerTemplate<ImageWidget>
{
	override void Set(TypeConverter typeConverter)
	{
		if (typeConverter.GetString())
		{
			m_Widget.LoadImageFile(0, typeConverter.GetString());
			m_Widget.SetImage(0);
		}
	}
};

class MultilineEditBoxWidgetController : WidgetControllerTemplate<MultilineEditBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter typeConverter)
	{
		m_Widget.SetText(typeConverter.GetString());
	}

	override void Get(out TypeConverter typeConverter)
	{
		string out_text;
		m_Widget.GetText(out_text);
		typeConverter.SetString(out_text);
	}

	override void InsertAt(int index, TypeConverter typeConverter)
	{
		m_Widget.SetLine(index, typeConverter.GetString());
	}

	override void Replace(int index, TypeConverter typeConverter)
	{
		InsertAt(index, typeConverter);
	}

	override void Remove(int index, TypeConverter typeConverter)
	{
		m_Widget.SetLine(index, string.Empty);
	}

	override void Clear()
	{
		for (int i = 0; i < m_Widget.GetLinesCount(); i++)
			m_Widget.SetLine(i, string.Empty);
	}
};

class SpacerBaseWidgetController : WidgetControllerTemplate<SpacerBaseWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter typeConverter)
	{
		if (typeConverter.GetWidget())
		{
			SetFocus(typeConverter.GetWidget());
		}
	}

	override void GetSelection(out TypeConverter typeConverter)
	{
		if (IsWidgetChild(m_Widget, GetFocus()))
		{
			typeConverter.SetWidget(GetFocus());
		}
	}

	override void Insert(TypeConverter typeConverter)
	{
		if (typeConverter.GetWidget())
		{
			m_Widget.AddChild(typeConverter.GetWidget());
		}
	}

	override void InsertAt(int index, TypeConverter typeConverter)
	{
		if (index == Count())
		{
			Insert(typeConverter);
		} else if (typeConverter.GetWidget())
		{
			Widget widgetA = GetChildAtIndex(m_Widget, index);
			m_Widget.AddChildAfter(typeConverter.GetWidget(), widgetA);
		}
	}

	override void Replace(int index, TypeConverter typeConverter)
	{
		if (typeConverter.GetWidget())
		{
			Widget widgetA = GetChildAtIndex(m_Widget, index);
			m_Widget.AddChildAfter(typeConverter.GetWidget(), widgetA);
			m_Widget.RemoveChild(widgetA);
		}
	}

	override void Remove(int index, TypeConverter typeConverter)
	{
		if (typeConverter.GetWidget())
		{
			m_Widget.RemoveChild(typeConverter.GetWidget());
		}
	}

	override void Move(int index, TypeConverter typeConverter)
	{
		Widget widgetA = GetChildAtIndex(m_Widget, index - 1);
		if (typeConverter.GetWidget() && widgetA)
		{
			m_Widget.RemoveChild(typeConverter.GetWidget());
			m_Widget.AddChildAfter(typeConverter.GetWidget(), widgetA);
		}
	}

	override void Swap(int indexA, int indexB)
	{
		if (indexA == indexB || indexA < 0 || indexB < 0)
			return;

		if (indexA > indexB)
		{
			int temp = indexA;
			indexA = indexB;
			indexB = temp;
		}

		Widget widgetA = GetChildAtIndex(m_Widget, indexA);
		Widget widgetB = GetChildAtIndex(m_Widget, indexB);
		Widget widgetC = GetChildAtIndex(m_Widget, indexB - 1);

		m_Widget.RemoveChild(widgetA);
		m_Widget.AddChildAfter(widgetA, widgetB);
		m_Widget.RemoveChild(widgetB);
		m_Widget.AddChildAfter(widgetB, widgetC);
	}

	override void Clear()
	{
		Widget widgetA = m_Widget.GetChildren();
		while (widgetA != null)
		{
			m_Widget.RemoveChild(widgetA);
			widgetA = widgetA.GetSibling();
		}
	}

	override int Find(TypeConverter typeConverter)
	{
		Widget widgetA = m_Widget.GetChildren();
		int result;
		while (widgetA != null)
		{

			if (widgetA == typeConverter.GetWidget())
			{
				return result;
			}

			widgetA = widgetA.GetSibling();
			result++;
		}

		return -1;
	}

	override int Count()
	{
		Widget widgetA = m_Widget.GetChildren();
		int result;
		while (widgetA != null)
		{
			widgetA = widgetA.GetSibling();
			result++;
		}

		return result;
	}
};

class XComboBoxWidgetController : WidgetControllerTemplate<XComboBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter typeConverter)
	{
		m_Widget.SetCurrentItem(typeConverter.GetInt());
	}

	override void GetSelection(out TypeConverter typeConverter)
	{
		typeConverter.SetInt(m_Widget.GetCurrentItem());
	}

	override void Insert(TypeConverter typeConverter)
	{
		m_Widget.AddItem(typeConverter.GetString());
	}

	override void InsertAt(int index, TypeConverter typeConverter)
	{
		m_Widget.SetItem(index, typeConverter.GetString());
	}

	override void Replace(int index, TypeConverter typeConverter)
	{
		InsertAt(index, typeConverter);
	}

	override void Remove(int index, TypeConverter typeConverter)
	{
		m_Widget.RemoveItem(index);
	}

	override void Clear()
	{
		m_Widget.ClearAll();
	}

	override int Count()
	{
		return m_Widget.GetNumItems();
	}
};

class TextListboxController : WidgetControllerTemplate<TextListboxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter typeConverter)
	{
		for (int i = 0; i < m_Widget.GetNumItems(); i++)
		{
			string row_text;
			m_Widget.GetItemText(i, 0, row_text);
			if (row_text == typeConverter.GetString())
			{
				m_Widget.SelectRow(i);
				return;
			}
		}
	}

	override void GetSelection(out TypeConverter typeConverter)
	{
		string selection;
		m_Widget.GetItemText(m_Widget.GetSelectedRow(), 0, selection);
		typeConverter.SetString(selection);
	}

	override void Insert(TypeConverter typeConverter)
	{
		m_Widget.AddItem(typeConverter.GetString(), typeConverter, 0);
	}

	override void InsertAt(int index, TypeConverter typeConverter)
	{
		m_Widget.SetItem(index, typeConverter.GetString(), typeConverter, 0);
	}

	override void Remove(int index, TypeConverter typeConverter)
	{
		m_Widget.SetItem(index, string.Empty, typeConverter, 0);
	}

	override void Swap(int indexA, int indexB)
	{
		string textA, textB;
		Class dataA, dataB;

		if (indexA < 0 || indexA > m_Widget.GetNumItems() || indexB < 0 || indexB > m_Widget.GetNumItems())
		{
			return;
		}

		m_Widget.GetItemText(indexA, 0, textA);
		m_Widget.GetItemData(indexA, 0, dataA);

		m_Widget.GetItemText(indexB, 0, textB);
		m_Widget.GetItemData(indexB, 0, dataB);

		m_Widget.SetItem(indexB, textA, dataA, 0);
		m_Widget.SetItem(indexA, textB, dataB, 0);
	}

	override void Clear()
	{
		m_Widget.ClearItems();
	}

	override int Count()
	{
		return m_Widget.GetNumItems();
	}
};

class ItemPreviewWidgetController : WidgetControllerTemplate<ItemPreviewWidget>
{
	override void Set(TypeConverter typeConverter)
	{
		EntityAI entity;
		if (Class.CastTo(entity, typeConverter.GetObject()))
		{
			m_Widget.SetItem(entity);
			m_Widget.SetModelPosition(vector.Zero);
		}
	}

	override void Get(out TypeConverter typeConverter)
	{
		typeConverter.Set(m_Widget.GetItem());
	}
};