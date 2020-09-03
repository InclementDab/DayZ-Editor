
typedef array<Class> Stack;
class Stack<Class T>: array<Class>
{
	private T _default;
	private T _array[];
	private int _size;
	
	void Stack() {
		_size = 0;
	}
	
	
	
	T Peek() {
		if (!_size) return _default;
		
		return _array[_size - 1];
	}
	
	T Pop() {
		if (!_size) return _default;
		
		T item;
		_size--;
		item = _array[_size];
		//delete _array[_size];
		return item;
	}
	
	void Add(T _value) {
		_array[_size++] = _value;
	}
	
	int Count() {
		return _size;
	}
	
	void Clear() {
		
	}
	
	T Get(int n) {
		return _array[n];
	}
	
	
	MapIterator Begin() {
		return 0;
	}
	
	MapIterator End() {
		return _size;
	}
	
	MapIterator Next(MapIterator it) {
		return it + 1;
	}
	
	T GetIteratorElement(MapIterator iterator) {
		return _array[MapIterator];
	}
}


