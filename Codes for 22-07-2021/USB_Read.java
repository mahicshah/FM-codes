import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortException;
import jssc.SerialPortList;

public class usbread {
  
    public void connect(String portname) {

        SerialPort port = new SerialPort("/dev/cu.usbmodem14101");

        try {
            port.openPort();
            port.setParams(
                    SerialPort.BAUDRATE_9600,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE
            );
            port.addEventListener((SerialPortEvent event)->{

                if(event.isRXCHAR()) {
                    try {
                        String s = port.readString();
                        System.out.print(s);
                    } catch (SerialPortException e) {
                        e.printStackTrace();
                    }
                }
            });

        } catch (SerialPortException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        String portlist[] = SerialPortList.getPortNames();

        for(int i =0; i<portlist.length;i++) {
            System.out.println(portlist[i]);
        }
        usbread obj = new usbread();
        obj.connect(portlist[0]);
    }
}
