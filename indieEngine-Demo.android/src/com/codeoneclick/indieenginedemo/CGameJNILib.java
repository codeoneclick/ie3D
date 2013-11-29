package com.codeoneclick.indieenginedemo;

public class CGameJNILib {

	 static
	 {
	        System.loadLibrary("IndieEngineDemo");
	 }
	 
	 public static native void fooo();
}
