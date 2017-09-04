package testpackage;

import java.util.*;

public class TestLaunch045
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing setting janel.working.dir" );
			System.out.println("user.dir=" + props.getProperty("user.dir"));
			System.out.println("TestLaunch045 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch045 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}