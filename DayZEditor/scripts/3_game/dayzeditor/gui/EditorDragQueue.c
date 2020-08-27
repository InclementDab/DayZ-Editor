

class EditorDragQueue: CallQueue
{
	private Class m_Context;
	private string m_Start;
	private string m_Update;
	private string m_End;
	
	
	void EditorDragQueue(Class context, string fn_start, string fn_update, string fn_end)
	{
		m_Context = context; m_Start = fn_start; m_Update = fn_update; m_End = fn_end;
		
		
		
	}
	
	override void Tick()
	{
		EditorLog.Trace("EditorDragQueue::Tick");
		if (m_processing) return;
		thread _TickThread();
	}
	
	private void _TickThread()
	{
		m_processing = true;
		
				
		m_processing = false;
	}
}

