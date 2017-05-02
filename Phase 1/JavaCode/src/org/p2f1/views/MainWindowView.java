package org.p2f1.views;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ScrollPaneConstants;

import org.p2f1.controllers.MainWindowController;




public class MainWindowView  extends JFrame{

	private static final long serialVersionUID = 8459978615692456891L;
	
	public static final String BTN_UART = "BTN_UART";
	public static final String BTN_RF = "BTN_RF";
	
	//Constants que contenen la mida de la pantalla de l'usuari
	private static final int SCREEN_WIDTH = (int) Toolkit.getDefaultToolkit().getScreenSize().getWidth();
	private static final int SCREEN_HEIGHT = (int) Toolkit.getDefaultToolkit().getScreenSize().getHeight();
	
	//Constants que indiquen la mida de la finestra per defecte
	private static final int WINDOW_WIDTH = 600;
	private static final int WINDOW_HEIGHT = 400;
	
	//Declarem les variables dels elements de la finestra gràfica
	private JPanel topPanel = null;
	private JPanel centerPanel = null;
	private JPanel infoPanel = null;
	private JPanel portPanel = null;
	private JPanel filePanel = null;
	private JLabel lblPort = null;
	private JLabel lblBaudRate = null;
	private JLabel lblPath = null;
	private JButton btnRF = null;
	private JButton btnUART = null;
	private JTextArea jtaText = null;
	private JScrollPane sp = null;

	private JComboBox<Integer> comboBaud = new JComboBox<Integer>();
	private JComboBox<String> comboPort = new JComboBox<String>();
	
	
	//Constructor de la classe MainWindowView (finestra gràfica)
	public MainWindowView(){
		configureWindow();
		configureTopPanel();
		configureCenterPanel();
		configureBottomPanel();
		

	}
	
	//Configurem els paràmetres de la finestra
	private void configureWindow(){
		setTitle("[SDM] Pràctica 2");
		setSize(new Dimension(WINDOW_WIDTH,WINDOW_HEIGHT));
		setLocation(SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2, SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		setLayout(new BorderLayout());
	}
	
	//Configura el panell superior i els seus components
	private void configureTopPanel(){
		
		//Configurem els JPanels
		topPanel = new JPanel();
		infoPanel = new JPanel();
		portPanel = new JPanel();
		topPanel.setLayout(new BorderLayout());
		topPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		infoPanel.setLayout(new BoxLayout(infoPanel,BoxLayout.Y_AXIS));
		portPanel.setLayout(new FlowLayout());
		
		//Configurem les labels
		
		lblBaudRate = new JLabel("BaudRate: ");
		lblPort = new JLabel("Port: ");
		
		
		
		
		//Afegim els components al port Panel
		portPanel.add(lblBaudRate);
		comboBaud.setSelectedItem(19200);
		portPanel.add(comboBaud);
		portPanel.add(lblPort);
		portPanel.add(comboPort);
		
		//Afegim l'infoPanel al topPanel (alineat a l'esquerra)
		topPanel.add(infoPanel,BorderLayout.WEST);
		topPanel.add(portPanel,BorderLayout.EAST);

		
		//Afegim el topPanel a la finestra
		add(topPanel,BorderLayout.NORTH);
		
	}
	
	//Configura el panell central i els seus components
	private void configureCenterPanel(){
		
		//Creem els panells
		centerPanel = new JPanel();
		filePanel = new JPanel();
		centerPanel.setLayout(new BoxLayout(centerPanel,BoxLayout.Y_AXIS));
		filePanel.setLayout(new FlowLayout());
		filePanel.setMaximumSize(new Dimension(1000,50));
		
		//Creem els TextFields
		jtaText = new JTextArea ();
		sp = new JScrollPane(jtaText); 
		sp.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
	    sp.setBounds(3,3,400,400);
		sp.setPreferredSize(new Dimension(500,60));


				
		//Creem els botons
		btnUART = new JButton("Carrega Missatge");
		btnRF = new JButton("Envia RF");
		btnUART.setAlignmentX(Component.CENTER_ALIGNMENT);
		btnRF.setAlignmentX(Component.CENTER_ALIGNMENT);
		btnUART.setMaximumSize(new Dimension(200,50));
		btnRF.setMaximumSize(new Dimension(200,50));
		btnRF.setMinimumSize(new Dimension(200,50));
		btnUART.setMinimumSize(new Dimension(200,50));
		btnRF.setPreferredSize(new Dimension(200,50));
		btnUART.setPreferredSize(new Dimension(200,50));
		
		//Agefim els controls al panell del fitxer
		lblPath = new JLabel("  ");
		filePanel.add(lblPath);
		filePanel.add(sp);
		
		
		//Afegim els controls al panell central
		centerPanel.add(Box.createVerticalGlue());
		centerPanel.add(filePanel);
		centerPanel.add(Box.createRigidArea(new Dimension(50,50)));
		centerPanel.add(btnUART);
		centerPanel.add(Box.createRigidArea(new Dimension(50,50)));
		centerPanel.add(btnRF);
		centerPanel.add(Box.createVerticalGlue());
		
		//Afegim el panell central a la finestra
		add(centerPanel,BorderLayout.CENTER);
	}
	
	//Configura el panell inferior i els seus components
	//Configura el panell inferior i els seus components. 
	//Actualment no hi ha cap component.
	private void configureBottomPanel(){
		
		
	}
	

	public void associateController(MainWindowController controller){
		
		//Assignem el nom per distingir els botons
		btnUART.setName(BTN_UART);
		btnRF.setName(BTN_RF);
		//comboBaud.setName("BAUD");
		
		//Assignem el controlador dels botons
		btnUART.addActionListener(controller);
		btnRF.addActionListener(controller);
		 //comboBaud.addActionListener(controller);
		//comboPort.addActionListener(controller);

		
		
	}

	public void setPortsList(String [] lPorts){
		comboPort.removeAllItems();
		for(String item : lPorts){
			comboPort.addItem(item);
		}
	}
	
	public void setBaudRateList(int [] lBaudRates){
		comboBaud.removeAllItems();
		for(int item : lBaudRates){
			comboBaud.addItem(item);
		}
	}
	
	public String getText(){
		return jtaText.getText();
	}

	public String getPort(){
		return (String)comboPort.getSelectedItem();
	}

	public int getBaudRate(){
		return (int)comboBaud.getSelectedItem();
	}
	
}
