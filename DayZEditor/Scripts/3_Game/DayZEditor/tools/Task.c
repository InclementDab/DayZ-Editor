// Crashes instantly. 
// Passing using / storing type 'func' only works when you are calling from the same context
// i think...
class Task<Class TResult>
{
	private bool m_ExecutionFinished = false;
	
	private TResult m_Result;
	private ref ScriptCallQueue m_CallQueue;
	
	void Task(func function)
	{
		thread StartSynchronously(function);
	}
	
	void Start(func function)
	{
		thread StartSynchronously(function);
	}
	
	private void StartSynchronously(func function)
	{
		m_CallQueue = new ScriptCallQueue();
		m_CallQueue.Call(function);
	}
	
	TResult GetResult()
	{
		while (!m_ExecutionFinished) {
			Sleep(10);
		}
		
		Print("returning result");
		return m_Result;
	}
}

/*


void SaveAs()
{
	EditorLog.Trace("Editor::SaveAs");
	thread _SaveAs();
}

private void _SaveAs()
{
	DeleteTest delete_test = new DeleteTest();		
	string result = delete_test.Start();

	// always prints the result. since the object isnt deleted until after Start() finishes
	Print(result); 
	
	// Prints NULL
	Print(delete_test);
}

class DeleteTest
{
	void ~DeleteTest()
	{
		Print("~DeleteTest");
	}
	
	private string m_SetString;
	string Start()
	{	
		Print("Starting...");
		thread DelaySetResult();
			
		while (m_SetString == string.Empty) {
			Sleep(1);
		}
		
		Print("Preparing to return");
		
		// turns out.. this isnt null until AFTER we return from Start()
		Print(this); 
		Sleep(2500);
		return m_SetString;
	}
	
	void SetResult(string set_string)
	{
		m_SetString = set_string;
		delete this;
		
		// calling 'delete this' cancels the execution on the existing function
		// however it doesnt cancel all executions in the object
		Print("This wont print"); 
	}
	
	void DelaySetResult()
	{
		Sleep(3000);
		SetResult("Result!");
	}
}
*/