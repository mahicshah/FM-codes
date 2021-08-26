package com.example.gpa_calculator;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseEvent;
import java.net.URL;
import java.text.DecimalFormat;
import java.util.Formatter;
import java.util.ResourceBundle;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class HelloController{

    @FXML
    public TextField Subject;
    @FXML
    public TextField Credits;
    @FXML
    public TextField Grade;
    @FXML
    private Label GPA;
    @FXML
    private Button AddButton;
    @FXML
    private Button CalculateButton;
    @FXML
    private Button ResetButton;
    @FXML
    private ListView myList;


    double marks = 0, sum = 0, gpa = 0.00;
    int cred = 0, total_cred = 0;
    public String s, c, g;


    public void calculation(ActionEvent event) {

        s = Subject.getText();
        c = Credits.getText();
        g = Grade.getText();

        if (g.equals("A+"))
            marks = 10;

        else if (g.equals("A"))
            marks = 9;

        else if (g.equals("B"))
            marks = 8;

        else if (g.equals("C"))
            marks = 7;

        else if (g.equals("D"))
            marks = 6;

        else if (g.equals("E"))
            marks = 5;

        else if (g.equals("F"))
            marks = 0;

        else
            Grade.setText(("Invalid Grade."));

        cred = Integer.parseInt(Credits.getText());
        total_cred += cred;
        sum = sum + (cred * marks);

        gpa = sum / total_cred;

        double rounded_gpa = Math.round(gpa * 100.0) / 100.0;

        GPA.setText(String.valueOf(rounded_gpa));

        myList.getItems().add("                           "+Subject.getText() + "                                                            " +cred+ "                                                           " +g);

        System.out.println("subject = " + s);
        System.out.println("credits = " + cred);
        System.out.println("grade = " + g);
        System.out.println("marks = " + marks);
        System.out.println("sum = " + sum);
        System.out.println("gpa = " + gpa);

        Subject.setText("");
        Credits.setText("");
        Grade.setText("");
    }


    public void add(ActionEvent event) {
        g = Grade.getText();

        if (g.equals("A+"))
            marks = 10;

        else if (g.equals("A"))
            marks = 9;

        else if (g.equals("B"))
            marks = 8;

        else if (g.equals("C"))
            marks = 7;

        else if (g.equals("D"))
            marks = 6;

        else if (g.equals("E"))
            marks = 5;

        else if (g.equals("F"))
            marks = 0;

        else
            Grade.setText(("Invalid Grade."));

        cred = Integer.parseInt(Credits.getText());
        total_cred += cred;
        sum = sum + (cred * marks);

        gpa = sum / total_cred;

        double rounded_gpa = Math.round(gpa * 100.0) / 100.0;

        myList.getItems().add("                           "+Subject.getText() + "                                                            " +cred+ "                                                           " +g);

        System.out.println("total cred = " + total_cred);
        System.out.println("sum = " + sum);


        Subject.setText("");
        Credits.setText("");
        Grade.setText("");
    }

    public void reset(ActionEvent event) {
        marks = 0;
        sum = 0;
        gpa = 0.00;
        cred = 0;
        total_cred = 0;

        GPA.setText("0.00");
        Subject.setText("");
        Credits.setText("");
        Grade.setText("");

        myList.getItems().clear();
    }

//    public TableView<Product> tableview;
//    public TableColumn<Product, String> table_sub;
//    public TableColumn<Product, String> table_cred;
//    public TableColumn<Product, String> table_g;
//
//    @Override
//    public void initialize(URL url, ResourceBundle resourceBundle) {
//        table_sub.setCellValueFactory(new PropertyValueFactory<>("Product_sub"));
//        table_cred.setCellValueFactory(new PropertyValueFactory<>("Product_cred"));
//        table_g.setCellValueFactory(new PropertyValueFactory<>("Product_g"));
//        tableview.setItems(add_values());
//
//    }
//
//    public ObservableList<Product> add_values() {
//        ObservableList<Product> observableList = FXCollections.observableArrayList();
//        observableList.add(new Product("a", "b", "c"));
//        return observableList;
//    }
}
