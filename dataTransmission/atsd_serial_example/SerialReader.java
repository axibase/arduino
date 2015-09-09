import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;

import java.io.DataOutputStream;
import java.io.InputStream;
import java.net.Socket;
import java.util.Enumeration;

public class SerialReader implements SerialPortEventListener,Runnable{
    public static void main(String[] argv) {
        String serial = "/dev/ttyACM0";
        if(argv.length > 0) {
            serial = argv[0];
        }
        SerialReader reader = new SerialReader(serial);
        reader.run();
    }

    SerialPort serialPort;
    private static final String host = "localhost";
    private static final int port = 8081;
    private InputStream input;
    private static final int TIME_OUT = 2000;
    private static final int DATA_RATE = 9600;
    private String row;
    private String serialPortPath;
    private Socket socket;
    private DataOutputStream stream;

    public SerialReader(String serialPortPath) {
        this.serialPortPath = serialPortPath;
        row = "";
    }

    public void run() {
        try {
            socket = new Socket(host, port);
            stream = new DataOutputStream(socket.getOutputStream());
        } catch (Exception e) {
            System.out.println("Can not connect to ATSD server, stdout will be using.");
        }

        CommPortIdentifier portId = null;
        System.setProperty("gnu.io.rxtx.SerialPorts", serialPortPath);//rxtx amd64 bugfix?
        Enumeration portEnum = CommPortIdentifier.getPortIdentifiers();
        while (portEnum.hasMoreElements()) {
            CommPortIdentifier currPortId = (CommPortIdentifier) portEnum.nextElement();
            if (currPortId.getName().equals(serialPortPath)) {
                portId = currPortId;
                break;
            }
        }
        if (portId == null) {
            System.out.println("Could not find specified COM port: " + serialPortPath);
            return;
        }
        try {
            // open serial port, and use class name for the appName.
            this.serialPort = (SerialPort) portId.open(this.getClass().getName(),
                    TIME_OUT);
            // set port parameters
            this.serialPort.setSerialPortParams(DATA_RATE,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);
            // open the streams
            this.input = this.serialPort.getInputStream();
            // add event listeners
            this.serialPort.addEventListener(this);
            this.serialPort.notifyOnDataAvailable(true);
        } catch (Exception e) {
            System.err.println(e.toString());
            close();
            System.exit(1);
        }
    }

    /**
     * This should be called when you stop using the port.
     * This will prevent port locking on platforms like Linux.
     */
    public synchronized void close() {
        if (this.serialPort != null) {
            this.serialPort.removeEventListener();
            this.serialPort.close();
        }
    }


    /**
     * Handle an event on the serial port. Read the data and print it.
     */
    public synchronized void serialEvent(SerialPortEvent oEvent) {
        if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
            try {
                int available = input.available();
                byte chunk[] = new byte[available];
                input.read(chunk, 0, available);
                row+= new String(chunk);
                int eolPos = row.indexOf('\n');
                if(eolPos != -1) {
                    if(row.indexOf("series") < 0) {
                        row = "";
                    } else {
                        if(socket.isConnected()) {
                            stream.writeBytes(row.substring(row.indexOf("series"), eolPos + 1));
                            System.out.print("sended: " + row.substring(0, eolPos + 1));

                        } else {
                            System.out.print(row.substring(0, eolPos));
                        }
                        row = row.substring(eolPos+1, row.length());
                    }
                }
            } catch (Exception e) {
                System.err.println(e.toString());
                close();
                System.exit(1);
            }
        }
    }

}
