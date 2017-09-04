package agent;

import java.lang.instrument.Instrumentation;

public class JanelAgent {

	public static void premain(String agentArgs, Instrumentation inst){
		System.out.println("JanelAgent premain method called.");
	}

}
