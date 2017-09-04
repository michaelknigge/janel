package testpackage;

import java.util.*;

public class TestLaunch003
{

	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing setting janel.java.bundle=require_SDK and janel.min.java.version" );
			System.out.println("TestLaunch003 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch003 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}