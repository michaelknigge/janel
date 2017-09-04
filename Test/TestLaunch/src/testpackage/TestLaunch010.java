package testpackage;

import java.util.*;

public class TestLaunch010
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing janel.trap.console.ctrl=yes");						
			System.out.println( "Can not send Ctrl-C from within Ant to test completely" );
			Thread.sleep(3000); 
			System.out.println("TestLaunch010 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch010 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}

	public static void initiateExit(int exitCode)
	{
		System.out.println( "initiateExit was called" );
	}
}