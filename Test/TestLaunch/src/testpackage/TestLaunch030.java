package testpackage;

import java.util.*;

public class TestLaunch030
{	
	public static void main(String[] args)
	{
		try
		{
			System.out.println( "Testing setting janel.main.argument to property value variables " );
			System.out.println( "and some without, plus adding real commandline arguments." );
			for(int i=0; i < args.length; i++)
			{
				System.out.println( "commandline arg [" + i + "]=" + args[i]);
			}
			
			System.out.println("TestLaunch030 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch030 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}