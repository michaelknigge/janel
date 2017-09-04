package testpackage;

import java.util.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URI;
import testpackage.TestLaunch026;

public class TestLaunch025
{
	/*
	 * args[0] executable name, java for launching with java.exe
	 * args[1] String spawnPath
	 */
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println("Testing launch using Runtime.exec");
			System.out.println("java.home=" + props.getProperty("java.home"));
			System.out.println("user.dir=" + props.getProperty("user.dir"));
			System.out.println("caller.dir.test=" + props.getProperty("caller.dir.test"));
			System.out.println("self.home.test=" + props.getProperty("self.home.test"));
			System.out.println("found.exe.folder.test=" + props.getProperty("found.exe.folder.test"));
			
			if(args.length < 2)
			{
				throw new Exception("2 args required");
			}
			
			launchJavaSubProcess(args[0], args[1]);
			
			System.out.println("TestLaunch025 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch025 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
	
	private static void launchJavaSubProcess(String executableName, String spawnPath) throws Exception
	{		
        String[] command = getCommand( executableName );

        System.out.println( "Sub-process command:" );
        for( int i=0; i<command.length; i++ )
        {
            System.out.println( "[" + i + "] " + command[i] );
        }

        File working = new File( spawnPath );
        String canonicalPath = working.getCanonicalPath();
        
        System.out.println( "Initiating subprocess in: " + canonicalPath );
        Process process = Runtime.getRuntime().exec( command, null, working );
        StreamReader out = new StreamReader( process.getInputStream() );
        StreamReader err = new StreamReader( process.getErrorStream() );
        out.start();
        err.start();
        int result = process.waitFor();
        System.out.println( "subprocess result: " + result );	
	}
	
    private static String[] getCommand( String executableName ) throws Exception
    {
        if( executableName.equals("java") )
        {
            return  new String[]{ 
              "java",
              "-cp",
              new File( 
                new URI( 
                  TestLaunch026.class.getProtectionDomain().getCodeSource().getLocation().toString() 
                ) 
              ).toString(),
              TestLaunch026.class.getName() };
        }
        else
        {
            return new String[]{ executableName };
        }
    }

   /**
    * Internal class to handle reading of subprocess output and error streams.
    */
    private static class StreamReader extends Thread
    {
        private InputStream m_input;
        private boolean m_error = false;
        
       /**
        * Creation of a new reader.
        * @param input the subprocess input stream
        */
        public StreamReader( InputStream input )
        {
            m_input = input;
        }
  
       /**
        * Start thestream reader.
        */
        public void run()
        {
            try
            {
                InputStreamReader isr = new InputStreamReader( m_input );
                BufferedReader reader = new BufferedReader( isr );
                String line = null;
                while( ( line = reader.readLine() ) != null )
                {
                    System.out.println( line );
                }
            }
            catch( Exception e )
            {
                 e.printStackTrace();
            }
        }
    }
	
}