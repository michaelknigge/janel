package testpackage;

import java.util.*;

public class TestLaunch014
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing setting janel.bin.jvm.dir=prefer_server" );
			System.out.println("TestLaunch014 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch014 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}