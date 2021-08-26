package com.example.gpa_calculator;

import javafx.beans.property.SimpleStringProperty;
import javafx.css.SimpleStyleableStringProperty;

public class Product {

    private SimpleStringProperty product_sub;
    private SimpleStringProperty product_cred;
    private SimpleStringProperty product_g;

    public Product(String product_sub, String product_cred, String product_g) {
        this.product_sub = new SimpleStringProperty(product_sub);
        this.product_cred = new SimpleStringProperty(product_cred);
        this.product_g = new SimpleStringProperty(product_g);
    }

    public String getProduct_sub() {
        return product_sub.get();
    }

    public void setProduct_sub(String product_sub) {
        this.product_sub = new SimpleStringProperty(product_sub);
    }

    public String getProduct_cred() {
        return product_cred.get();
    }

    public void setProduct_cred(String product_cred) {
        this.product_cred = new SimpleStringProperty(product_cred);
    }

    public String getProduct_g() {
        return product_g.get();
    }

    public void setProduct_g(String product_g) {
        this.product_g = new SimpleStringProperty(product_g);
    }
}
