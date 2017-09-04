package testpackage;

import java.util.*;

public class TestLaunch027
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "Testing setting janel.jvm.path to a nonexistent file." );			
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			
			System.out.println("TestLaunch027 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch027 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}