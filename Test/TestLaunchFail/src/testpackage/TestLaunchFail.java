package testpackage;

import java.io.*;
import java.util.*;

public class TestLaunchFail
{

	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println("TestLaunchFail SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunchFail FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}

	//public static void initiateExit(int exitCode)
	//{
	//	System.out.println( "Initiate exit was called" );
	//}
}