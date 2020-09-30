class XMLApi
{
    private ref array< ref Param2< string, XMLCallback > > _threads = new array< ref Param2< string, XMLCallback > >;

    void Read( string file, notnull ref XMLCallback callback )
    {
        _threads.Insert( new Param2< string, XMLCallback >( file, callback ) );

		thread ThreadRead();
    }

    private void ThreadRead()
    {
        XMLDocument document = new XMLDocument();
        XMLReader reader = XMLReader.Open( _threads[0].param1 );
        XMLCallback callback = _threads[0].param2;

        _threads.Remove( 0 );

		#ifndef COMPONENT_SYSTEM
        Sleep( 10 );
		#endif

        callback.OnStart( document );

        bool success = false;
        if ( document && reader )
        {
            success = document.Read( reader );
        }

        if ( success )
        {
            callback.OnSuccess( document );
        } else 
        {
            callback.OnFailure( document );
        }
    }
};

static ref XMLApi g_reader;

static ref XMLApi GetXMLApi()
{
    if ( g_reader == NULL )
        g_reader = new XMLApi();

    return g_reader;
}