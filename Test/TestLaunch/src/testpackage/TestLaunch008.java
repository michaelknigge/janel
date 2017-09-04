package testpackage;

import java.util.*;
import javax.swing.*;

public class TestLaunch008
{

	ImageIcon iconTest;	

	public TestLaunch008()
	{
		loadImage();
		JFrame frame = new JFrame("Test8");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		JLabel gifLabel = new JLabel(iconTest);
		frame.getContentPane().add(gifLabel);
		frame.pack();
		frame.setVisible(true);		
	}

	void loadImage()
	{
		ClassLoader classLoader = this.getClass().getClassLoader();
		iconTest = new ImageIcon( classLoader.getResource("images/Help16.gif") );
    }
	
	public static void main(String[] args)
	{
		try
		{
			Properties props = System.getProperties();
			System.out.println( "java.home=" + props.getProperty("java.home"));				
			System.out.println( "Testing GUI exe setup similar to JavadocExpress and JavadocJsharp");
			System.out.println( "setting -Djava.class.path=dist\\TestLaunch.jar;${FOUND_JAVA_HOME}\\lib\\tools.jar");
			System.out.println( "setting -DJAVA_HOME=${FOUND_JAVA_HOME}");
			System.out.println( "setting janel.java.bundle=SDK");
			System.out.println( "setting janel.bin.jvm.dir=client" );
			System.out.println( "setting janel.min.java.version=1.4" );
			TestLaunch008 tl = new TestLaunch008();
			Thread.sleep(1000);
			System.out.println("TestLaunch008 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch008 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}