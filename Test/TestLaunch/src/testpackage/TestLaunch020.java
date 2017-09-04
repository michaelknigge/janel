package testpackage;

import java.util.*;

public class TestLaunch020
{
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));
			System.out.println("testing setting -Duser.dir=dist and -Djava.class.path=TestLaunch.jar");
			System.out.println("System property user.dir=" + System.getProperty("user.dir"));
			System.out.println("System property java.class.path=" + System.getProperty("java.class.path"));

			System.out.println("TestLaunch020 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch020 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}