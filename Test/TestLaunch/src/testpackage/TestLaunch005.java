package testpackage;

import java.util.*;

public class TestLaunch005
{

	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));							
			System.out.println("testing janel.java.bundle=require_SDK and janel.bin.jvm.dir=require_server");
			System.out.println("Also setting janel.min.java.version=1.4");
			System.out.println("TestLaunch005 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch005 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}