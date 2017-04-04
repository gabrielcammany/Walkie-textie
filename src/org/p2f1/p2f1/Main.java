package org.p2f1.p2f1;

import org.p2f1.controllers.MainWindowController;
import org.p2f1.models.MainWindowModel;
import org.p2f1.views.MainWindowView;


public class Main {

	public static void main(String [] args){
		
		MainWindowView view = new MainWindowView();
		MainWindowModel model = new MainWindowModel();
		MainWindowController controller = new MainWindowController(view, model);
		controller.showView();
		
	}
	
}
