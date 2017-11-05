import controllers.MainWindowController;
import models.MainWindowModel;
import views.MainWindowView;


public class Main {

	public static void main(String [] args){
		
		MainWindowView view = new MainWindowView();
		MainWindowModel model = new MainWindowModel();
		MainWindowController controller = new MainWindowController(view, model);
		controller.showView();
		controller.start();
	}
	
}
