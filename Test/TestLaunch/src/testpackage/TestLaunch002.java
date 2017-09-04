package testpackage;

import java.util.*;

public class TestLaunch002
{

	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));	
			System.out.println("Testing setting janel.java.home.path");
			System.out.println("TestLaunch002 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch002 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}