package org.p2f1.controllers;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;

import org.omg.PortableServer.POA;
import org.p2f1.models.MainWindowModel;
import org.p2f1.p2f1.PortThread;
import org.p2f1.views.MainWindowView;

import com.SerialPort.SerialPort;

public class MainWindowController implements ActionListener{
	private MainWindowView view = null;
	private MainWindowModel model = null;
	private SerialPort sp = null;
	private PortThread portThread = null;
    private final byte end_byte = (byte) (Integer.parseInt("88",16));
    private final byte start_byte_rf = (byte) (Integer.parseInt("40",16));
    private final byte end_byte_rf = (byte) (Integer.parseInt("40",16));
    private final byte flag_desar_msg = (byte) (Integer.parseInt("81",16));
    private final byte flag_enviar_rf_msg = (byte) (Integer.parseInt("82",16));
    private final byte flag = (byte) (Integer.parseInt("84",16));
    private final byte primer_byte_part_ID = (byte) (Integer.parseInt("37",16));
    private final byte segon_byte_part_ID = (byte) (Integer.parseInt("37",16));
    private final byte tercer_byte_part_ID = (byte) (Integer.parseInt("39",16));
    private final byte espai_byte_part_ID = (byte) (Integer.parseInt("20",16));

	public MainWindowController(MainWindowView view, MainWindowModel model){
		try{
			this.sp = new SerialPort();	
			this.view = view;
			this.model = model;
			this.view.associateController(this);
			this.view.setBaudRateList(sp.getAvailableBaudRates());
			this.view.setPortsList(sp.getPortList());
            this.portThread = new PortThread(sp,this);
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
                            sp.writeByte(flag_enviar_rf_msg);
                        } catch (Exception e1) {
                            e1.printStackTrace();
                        }
                        break;
                    case MainWindowView.BTN_UART:
                        //TODO Afegir el codi per enviar per UART
                        portThread.setEscolta(false);
                        enviar(false);
                        break;
                }
            } catch (Exception e1) {
                e1.printStackTrace();
            }
            portThread.setEscolta(true);

		}
	}
	
	public void showView(){
		view.setVisible(true);
	}

	public void enviar(boolean confirmacio){
        try {
            if(model.checkInputText(view.getText())){
                byte resposta;
                byte[] utf8Bytes = view.getText().getBytes("UTF-8");
                if(confirmacio){
                    sp.writeByte(flag);
                }else{
                    sp.writeByte(flag_desar_msg);
                    resposta = sp.readByte();
                    while(resposta == 0){
                        resposta = sp.readByte();
                    }
                }

                //Part de id RF
                sp.writeByte(start_byte_rf);
                resposta = sp.readByte();
                while(resposta == 0){
                    resposta = sp.readByte();
                }
                sp.writeByte(primer_byte_part_ID);
                resposta = sp.readByte();
                while(resposta == 0){
                    resposta = sp.readByte();
                }
                sp.writeByte(segon_byte_part_ID);
                resposta = sp.readByte();
                while(resposta == 0){
                    resposta = sp.readByte();
                }
                sp.writeByte(tercer_byte_part_ID);
                resposta = sp.readByte();
                while(resposta == 0){
                    resposta = sp.readByte();
                }

                for (byte value:utf8Bytes) {
                    sp.writeByte(value);
                    resposta = sp.readByte();
                    while(resposta == 0){
                        resposta = sp.readByte();
                    }
                }
                sp.writeByte(end_byte_rf);
                resposta = sp.readByte();
                while(resposta == 0){
                    resposta = sp.readByte();
                }
                System.out.print(" Done Send!\n");
                sp.writeByte(end_byte);
                System.out.print(" Done!\n");

            }else{
                JOptionPane.showMessageDialog(null, "No has escrit cap missatge! ", "Error",JOptionPane.ERROR_MESSAGE);
            }

        } catch (Exception e1) {
            JOptionPane.showMessageDialog(null, "Error en enviar les dades!\n "+ e1.getMessage(), "Error",JOptionPane.ERROR_MESSAGE);

        }
    }

    public void start(){
        try {
            sp.openPort(view.getPort(),19200);
            portThread.setPort(sp);
            portThread.run();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void restart() {
        try {
            sp.closePort();
            sp.openPort(view.getPort(),19200);
            portThread.setPort(sp);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
