package testpackage;

import java.util.*;

public class TestLaunch006
{

	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println("Testing no extra custom properties set.");
			System.out.println("TestLaunch006 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch006 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}