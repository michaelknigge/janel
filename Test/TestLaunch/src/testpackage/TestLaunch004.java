package testpackage;

import java.util.*;

public class TestLaunch004
{

	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing setting janel.bin.jvm.dir=client" );
			System.out.println("TestLaunch004 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch004 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}

}