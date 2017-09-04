package testpackage;

import java.util.*;

public class TestLaunch044
{
	static {
		System.loadLibrary("NativeTest");	
	}
	
	public native String nativeTestMethod(String parameter1);
	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing setting janel.library.path.dir.recursive" );
			
			TestLaunch015 tl = new TestLaunch015();
			String result =	tl.nativeTestMethod("TEST");
			System.out.println( "Result from calling nativeTestMethod()=" + result);
			
			System.out.println("TestLaunch044 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch044 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}