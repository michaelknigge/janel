package testpackage;

import java.util.*;

public class TestLaunch012
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing setting multiple janel.classpath.jars.dir" );
			System.out.println( "and that janel.classpath.jars.dir is relative to Janel executable directory" );
			System.out.println("user.dir=" + props.getProperty("user.dir"));
			System.out.println("TestLaunch012 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch012 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}