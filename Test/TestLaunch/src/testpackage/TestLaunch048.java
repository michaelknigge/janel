package testpackage;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class TestLaunch048
{
	private static final String LOG_FILE = "C:\\Test\\JanelTestLog048.txt";
	private static final String STOPPED_LOG_FILE = "C:\\Test\\JanelTestLogStopped048.txt";
	
	public static void main(String[] args)
	{		
		BufferedWriter bufferedLogFileWriter = null;
		try
		{	
			bufferedLogFileWriter = new BufferedWriter(new FileWriter(LOG_FILE));
			bufferedLogFileWriter.write("Testing Janel's service by installing the service, starting it, stopping it, then uninstalling it.\r\n");
			
			for(int i=0; i < args.length; i++)
			{
				bufferedLogFileWriter.write("Argument " + (i+1) + "=" + args[i] + "\r\n");	
			}
			
			bufferedLogFileWriter.write("TestLaunch048 SUCCESSFUL\r\n");
			bufferedLogFileWriter.flush();
			bufferedLogFileWriter.close();
			
			// sleep for 2 minutes to give the test enough time to call stop on it.
			long startTime = System.currentTimeMillis();
			long currentTime;
			do {
				Thread.sleep(5000);
				currentTime = System.currentTimeMillis();
			} while (currentTime - startTime < 120000);
		}
		catch(Exception ex)
		{
			ex.printStackTrace();
			System.out.println("TestLaunch048 FAILURE");
			System.exit(1);
		}
		finally 
		{
			if(bufferedLogFileWriter != null) {
				try {
					bufferedLogFileWriter.close();
				}
				catch(IOException ioe) { }
			}
		}
		System.exit(0);
	}
	
	public static void initiateExit(int exitCode)
	{
		BufferedWriter bufferedLogFileWriter = null;
		try
		{	
			System.out.println( "initiateExit was called." );
			bufferedLogFileWriter = new BufferedWriter(new FileWriter(STOPPED_LOG_FILE));
			bufferedLogFileWriter.write("initiateExit was called.\r\n");
			bufferedLogFileWriter.flush();
			bufferedLogFileWriter.close();
		} 
		catch (IOException e) 
		{
			e.printStackTrace();
		}
		finally 
		{
			if(bufferedLogFileWriter != null) {
				try {
					bufferedLogFileWriter.close();
				}
				catch(IOException ioe) { }
			}
			System.exit(0);
		}
	}

}