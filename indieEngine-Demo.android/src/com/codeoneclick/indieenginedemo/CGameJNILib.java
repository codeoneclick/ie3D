package com.codeoneclick.indieenginedemo;

public class CGameJNILib {

	 static
	 {
	        System.loadLibrary("IndieEngine-Demo");
	 }
	 
	 public static native void start();
	 public static native void run();
}
