package org.p2f1.controllers;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JOptionPane;

import org.p2f1.models.MainWindowModel;
import org.p2f1.views.MainWindowView;

import com.SerialPort.SerialPort;

public class MainWindowController implements ActionListener{

	private MainWindowView view = null;
	private MainWindowModel model = null;
	private SerialPort sp = null;
	
	public MainWindowController(MainWindowView view, MainWindowModel model){
		try{
			this.sp = new SerialPort();	
			this.view = view;
			this.model = model;
			this.view.associateController(this);
			this.view.setBaudRateList(sp.getAvailableBaudRates());
			this.view.setPortsList(sp.getPortList());
		}catch(Exception e){
			System.out.println(e.getMessage());
		}
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource() instanceof JButton){
			JButton btn = (JButton) e.getSource();
			switch(btn.getName()){
				case MainWindowView.BTN_RF:
					//TODO Afegir el codi per enviar per RF
                    sp
                    while(){

                    }
					JOptionPane.showMessageDialog(null, "M'has de programar!", "Missatge",JOptionPane.INFORMATION_MESSAGE);
					break;
				case MainWindowView.BTN_UART:
                    byte value = Byte.parseByte("1", 2);
					//TODO Afegir el codi per enviar per UART
                    byte[] byteArray = new byte[] {value, 79, 87, 46, 46, 46};
                    JOptionPane.showMessageDialog(null, "M'has de programar!", ,JOptionPane.INFORMATION_MESSAGE);
					if(model.checkInputText(view.getText())){
						JOptionPane.showMessageDialog(null, "M'has de programar!", "Missatge",JOptionPane.INFORMATION_MESSAGE);
					}else{
						JOptionPane.showMessageDialog(null, "Has de inserir text!", "Missatge",JOptionPane.ERROR_MESSAGE);
					}
					break;
				
			}
		}
	}
	
	public void showView(){
		view.setVisible(true);
	}
	
}
