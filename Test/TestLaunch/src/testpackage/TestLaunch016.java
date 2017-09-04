package testpackage;

import java.util.*;

public class TestLaunch016
{	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));							
			System.out.println("testing setting janel.main.argument=arg0");
			System.out.println("and janel.main.argument=test arg1");
			
			for( int i=0; i < args.length; i++)
			{
				System.out.println("args[" + i + "]=" + args[i]);
			}
			
			System.out.println("TestLaunch016 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch016 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}