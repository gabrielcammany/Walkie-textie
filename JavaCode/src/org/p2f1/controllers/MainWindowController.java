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
            sp.openPort("COM3",19200);
		}catch(Exception e){
			System.out.println(e.getMessage());
		}
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getSource() instanceof JButton){
			JButton btn = (JButton) e.getSource();
            try {
                switch(btn.getName()){
                    case MainWindowView.BTN_RF:
                        //TODO Afegir el codi per enviar per RF
                        try {
                            byte flag_enviar_rf_msg = (byte) (Integer.parseInt("82",16));
                            sp.writeByte(flag_enviar_rf_msg);
                            byte resposta = sp.readByte();
                            while(resposta == 0){ ;
                                resposta = sp.readByte();
                            }
                            System.out.print(" Reposta RF Enviat: "+resposta+"\n");
                        } catch (Exception e1) {
                            e1.printStackTrace();
                        }
                        break;
                    case MainWindowView.BTN_UART:
                        //TODO Afegir el codi per enviar per UART
                        byte flag_desar_msg = (byte) (Integer.parseInt("81",16));
                        byte flag_desat_msg = (byte) (Integer.parseInt("85",16));
                        byte end_byte = (byte) (Integer.parseInt("88",16));
                        //byte flag_confirmacio_msg = Byte.parseByte("83", 16);
                        //byte flag_desar_sense_confirmacio_msg = Byte.parseByte("84", 16);
                        try {
                            //sp.openPort(view.getPort(),view.getBaudRate());
                            if(model.checkInputText(view.getText())){
                                byte[] utf8Bytes = view.getText().getBytes("UTF-8");
                                sp.writeByte(flag_desar_msg);
                                byte resposta = sp.readByte();
                                while(resposta == 0){ ;
                                    resposta = sp.readByte();
                                }
                                System.out.print(" Reposta Enviat: "+resposta+"\n");
                                int i = 0;
                                System.out.print(" Index1: "+i+"\n");
                                System.out.print(" Mida1: "+utf8Bytes.length+"\n");
                                for (byte value:utf8Bytes) {
                                    sp.writeByte(value);
                                    resposta = sp.readByte();
                                    while(resposta == 0){ ;
                                        resposta = sp.readByte();
                                    }
                                    System.out.print(" Index2: "+i+"\n");
                                    System.out.print(" Reposta Enviat: "+resposta+"\n");
                                    i++;
                                }
                                sp.writeByte(end_byte);

                            }else{
                                JOptionPane.showMessageDialog(null, "No has escrit cap missatge!", "Error",JOptionPane.ERROR_MESSAGE);
                            }

                        } catch (Exception e1) {
                            JOptionPane.showMessageDialog(null, "Error en enviar les dades!\n "+ e1.getMessage(), "Error",JOptionPane.ERROR_MESSAGE);

                        }
                        break;

                }
            } catch (Exception e1) {
                e1.printStackTrace();
            }

		}
	}
	
	public void showView(){
		view.setVisible(true);
	}
	
}
