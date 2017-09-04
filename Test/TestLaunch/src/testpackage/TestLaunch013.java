package testpackage;

import java.util.*;

public class TestLaunch013
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing adding arguments to the command-line" );
			for(int i=0; i < args.length; i++) 
			{
				System.out.println( "args[" + i + "]=" + args[i] );
			}
			System.out.println("TestLaunch013 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch013 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}