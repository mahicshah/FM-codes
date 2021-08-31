package com.example.RealTimeDataRead;

import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.Scene;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.ComboBox;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import jssc.SerialPort;
import static jssc.SerialPort.MASK_RXCHAR;
import jssc.SerialPortEvent;
import jssc.SerialPortException;
import jssc.SerialPortList;
import eu.hansolo.medusa.Gauge;

public class JavaFX_jssc_Uno extends Application {

    Gauge gauge = new Gauge();

    SerialPort arduinoPort = null;
    ObservableList<String> portList;

    final int NUM_OF_POINT = 50;
    XYChart.Series series;

    private void detectPort() {

        portList = FXCollections.observableArrayList();

        String[] serialPortNames = SerialPortList.getPortNames();
        for(String name: serialPortNames){
            System.out.println(name);
            portList.add(name);
        }
    }

    @Override
    public void start(Stage primaryStage) {

        detectPort();
        final ComboBox comboBoxPorts = new ComboBox(portList);
        comboBoxPorts.valueProperty()
                .addListener(new ChangeListener<String>() {

                    @Override
                    public void changed(ObservableValue<? extends String> observable,
                                        String oldValue, String newValue) {

                        System.out.println(newValue);
                        disconnectArduino();
                        connectArduino(newValue);
                    }

                });

        final NumberAxis xAxis = new NumberAxis();
        final NumberAxis yAxis = new NumberAxis();
        xAxis.setLabel("Time");
        yAxis.setLabel("Voltage");

        final LineChart<Number,Number> lineChart =
                new LineChart<>(xAxis,yAxis);
        lineChart.setTitle("Real Time Line Chart");

        series = new XYChart.Series();
        lineChart.setLegendVisible(false);
        lineChart.getData().add(series);
        lineChart.setAnimated(false);
        lineChart.setCreateSymbols(false);

        for(int i=0; i<NUM_OF_POINT; i++){
            series.getData().add(new XYChart.Data(i, 0));
        }

        gauge.setMinValue(0.00);
        gauge.setMaxValue(255.00);
        gauge.setMaxHeight(100000);
        gauge.setMaxWidth(100000);

        VBox vBox = new VBox();
        vBox.getChildren().addAll(
                comboBoxPorts, lineChart, gauge);

        StackPane root = new StackPane();
        root.getChildren().add(vBox);

        Scene scene = new Scene(root, 500, 400);

        primaryStage.setTitle("Real Time Arduino Data + Speedometer");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    public void shiftSeriesData(float newValue)
    {
        for(int i=0; i<NUM_OF_POINT-1; i++){
            XYChart.Data<String, Number> ShiftDataUp =
                    (XYChart.Data<String, Number>)series.getData().get(i+1);
            Number shiftValue = ShiftDataUp.getYValue();
            XYChart.Data<String, Number> ShiftDataDn =
                    (XYChart.Data<String, Number>)series.getData().get(i);
            ShiftDataDn.setYValue(shiftValue);
        }
        XYChart.Data<String, Number> lastData =
                (XYChart.Data<String, Number>)series.getData().get(NUM_OF_POINT-1);
        lastData.setYValue(newValue);
    }

    public boolean connectArduino(String port){

        System.out.println("Arduino is connected.");

        boolean success = false;
        SerialPort serialPort = new SerialPort(port);
        try {
            serialPort.openPort();
            serialPort.setParams(
                    SerialPort.BAUDRATE_9600,
                    SerialPort.DATABITS_8,
                    SerialPort.STOPBITS_1,
                    SerialPort.PARITY_NONE);
            serialPort.setEventsMask(MASK_RXCHAR);
            serialPort.addEventListener((SerialPortEvent serialPortEvent) -> {
                if(serialPortEvent.isRXCHAR()){
                    try {

                        byte[] b = serialPort.readBytes();
                        int value = b[0] & 0xff;    //convert to int
                        String st = String.valueOf(value);

                        //Update label in ui thread
                        Platform.runLater(() -> {

                            gauge.setValue(Double.parseDouble(st));

                            shiftSeriesData((float)value * 5/255); //in 5V scale
                        });

                    } catch (SerialPortException ex) {
                        Logger.getLogger(JavaFX_jssc_Uno.class.getName())
                                .log(Level.SEVERE, null, ex);
                    }

                }
            });

            arduinoPort = serialPort;
            success = true;

        } catch (SerialPortException ex) {

            Logger.getLogger(JavaFX_jssc_Uno.class.getName())
                    .log(Level.SEVERE, null, ex);
            System.out.println("SerialPortException: " + ex.toString());
        }
        return success;
    }

    public void disconnectArduino() {

        System.out.println("Arduino is disconnected.");

        if(arduinoPort != null) {

            try {
                arduinoPort.removeEventListener();

                if(arduinoPort.isOpened()) {

                    arduinoPort.closePort();
                }

            } catch (SerialPortException ex) {

                Logger.getLogger(JavaFX_jssc_Uno.class.getName())
                        .log(Level.SEVERE, null, ex);
            }
        }
    }

    @Override
    public void stop() throws Exception {

        disconnectArduino();
        super.stop();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
