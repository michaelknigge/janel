package testpackage;

import java.util.*;
import javax.swing.*;

public class TestLaunch007
{

	ImageIcon iconTest;	

	public TestLaunch007()
	{
		loadImage();
		JFrame frame = new JFrame("Test7");
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
			System.out.println( "Testing GUI getting a resource from the jar");						
			TestLaunch007 tl7 = new TestLaunch007();
			Thread.sleep(1000);
			System.out.println("TestLaunch007 SUCCESSFUL");
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch007 FAILURE");
			System.exit(1);
		}
		System.exit(0);
	}
}