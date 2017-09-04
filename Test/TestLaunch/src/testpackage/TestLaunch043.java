package testpackage;

import java.util.*;

public class TestLaunch043
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing janel.classpath.jars.dir.recursive" );
			System.out.println("TestLaunch043 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch043 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}