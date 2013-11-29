package com.codeoneclick.indieenginedemo;

import android.app.Activity;
import android.os.Bundle;

public class CGameViewController extends Activity
{
	 CGameView m_view;

	 @Override protected void onCreate(Bundle icicle) 
	 {
	     super.onCreate(icicle);
	     m_view = new CGameView(getApplication());
	     setContentView(m_view);
	 }

	 @Override protected void onPause() 
	 {
	     super.onPause();
	     m_view.onPause();
	 }

	 @Override protected void onResume() 
	 {
	     super.onResume();
	     m_view.onResume();
	 }
}
