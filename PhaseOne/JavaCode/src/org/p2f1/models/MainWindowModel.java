package org.p2f1.models;

public class MainWindowModel {

	public boolean checkInputText(String sText){
		if(sText.length() > 0){
			return true;
		}	
		return false;
	}
	
}
