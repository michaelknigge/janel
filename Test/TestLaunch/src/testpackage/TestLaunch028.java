package testpackage;

import java.util.*;

public class TestLaunch028
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "Testing setting janel.java.home.path to a nonexistent directory." );			
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			
			System.out.println("TestLaunch028 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch028 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}