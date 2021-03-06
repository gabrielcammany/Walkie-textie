import com.SerialPort.SerialPort;
import controllers.MainWindowController;

/**
 * Created by gabri on 19/04/2017.
 */
public class PortThread implements Runnable {


    private SerialPort sp = null;
    private int baudRate;
    private String port;
    private boolean escolta = true;
    private MainWindowController mainWindowController = null;
    private final byte flag = (byte) (Integer.parseInt("84",16));

    public PortThread(SerialPort sp, MainWindowController mainWindowController){
        this.sp = sp;
        this.mainWindowController = mainWindowController;
    }

    public void run() {
        byte aux;
        while (true) {
            while (escolta) {
                try {
                    aux = sp.readByte();
                    Thread.sleep(2);
                    if (aux != 0) System.out.print(aux);
                    if (aux == flag) {
                        mainWindowController.enviar(true);
                        mainWindowController.restart();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            try {
                Thread.sleep(3);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void setEscolta(boolean escolta) {
        this.escolta = escolta;
    }

    public void setPort(SerialPort port) {
        this.sp = port;
    }
}