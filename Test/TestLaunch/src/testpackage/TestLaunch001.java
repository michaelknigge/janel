package testpackage;

import java.util.*;

public class TestLaunch001
{

	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println("Testing janel.jvm.path=${env.JAVA_HOME}\\jre\\bin\\server\\jvm.dll" );
			System.out.println("Testing janel.debug.file" );
			System.out.println("TestLaunch001 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch001 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}

	//public static void initiateExit(int exitCode)
	//{
	//	System.out.println( "Initiate exit was called" );
	//}
}